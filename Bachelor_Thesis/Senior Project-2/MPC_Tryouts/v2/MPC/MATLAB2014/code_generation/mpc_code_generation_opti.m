clear all
close all
clc

% CasADi v3.5.1
addpath('C:\Program Files\casadi-windows-matlabR2014b-v3.5.1')
import casadi.*

% Constants for MPC Design
T = 0.2; % sampling time(s)
N = 3;  % prediction horizon

Q = 10;
R = 0.001;

% Constants for Vehicle Dynamics
Rtire  =  0.354;  G      =  9.81;   
mu     =  0.09;   rho    =  1.225;  
Cd     =  0.55;   A      =  5.1;    
mss    =  2100;   ng     =  0.85;   
Im     =  0.18;   Gb     =  18.5;   

opti = casadi.Opti();

% State Variables and Vectors
states = SX.sym('v');      % velocity(m/s)      

% Control Variables, Vectors
controls = SX.sym('trq');  % torque(N*m)

% Variables for Vehicle Dynamics                                           
Fbrake    =  0;                             % braking force(N)
grade     =  SX.sym('grade');               % grade(%)
alpha     =  atan(grade/100);               % degree of slope(rad)
Faero     =  0.5*rho*A*Cd*(states^2);       % drag force against vehicle(N)
Frub      =  mu*mss*G;                      % rubbing force against tires(N)
Ftire     =  controls*Gb/Rtire;             % driving force(N)

dvdt =  (Ftire - Frub - Faero - mss*G*sin(alpha) - Fbrake)/(mss + Im*Gb^2/(ng*Rtire^2)); %acceleration(m/s2)

f = Function('f',{states,controls,grade},{dvdt});

u = opti.variable(1,N);
x = opti.variable(1,N+1);
p = opti.parameter(1,3*N+1);

J = 0;
for k = 1:N
    current_state = x(k); current_control = u(k);
    J = J + ((current_state - p(3*k-1))'*Q*(current_state - p(3*k-1))) + ...
              ((current_control - p(3*k))'*R*(current_control - p(3*k)));
end
opti.minimize(J);

opti.subject_to(x(1) == p(1));
for k = 1:N
    opti.subject_to(x(k+1) == x(k)+(T*f(x(k),u(k),p(3*k+1)))); 
end
opti.subject_to(-250 <= u(:) <= 250);
opti.subject_to(0 <= x(:) <= 60);

%opti.solver('sqpmethod',struct('qpsol','qrqp'));
%opti.set_value(p(1),0);
%for k=1:N
    %opti.set_value(p(2*k),20);
    %opti.set_value(p(2*k+1), 0);
%end

opts = struct;
opts.qpsol = 'qrqp';
opts.print_header = false;
opts.print_iteration = false;
opts.print_time = false;
opts.max_iter = 2000;
opts.qpsol_options.print_iter = false;
opts.qpsol_options.print_header = false;
opts.qpsol_options.print_info = false;

opti.solver('sqpmethod', opts);
M = opti.to_function('M',{p},{x});
M.save('M.casadi')

% cg_options = struct;
% cg_options.casadi_real = 'real_T';
% cg_options.casadi_int = 'int_T';
% cg_options.with_header = true;
% cg = CodeGenerator('M',cg_options);
% cg.add_include('simstruc.h');
% cg.add(M);
% cg.generate();


M.generate('codegen_demo.c',struct('mex',true));
% mex codegen_demo.c -DMATLAB_MEX_FILE

% lib_path = GlobalOptions.getCasadiPath();
% inc_path = GlobalOptions.getCasadiIncludePath();
% mex('-v',['-I' inc_path],['-L' lib_path],'-lcasadi', 'codegen_demo.c')


% parameters = [0, 40, 0, 1, 40, 0, 1, 40, 0, 1];
% 
% result = full(M(parameters));

%sol = opti.solve();
%s_f = Function('s_f',{cur_speed},{sol.value(x)});

% X_hist = [];
% U_hist = [];
% 
% for i = 1:6*N
%     sol = opti.solve();
%     X_hist(:,i) = sol.value(x);
%     U_hist(:,i) = sol.value(u);
%     
%     opti.set_value(p(1), X_hist(1,i)+ T*f(X_hist(1,i),U_hist(1,i)));
% end
% 
% U_hist(end,1)


