classdef MPC_Controller_v2_2016 < matlab.System & matlab.system.mixin.Propagates
    
    % This MATLAB System Block formulates the non-linear programming problem for the MPC Implementation.
    % It initializes and outputs the solver once later to be evaluated. 
    
    % This template includes the minimum set of functions required
    % to define a System object with discrete state.

    properties
    %By default, these are public, tunable properties meaning 
    %their value can change at any time of the simulation.

    end
    
    properties(Nontunable)
    % Nontunable attribute for a property is used when the algorithm 
    % depends on the value being constant once data processing starts.
    
    %S = 0.01;   % S*(Next Control - Current Control)^2
    
    % 1. Constants for MPC Design %
    % 2. Weighing Matrices for States and Controls %
    % 3. Constants for Vehicle Dynamics %
    
        T = 1;            % Sample Time (s)
        N = 5;              % Prediction Horizon (# of steps)
      
        Q = 5;              % Q*(Current State - Reference State)^2
        R = 0.0001;         % R*(Current Control - Reference Control)^2
        
        Fbrake = 0;         % Braking Force (N)
        Trajlen = 2500;     % Trajectory Length (# of steps)
    end

    properties (DiscreteState)
    %If your algorithm uses properties that hold state, 
    %you can assign those properties the DiscreteState attribute.
    
    % Updated Variables at Each Time-Step
        X0;          % previous states over the horizon   (predicted)  
        u0;          % previous controls over the horizon (predicted)
    end

    properties (Access = private)
    % Conversion Metrics
            % 1 rad/s = 1/2*pi Hz = 60/2*pi RPM
            % 1 m/s   = 3.6 km/h
            % 1 hP    = 745.7 W
        
    % Constants for Vehicle Dynamics
        Rtire;       % radius of tire(m)
        G;           % earth's gravitational constant(m/s2)
        mu;          % road friction(N) 
        rho;         % density of air as a fluid(kg/m3)
        Cd;          % air drag coefficient
        A;           % vehicle frontal area(m2)
        mss;         % vehicle mass(kg)
        ng;          % tire-specific constant
        Im;          % motor inertia(kg*m2)
        Gb;          % total gear ratio
    % Symbols for States, Controls and Constraints
        states;      % velocity(m/s)
        controls;    % torque(N*m)
    % Variables for Vehicle Dynamics     
        alpha;       % degree of slope(rad)
        Faero;       % drag force against vehicle(N)
        Frub;        % rubbing force against tires(N)
        Ftire;       % driving force(N)
        dvdt;        % acceleration(m/s2)
    % Objective Function and Constraints Vector    
        J;           % objective function
        g;           % constraints vector
    % Non-Linear Mapping Function f(x,u) and System R.H.S
        f;       % non-linear mapping function
        U;       % decision variables (controls)
        X;       % a vector that represents the states over the optimization problem
        P;       % parameters (which include the initial state and the reference along 
                 % the predicted trajectory (reference states and reference controls))
    % Non-Linear Programming Problem Initialization
        opt_variables  % optization variables
        nlp_prob       % non-linear problem structure initalization
        opts           % simulation properties
    % Pre-computed Functions.
        rpm_calc
        solver
    end

    methods (Access = protected)
        function [sz, dt, cp] = getDiscreteStateSpecificationImpl(obj, name)
            if strcmp(name, 'X0')
                sz = [obj.N + 1, 1];
                dt = 'double';
                cp = false;
            elseif strcmp(name, 'u0')
                sz = [obj.N, 1];
                dt = 'double';
                cp = false;
            else
                error(['Error: Incorrect State Name: ', name.']);
            end
        end
        function num = getNumInputsImpl(~)
            num = 5; %cur_sim_tim, cur_speed, ref_speed, ref_control, grade
        end
        function num = getNumOutputsImpl(~)
            num = 1; %trq_req
        end
        function [dt1] = getOutputDataTypeImpl(~)
        	dt1 = 'double'; %torque request
        end
        function [dt1, dt2, dt3, dt4, dt5] = getInputDataTypeImpl(~)
            dt1 = 'double'; %current simulation time
            dt2 = 'double'; %current speed
            dt3 = 'double'; %reference speed
            dt4 = 'double'; %reference control
            dt5 = 'double'; %grade
        end
        function [sz1] = getOutputSizeImpl(~)
        	sz1 = [1, 1];
        end
        function [sz1, sz2, sz3, sz4, sz5] = getInputSizeImpl(obj)
        	sz1 = [1, 1];
            sz2 = [1, 1];
            sz3 = [1, obj.Trajlen];
            sz4 = [1, obj.Trajlen];
            sz5 = [1, obj.Trajlen];
        end
        function [cp1, cp2, cp3, cp4, cp5] = isInputComplexImpl(~)
        	cp1 = false;
            cp2 = false;
            cp3 = false;
            cp4 = false;
            cp5 = false;
        end
        function [cp1] = isOutputComplexImpl(~)
        	cp1 = false;
        end
        function [fz1, fz2, fz3, fz4, fz5] = isInputFixedSizeImpl(~)
        	fz1 = true;
            fz2 = true;
            fz3 = true;
            fz4 = true;
            fz5 = true;
        end
        function [fz1] = isOutputFixedSizeImpl(~)
        	fz1 = true;
        end
        function setupImpl(obj)
            % Implement tasks that need to be performed only once, 
            % such as pre-computed constants.
            
            % CasADi v3.5.1
            addpath('C:\Program Files\casadi-windows-matlabR2016a-v3.5.1')
            import casadi.*
            
            % Updated Variables at Each Time-Step
            obj.u0 = zeros(obj.N, 1);
            obj.X0 = zeros(obj.N + 1, 1);

            % Constants for Vehicle Dynamics
            obj.Rtire  =  0.354;  obj.G      =  9.81;   
            obj.mu     =  0.09;   obj.rho    =  1.225; 
            obj.Cd     =  0.55;   obj.A      =  5.1;    
            obj.mss    =  4200;   obj.ng     =  0.85;   
            obj.Im     =  0.18;   obj.Gb     =  18.5;   

            % Symbols for States, Controls and Changing Constraints
            obj.states = SX.sym('v'); n_states = length(obj.states);
            obj.controls = SX.sym('trq'); n_controls = length(obj.controls);

            % Variables for Vehicle Dynamics
            obj.alpha     =  atan(0 / 100);
            obj.Faero     =  0.5*obj.rho*obj.A*obj.Cd*(obj.states^2);                     
            obj.Frub      =  obj.mu*obj.mss*obj.G;                                   
            obj.Ftire     =  obj.controls*obj.Gb / obj.Rtire;
            obj.dvdt      =  (obj.Ftire - obj.Frub - obj.Faero - obj.mss*obj.G*sin(obj.alpha)...
                - obj.Fbrake)/(obj.mss + obj.Im*obj.Gb^2 / (obj.ng*obj.Rtire^2));

            % Objective Function and Constraints Vector
            obj.J = 0; obj.g = [];                                              

            % Non-Linear Mapping Function f(x,u) and System R.H.S
            obj.f = Function('f', {obj.states, obj.controls}, {obj.dvdt});
            obj.U = SX.sym('U', n_controls, obj.N);       
            obj.X = SX.sym('X', n_states, (obj.N + 1));    
            obj.P = SX.sym('P', n_states + obj.N*(n_states + n_controls));  
            
            % Computation of the Objective Function
                        
            % Caching of the System Object Data in Local Variable Before 
            % Multiple Access in Each Iteration in a Loop
                        
            for k = 1:obj.N
                current_state = obj.X(k); current_control = obj.U(k);
                
                %if k == obj.N
                %    next_control = U(k);
                %else
                %    next_control = U(k+1);
                %end
                
                obj.J = obj.J + ((current_state - obj.P(2*k))'*obj.Q*(current_state - obj.P(2*k))) + ...
                          ((current_control - obj.P(2*k+1))'*obj.R*(current_control - obj.P(2*k+1))); %+ ...
                          %((next_control-current_control)*obj.S*(next_control-current_control));
            end

            % Non-Linear Programming Problem Initialization
            obj.opt_variables = [reshape(obj.X, obj.N+1, 1);reshape(obj.U, obj.N, 1)];
            %obj.nlp_prob = struct('f', obj.J, 'x', obj.opt_variables, 'g', obj.g, 'p', obj.P);

            obj.opts = struct;
            obj.opts.ipopt.max_iter = 2000;
            obj.opts.ipopt.print_level =0;%0,3
            obj.opts.print_time = 0;
            obj.opts.ipopt.acceptable_tol =1e-8;
            obj.opts.ipopt.acceptable_obj_change_tol = 1e-6;

            rpm = (obj.states/obj.Rtire)*(60/(2*pi))*obj.Gb; % revolutions per minute(rpm)

            obj.rpm_calc  = Function('rpm_calc', {obj.states},{rpm});
            %obj.solver    = nlpsol('solver', 'ipopt', obj.nlp_prob, obj.opts);
        end

        function [trq_req] = stepImpl(obj, cur_sim_tim, cur_speed,...
                ref_speed, ref_control, grade)      
            import casadi.* 
            inst = uint16((cur_sim_tim/obj.T) + 1); % kth instance of simulation
            
            if inst > length(ref_speed) - obj.N
                xs = ref_speed(inst:end);
                cs = ref_control(inst:end);
                gs = grade(inst:end);
                
                check = (length(ref_speed) - inst + 1); %to check how many 
                %reference velocity points are supposed to be added to have
                %exact N points at every step
                
                for iter = check:obj.N-1 %array completion
                    xs(length(xs)+1) = xs(end); 
                    cs(length(cs)+1) = cs(end);
                    gs(length(gs)+1) = gs(end);
                end
            else
                xs = ref_speed(inst:(inst+obj.N-1));   % reference speed
                cs = ref_control(inst:(inst+obj.N-1)); % reference control
                gs = grade(inst:(inst+obj.N-1));       % grade data
            end
                        
%             motor_inv_max_trq_output   = [250,250,249,243,215,162,132.5,112.5,97,84,79.6];
%             motor_inv_max_rpm_input    = [0,4000,5000,5500,6000,7000,8000,9000,10000,11000,12000];         
%             
%             maxAvailableTrq = interp1(motor_inv_max_rpm_input,motor_inv_max_trq_output,full(obj.rpm_calc(cur_speed)));
%             
%             if  isnan(maxAvailableTrq)
%                 maxAvailableTrq = 250; 
%             end
            
            maxAvailableTrq = 290;
            
            v_max    = 55;  v_min   = 0; 
            trq_max =  maxAvailableTrq; trq_min = -maxAvailableTrq;
            
            calc_constraints_vect(obj,gs);
            obj.nlp_prob = struct('f', obj.J, 'x', obj.opt_variables, 'g', obj.g, 'p', obj.P);
            obj.solver   = nlpsol('solver', 'ipopt', obj.nlp_prob, obj.opts);
            
            args = struct;
            
            %Equality Constraints
            args.lbg(1:obj.N+1) = 0;    
            args.ubg(1:obj.N+1) = 0;     

            %States Upper and Lower Bounds
            args.lbx(1:obj.N+1) = v_min; %velocity lower bound
            args.ubx(1:obj.N+1) = v_max; %velocity upper bound

            %Controls Upper and Lower Bounds
            args.lbx((obj.N+1)+1:(2*obj.N)+1) = trq_min; %torque lower bound
            args.ubx((obj.N+1)+1:(2*obj.N)+1) = trq_max; %torque upper bound
            
            args.p(1) = cur_speed; % initial condition of the robot posture
            for k = 1:obj.N %new - set the reference to track
                args.p(2*k)   = xs(k);
                args.p(2*k+1) = cs(k);
            end
            
            args.x0  = [reshape(obj.X0',(obj.N+1),1);reshape(obj.u0',obj.N,1)];
            
            sol = obj.solver('x0', args.x0, 'lbx', args.lbx, 'ubx', args.ubx,...
            'lbg', args.lbg, 'ubg', args.ubg,'p',args.p); 
            
            u = reshape(full(sol.x((obj.N+1)+1:end))',1,obj.N)'; %control actions for the given prediction horizon
            obj.X0 = full(sol.x(1:(obj.N+1))); %predicted states 
            trq_req = u(1); %first control action is output to be applied
            obj.u0 = [u(2:size(u,1));u(size(u,1))]; %shift controls to initialize the next step
            obj.X0 = [obj.X0(2:end);obj.X0(end)]; % shift trajectory to initialize the next step
        end

        function calc_constraints_vect(obj, grade_data)
            import casadi.*            
            g_rep = [];             Ftire_rep = obj.Ftire;  Frub_rep  = obj.Frub;   Fbrake_rep = obj.Fbrake;
            ng_rep = obj.ng;        Faero_rep = obj.Faero;  mss_rep = obj.mss;      U_rep = obj.U;   
            G_rep = obj.G;          Im_rep = obj.Im;        Gb_rep = obj.Gb;        N_rep = obj.N;
            Rtire_rep = obj.Rtire;  X_rep = obj.X;          T_rep = obj.T;          P_rep = obj.P;
            
            init_state = X_rep(1);                    % initial state
            g_rep = [g_rep; (init_state - P_rep(1))]; % initial condition constraints
            for k = 1:N_rep
                alpha_rep = atan(grade_data(k) / 100);
                dvdt_rep = (Ftire_rep - Frub_rep - Faero_rep - mss_rep*G_rep*sin(alpha_rep) - Fbrake_rep)/(mss_rep + Im_rep*Gb_rep^2 / (ng_rep*Rtire_rep^2));
                func = Function('func', {obj.states, obj.controls}, {dvdt_rep});
                
                current_state = X_rep(k); current_control = U_rep(k);
                f_value  = func(current_state,current_control);
                next_state = X_rep(k+1);  
                next_state_pred = current_state + (T_rep*f_value);
                g_rep = [g_rep; next_state - next_state_pred];
            end
            obj.alpha = alpha_rep;
            obj.dvdt  = dvdt_rep;
            obj.g = g_rep;
        end 
        
        function resetImpl(~)
            % Initialize discrete-state properties.
        end
    end
end
