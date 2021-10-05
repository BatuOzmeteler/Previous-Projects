function [t0, x0, u0] = move(T, t0, x0, u, f)
state = x0;
control = u(1);
f_value = f(state,control);
state = state + (T*f_value);

x0 = state;
t0 = t0 + T;
u0 = [u(2:size(u,1)); u(size(u,1))];
end