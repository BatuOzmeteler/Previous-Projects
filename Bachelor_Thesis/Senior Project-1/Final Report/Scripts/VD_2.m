function [voo, rpmo] = emotor(rpmi,vi)
%#codegen
voo = vi;
rpmo = rpmi;
%% RPM & Speed limitation
if rpmi > 12000,
    rpmo = 12000;
    voo   = 3.6*vi;
elseif rpmi < 0, 
    rpmo = 0;
    voo   = 3.6*vi;
end 


end



