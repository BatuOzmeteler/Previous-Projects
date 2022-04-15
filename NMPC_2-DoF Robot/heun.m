% Heun Integrator
function xf = heun(ode, h, x, u)

    xt = x + h*ode(x, u);
    xf = x + h/2*(ode(x, u) + ode(xt, u));
    
end
