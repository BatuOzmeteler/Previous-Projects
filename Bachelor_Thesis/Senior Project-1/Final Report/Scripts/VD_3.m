function vv  = emotorinv(rpm)
%#codegen
Rtyre = 0.354;
Gb    = 18.5;
vv    = (rpm*Rtyre)*(2*pi/60)*(1/Gb);
end