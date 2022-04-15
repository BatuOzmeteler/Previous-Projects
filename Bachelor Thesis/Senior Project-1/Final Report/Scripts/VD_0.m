function dvdt = dvdtf(Trq,Grade,Fbrake,mss,v)
%#codegen

alpha = atan(Grade/100);

%% Vehicle Dynamics

Rtyre = 0.354;
g     = 9.81;
mu    = 0.009;
Cd    = 0.55;
A     = 5.10;
% mss = 2100;
ng    = 0.85;
Im    = 0.18;% g
Gb    = 18.5;

Ftyre = Gb*Trq/Rtyre;
Frub  = mu*mss*g;
Faero = 0.5*1.225*A*Cd*v*abs(v);
dvdt  = (1/(mss + Im*Gb^2/(ng*Rtyre^2)))*(Ftyre - Frub - Faero - mss*g*sin(alpha) - Fbrake);

end

