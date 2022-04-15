%% Deal Problem Data 
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This function deals the data to separate
% variables for the problem creation.
%%
function [bounds, neighbours, n_neighbours, n_z, n_g, n_h, n_cc] = deal_data(data)

    bounds = data.bounds;

    neighbours = data.neighbours;
    n_neighbours = data.n_neighbours;

    n_z = data.n_z;
    n_g = data.n_g;
    n_h = data.n_h;
    n_cc = data.n_cc;
    
end
