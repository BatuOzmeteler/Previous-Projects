%% Deal Elements 
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function deals the elements to separate
% variables for the problem creation.
%%
function [z, H_bar, f, g, h, AA, llbx, uubx, zz0, lam0] = deal_elem(elem)

    z = elem.z;
    
    H_bar = elem.H_bar;
    
    f = elem.f;
    g = elem.g;
    h = elem.h;
    AA = elem.AA;
    
    llbx = elem.llbx;
    uubx = elem.uubx;
    
    zz0 = elem.zz0;
    lam0 = elem.lam0;
    
end
