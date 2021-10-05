
% reactor_model_ode_rhs.m 

function f = reactor_model_ode_rhs(x, u)

% --------------------------------------------------------------------

% States
C_A = x(1);             % Concentration of Component A [kmol / m3]
C_B = x(2);             % Concentration of Component B [kmol / m3]
T_R = x(3);             % Reactor Temperature [K]
T_J = x(4);             % Jacket Temperature  [K]

% Inputs
Fr = u(1);              % Feed Volumetric Flowrate [m3 / min]
Qj = u(2);              % Heat Removal by The Jacket [kJ / min]

% --------------------------------------------------------------------

% Parameters
C_A_in = 5.1;       % Component A Inlet Concentration [kmol / m3]
V = 0.01;           % Reactor Volume [m3]
k_01 = 2.145e10;    % Pre-exponential Factor - First Reaction [1/min]  
k_02 = 2.145e10;    % Pre-exponential Factor - Second Reaction [1/min]
E_R1 = 9758.3;      % Reaction Activation Energy - First Reaction [K]
E_R2 = 9758.3;      % Reaction Activation Energy - Second Reaction [K]
deltaH_R1 = -4200;  % Heat of Reaction - First Reaction [kJ / kmol]
deltaH_R2 = -11000; % Heat of Reaction - Second Reaction [kJ / kmol]
T_in = 387.05;      % Inlet Temperature [K]
rho = 934.2;        % Liquid Density [kg / m3]
cp = 3.01;          % Heat Capacity of the Reaction Medium [kJ / kg*K]
cp_j = 2.0;         % Heat Capacity of the Jacket Medium [kJ / kg*K]
m_j = 5.0;          % Coolant Mass [kg]
kA = 14.448;        % Heat Transfer Coefficient [kJ / min*K]

% --------------------------------------------------------------------

% Differential Equations

% dC_A / dt
f(1,1) = ((Fr / V) * (C_A_in - C_A)) - (k_01*exp(-E_R1 / T_R)*C_A);

% dC_B / dt
f(2,1) = ((-Fr / V)*C_B) + (k_01*exp(-E_R1 / T_R)*C_A) - (k_02*exp(-E_R2 / T_R)*C_B);

% dT_R / dt
f(3,1) = ((Fr / V)*(T_in - T_R)) - ((k_01*exp(-E_R1 / T_R)*C_A*deltaH_R1) / (rho*cp)) ...
    - ((k_02*exp(-E_R2 / T_R)*C_B*deltaH_R2) / (rho*cp)) - ((kA*(T_R - T_J)) / (rho*cp*V));


% dT_J / dt
f(4,1) = ((kA*(T_R - T_J)) - Qj) / (m_j*cp_j);

end