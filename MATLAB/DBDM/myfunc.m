function f = myfunc(p)

load my_data.mat
x0 = [293; 293; 293]; % all temperatures are 20 C^o if {Q^{dot}}_{heat} = 0

tspan = data_set_step_timestamp(1:950);

options = odeset('RelTol', 1e-8, 'AbsTol', 1e-6);
[time, h] = ode45(@(t, x)cstr_model_ode_rhs(t, x, p), tspan, x0, options);

f = 0;
    for i = 1:length(time)
        f = f + (h(i, 1) - (smooth_data(i) + 273))^2;
    end

end
