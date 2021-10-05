% % Q2 - 3)  Calculate the equilibrium points of the system for the given
% % steady-state inputs. Fr_ss = 0.002365 [m3 / min], Qj_ss = 18.5583 [kJ / min]
% 
% Fr_ss = 0.002365; Qj_ss = 18.5583;
% Tr_ss = sym('Tr_ss'); Tj_ss = sym('Tj_ss'); C_A_ss = sym('C_A_ss'); C_B_ss = sym('C_B_ss'); 
% 
% C_A_dot_ss = ((Fr_ss / V) * (C_A_in - C_A_ss)) - (k_01*exp(-E_R1 / Tr_ss)*C_A_ss) == 0;
% C_A_ss_sol = solve(C_A_dot_ss, C_A_ss);
% 
% C_B_dot_ss = (-(Fr_ss / V) * C_B_ss) + (k_01*exp(-E_R1 / Tr_ss)*C_A_ss) - (k_02*exp(-E_R2 / Tr_ss)*C_B_ss) == 0;
% C_B_ss_sol = solve(C_B_dot_ss, C_B_ss);
% 
% T_R_dot_ss = ((Fr_ss / V)*(T_in - Tr_ss)) - ((k_01*exp(-E_R1 / Tr_ss)*C_A_ss*deltaH_R1) / (rho*cp)) - ...
%     ((k_02*exp(-E_R2 / Tr_ss)*C_B_ss*deltaH_R2) / (rho*cp)) - (((kA*(Tr_ss - Tj_ss)) - Qj_ss) / (rho*cp*V)) == 0;
% %T_R_ss_sol = solve(T_R_dot_ss, Tr_ss);
% 
% T_J_dot_ss = ((kA*(Tr_ss - Tj_ss)) - Qj_ss)/(m_j*cp_j) == 0;
% T_J_ss_sol = solve(T_J_dot_ss, Tj_ss);
% 
% % Substitutions
% subs_C_B_dot_ss = subs(C_B_ss_sol, C_A_ss, C_A_ss_sol);
% subs_T_R_dot_ss = subs(T_R_dot_ss, [C_A_ss, C_B_ss, Tj_ss], [C_A_ss_sol, subs_C_B_dot_ss, T_J_ss_sol]);
% 
% % Equilibrium Points
% T_R_steady = solve(subs_T_R_dot_ss, Tr_ss); 
% C_A_steady = subs(C_A_ss_sol, Tr_ss, T_R_steady); 
% C_B_steady = subs(subs_C_B_dot_ss, Tr_ss, T_R_steady); 
% T_J_steady = subs(T_J_ss_sol, Tr_ss, T_R_steady); 

%-------------------------------PLOT OF INPUT ENERGY VS REMOVED ENERGY-------------------------------------%
% input = ((Fr_ss / V)*(T_in));
% output = ((k_01*exp(-E_R1 / Tr_ss)*C_A_ss*deltaH_R1) / (rho*cp)) + ((Fr_ss / V)*Tr_ss) + ...
%     ((k_02*exp(-E_R2 / Tr_ss)*C_B_ss*deltaH_R2) / (rho*cp)) + (((kA*(Tr_ss - Tj_ss)) - Qj_ss) / (rho*cp*V));
% 
% subst = subs(output, [C_A_ss, C_B_ss, Tj_ss], [C_A_ss_sol, subs_C_B_dot_ss, T_J_ss_sol]);
% 
% Temperature = 350:0.1:400;
% n = length(Temperature);
% y_out = zeros(1,n);
% y_in = zeros(1,n);
% for i = 1:n
%     y_out(i) = subs(subst, Tr_ss, Temperature(i));
%     y_in(i)  = input;
% end
% 
% figure(1)
% plot(Temperature, y_out, 'r', Temperature, y_in, 'b-');
% grid on;
% legend('Input Energy','Removed Energy');
% xlabel('Temperature')

%------CALCULATION OF EQUILIBRIUM POINTS WITH NEWTON'S METHOD------%

% x = 2; % initial guess x0
% p = ((Fr_ss / V) * (C_A_in - C_A_ss)) - (k_01*exp(-E_R1 / Tr_ss)*C_A_ss);
% f = subs(p, [C_A_ss, Tr_ss], [x, T_R_steady]); %f(x0)
% i = 0; % number of iterations
% while abs(f)>10^(-4)
%     df = (-Fr_ss/V) - (k_01*exp(-E_R1/T_R_steady)*V); % the derivative at xi df/dx|xi
%     x = x-f/df; %The next iteration xi+1
%     f = subs(p, [C_A_ss, Tr_ss], [x, T_R_steady]); %f(xi+1)
%     i=i+1;% increase the iteration number by 1
% end
% x
% f
% i
%-------------------------------------------------------------------%