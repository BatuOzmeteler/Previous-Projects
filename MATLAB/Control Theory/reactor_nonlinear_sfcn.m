
% reactor_nonlinear_sfcn.m

% --------------------------------------------------------------------
% Simulink Function for the Simulation of the Non-linear System
% --------------------------------------------------------------------

function [sys, x0, str, ts] = reactor_nonlinear_sfcn(t, x, u, flag)


% Choose the function performed currently by the S-function
switch flag
    
    % Initialization
    case 0
        
        str = [];       % Empty (default behavior)
        ts  = [0 0];    % Default values for continuous systems
        
        % Dimensions of the system (states, inputs and outputs)
        sys_dims = simsizes;    % simsizes: MATLAB construct for 
                                %           initialization purposes
        
        % Problem-specific dimensions
        % The names of the fields of sys_dims are expected by Simulink
        sys_dims.NumContStates  = 4;  % Num. continuous states
        sys_dims.NumDiscStates  = 0;  % Num. discontinuous states
        sys_dims.NumInputs      = 2;  % Num. of inputs (Fr, Qj)
        sys_dims.DirFeedthrough = 0;  % Num. of feedthroughs (matrix D)
        sys_dims.NumSampleTimes = 1;  % Default for continuous systems
        
        sys_dims.NumOutputs = 4; % Num. of outputs (C_A, C_B, T_R, T_J;
                                 % The measurements are specified in
                                 % the block diagram with the matrix C)
        
        % Output: structure with system dimensions
        sys = simsizes(sys_dims);
        
        % actual initial conditions 
        x0_actual = [0; 0; 387.05; 387.05];
        
        % part7 initial conditions 
        C_A_ss = 1.6329;
        C_B_ss = 1.1101;
        T_R_ss = 398.6581;
        T_J_ss = 397.3736;
        
        x0_7_1 = [C_A_ss*0.9; C_B_ss*0.9; T_R_ss - 10; T_J_ss - 10]; 
        x0_7_2 = [C_A_ss*1.1; C_B_ss*1.1; T_R_ss + 10; T_J_ss + 10];
        
        % USER: Output: initial conditions
        x0 = x0_7_1;       
        
        
    % Evaluation of the derivatives
    case 1
        
        % Output: RHS of the ODE system
        sys = reactor_model_ode_rhs(x, u);
        
        
    % Evaluation of the outputs (y = C*x)
    case 3
        
        % System outputs
        % (the measurements are specified in
        % the block diagram with the matrix C)
        sys = x;                                         
        
        
    % Additional flags (values = 2, 4 and 9)
    case {2 4 9}
        
        sys = [];
        
    otherwise
        
        error('Unknown flag');
        
end

% --------------------------------------------------------------------
% EOF
% --------------------------------------------------------------------
