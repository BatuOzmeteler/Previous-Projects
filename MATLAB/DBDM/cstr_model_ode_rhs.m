function f = cstr_model_ode_rhs(t, x, p)

% -------------------------------------------------------------------------

% States
T_R    = x(1);
T_J    = x(2);
T_Heat = x(3);

% Inputs
% step_input = [zeros(1, 100) ones(1, 850)]';

Q_Heat = 1;

% -------------------------------------------------------------------------

% Parameters

T_E = 293;                  % temperature of the environment (K) 
m_R = 2;                    % mass of the content in the reactor (kg)
m_J = 0.5;                  % mass of the heating medium in the jacket (kg)
m_T = 4;                    % mass of the heating medium in the thermostat (kg)
cp_T = 2;                   % heat capacity of the content of the jacket and thermostat (kJ / kg*K)
cp_R = 4;                   % heat capacity of the content of the reactor (kJ / kg*K)
m_dot_T = 0.02;             % in/outlet mass flow of the thermostat (kg / s)
A1 = 0.4;                   % heat transfer area between the jacket and environment (m^2)
A2 = 0.2;                   % heat transfer area between the jacket and reactor (m^2)
k1 = p(1);                  % heat transfer coefficient of jacket-environment (kW / m^2 * K)
k2 = p(2);                  % heat transfer coefficient of jacket-reactor (kW / m^2 * K)

% -------------------------------------------------------------------------

% Differential Equations

f(1,1) = ((-k2*A2)*(T_R - T_J)/(m_R*cp_R)); % T_R_dot
f(2,1) = (((T_Heat - T_J)*m_dot_T)/m_J) - ((k1*A1)*(T_J - T_E)/(m_J*cp_T)) - ...
    ((k2*A2)*(T_J - T_R)/(m_J*cp_T)); % T_J_dot
f(3,1) = (Q_Heat/(m_T*cp_T)) + (((T_J - T_Heat)*m_dot_T)/m_T); % T_Heat
     
end
