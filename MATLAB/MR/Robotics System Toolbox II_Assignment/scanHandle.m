classdef scanHandle < handle
    properties
        ranges = 0; %array of range readings in [m]
        angles = 0; %array of angles in [rad]
        rmin = 0;    %minimum range reading
        phimin = 0;  %angle of minimum range reading
    end
end

