%This MATLAB Script generates the S-Function of an NLP
%solver which is used in a MPC Controller.

function  generate_s_func(sample_time, prediction_horizon, coefficient_Q, coefficient_R,...
    braking_force, trajectory_length, reference_control, reference_speed, grade_data)

    % CasADi v3.5.1
        addpath('C:\Program Files\casadi-windows-matlabR2014b-v3.5.1')
        import casadi.*
        
        load('X'); load('Y'); load('Z'); load('Speed'); load('Torque');
        x = X(2,:); y = Y(2,:); z = Z(2,:);
        
        Speed(2,end) = 0;                     % final value must be positive or zero(it was negative)
        ref_speed = floor(Speed(2,:) ./ 3.6); % km/h to m/s conversion
        
        T = sample_time;              % Sample Time (s)
        N = prediction_horizon        % Prediction Horizon (# of steps)
        Q = coefficient_Q;            % Q*(Current State - Reference State)^2
        R = coefficient_R;            % R*(Current Control - Reference Control)^2
        Fbrake = braking_force;       % Braking Force (N)
        Trajlen = trajectory_length;  % Trajectory Length (# of steps)
        
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