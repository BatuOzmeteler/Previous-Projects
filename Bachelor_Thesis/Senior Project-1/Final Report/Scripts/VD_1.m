function mot_rpm  = emotor(v)
%#codegen

%% e-motor
Rtyre   = 0.354;
Gb      = 18.5;
mot_rpm = (v/Rtyre)*(60/(2*pi))*Gb;
end

