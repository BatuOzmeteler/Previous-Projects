classdef MPC_Controller_v1 < matlab.System & matlab.system.mixin.Propagates
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
    
        T = 0.2;     % Sample Time (s)
        N = 5;       % Prediction Horizon (# of steps)
      
        Q = 5;       % Q*(Current State - Reference State)^2
        R = 0.0001;  % R*(Current Control - Reference Control)^2
        
        Fbrake = 0;  % Braking Force (N)
        Grade  = 0;  % Grade (%)   
    end

    properties (DiscreteState)
        X0;
        u0;        
    end

    properties (Access = private)
        % Pre-computed constants.
        rpm_calc;
        solver;
    end

    methods (Access = protected)
        function [sz,dt,cp] = getDiscreteStateSpecificationImpl(obj,name)
            if strcmp(name,'X0')
                sz = [obj.N+1,1];
                dt = 'double';
                cp = false;
            elseif strcmp(name,'u0')
                sz = [obj.N,1];
                dt = 'double';
                cp = false;
            else
                error(['Error: Incorrect State Name: ', name.']);
            end
        end
        function num = getNumInputsImpl(~)
            num = 4; %cur_sim_tim, cur_speed, ref_speed, ref_control
        end
        function num = getNumOutputsImpl(~)
            num = 1; %trq_req
        end
        function [dt1] = getOutputDataTypeImpl(~)
        	dt1 = 'double'; %torque request
        end
        function [dt1, dt2, dt3, dt4] = getInputDataTypeImpl(~)
            dt1 = 'double'; %current simulation time
            dt2 = 'double'; %current speed
            dt3 = 'double'; %reference speed
            dt4 = 'double'; %reference control
        end
        function [sz1] = getOutputSizeImpl(~)
        	sz1 = [1,1];
        end
        function [sz1, sz2, sz3, sz4] = getInputSizeImpl(~)
        	sz1 = [1,1];
            sz2 = [1,1];
            sz3 = [1,50];
            sz4 = [1,50];
        end
        function [cp1, cp2, cp3, cp4] = isInputComplexImpl(~)
        	cp1 = false;
            cp2 = false;
            cp3 = false;
            cp4 = false;
        end
        function [cp1] = isOutputComplexImpl(~)
        	cp1 = false;
        end
        function [fz1,fz2,fz3,fz4] = isInputFixedSizeImpl(~)
        	fz1 = true;
            fz2 = true;
            fz3 = true;
            fz4 = true;
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
          
            obj.u0 = zeros(obj.N,1);
            obj.X0 = zeros(obj.N+1,1);
                
            % Conversion Metrics
            % 1 rad/s = 1/2*pi Hz = 60/2*pi RPM
            % 1 m/s   = 3.6 km/h
            % 1 hP    = 745.7 W

            % Constants for Vehicle Dynamics
            Rtire  =  0.354;  % radius of tire(m)
            G      =  9.81;   % earth's gravitational constant(m/s2)
            mu     =  0.09;   % road friction(N) 
            rho    =  1.225;  % density of air as a fluid(kg/m3)
            Cd     =  0.55;   % air drag coefficient
            A      =  5.1;    % vehicle frontal area(m2)
            mss    =  2100;   % vehicle mass(kg)
            ng     =  0.85;   % tire-specific constant
            Im     =  0.18;   % motor inertia(kg*m2)
            Gb     =  18.5;   % total gear ratio

            % State Variables and Vectors
            states = SX.sym('v');      % velocity(m/s)       
            n_states = length(states);

            % Control Variables, Vectors
            controls = SX.sym('trq');  % torque(N*m)
            n_controls = length(controls);

            % Variables for Vehicle Dynamics                                           
            alpha     =  atan(obj.Grade/100);                         % degree of slope(rad)
            Faero     =  0.5*rho*A*Cd*(states^2);                     % drag force against vehicle(N)
            Frub      =  mu*mss*G;                                    % rubbing force against tires(N)
            Ftire     =  controls*Gb/Rtire;                           % driving force(N)
            dvdt      =  (Ftire - Frub - Faero - mss*G*sin(alpha)...
                - obj.Fbrake)/(mss + Im*Gb^2/(ng*Rtire^2));           %acceleration(m/s2)

            % Objective Function and Constraints Vector
            J = 0; g = [];                                              

            % Non-Linear Mapping Function f(x,u) and System R.H.S
            f = Function('f',{states,controls},{dvdt});

            U = SX.sym('U',n_controls,obj.N);        % decision variables (controls)
            X = SX.sym('X',n_states,(obj.N+1));      % a vector that represents the states over the optimization problem
            P = SX.sym('P',n_states + obj.N*(n_states+n_controls)); % parameters (which include the initial state and 
            %the reference along the predicted trajectory (reference states and reference controls))

            % Computation of the Objective Function and Constraints Vector

            init_state = X(1);       % initial state
            g = [g;(init_state-P(1))]; % initial condition constraints
            for k=1:obj.N
                current_state = X(k); current_control = U(k);
                
                %if k == obj.N
                %    next_control = U(k);
                %else
                %    next_control = U(k+1);
                %end
                
                J = J + ((current_state-P(2*k))'*obj.Q*(current_state-P(2*k))) + ...
                          ((current_control-P(2*k+1))'*obj.R*(current_control-P(2*k+1))); %+ ...
                          %((next_control-current_control)*obj.S*(next_control-current_control));
                
                f_value  = f(current_state,current_control);
                next_state = X(k+1);  
                next_state_pred = current_state + (obj.T*f_value);
                g = [g;next_state - next_state_pred];
            end

        % Non-Linear Programming Problem Initialization
        opt_variables = [reshape(X,obj.N+1,1);reshape(U,obj.N,1)];
        nlp_prob = struct('f', J, 'x', opt_variables, 'g', g, 'p', P);

        opts = struct;
        opts.ipopt.max_iter = 2000;
        opts.ipopt.print_level =0;%0,3
        opts.print_time = 0;
        opts.ipopt.acceptable_tol =1e-8;
        opts.ipopt.acceptable_obj_change_tol = 1e-6;

        rpm = (states/Rtire)*(60/(2*pi))*Gb; % revolutions per minute(rpm)
        
        obj.rpm_calc  = Function('rpm_calc', {states},{rpm});
        obj.solver    = nlpsol('solver', 'ipopt', nlp_prob, opts);
        end

        function [trq_req] = stepImpl(obj, cur_sim_tim, cur_speed,...
                ref_speed, ref_control)      
            
            inst = uint8((cur_sim_tim/obj.T) + 1); % kth instance of simulation
            x0 = cur_speed;

            if inst > length(ref_speed) - obj.N
                xs = ref_speed(inst:end);
                cs = ref_control(inst:end);
                
                check = (length(ref_speed) - inst + 1); %to check how many 
                %reference velocity points are supposed to be added to have
                %exact N points at every step
                
                for iter = check:obj.N-1 %array completion
                    xs(length(xs)+1) = xs(end); 
                    cs(length(cs)+1) = cs(end);
                end
            else
                xs = ref_speed(inst:(inst+obj.N-1));  % reference speed
                cs = ref_control(inst:(inst+obj.N-1));% reference control
            end
            
%             motor_inv_max_trq_output   = [250,250,249,243,215,162,132.5,112.5,97,84,79.6];
%             motor_inv_max_rpm_input    = [0,4000,5000,5500,6000,7000,8000,9000,10000,11000,12000];         
%             
%             maxAvailableTrq = interp1(motor_inv_max_rpm_input,motor_inv_max_trq_output,full(obj.rpm_calc(x0)));
%             
%             if  isnan(maxAvailableTrq)
%                 maxAvailableTrq = 250; 
%             end
            
            maxAvailableTrq = 250;
            
            v_max    = 40;  v_min   = 0; 
            trq_max =  maxAvailableTrq; trq_min = -maxAvailableTrq;
            
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
            
            args.p(1) = x0; % initial condition of the robot posture
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

        function resetImpl(~)
            % Initialize discrete-state properties.
        end
    end
end
