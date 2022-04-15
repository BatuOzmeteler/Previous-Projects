%% Create Formation
% Authors: 
% - Mehmet Batu Özmeteler
%% Summary
% This script creates a formation for a group of quadrotos.
%%
function out = create_formation(point, d, N_agents, agent_id)

    out = point;
    d = d/2;
    
    if N_agents == 2
        switch agent_id
            case 1
                out(1) = out(1) - d;
            case 2
                out(1) = out(1) + d;
        end
    else 
        switch agent_id
            case 1
                out(1) = out(1) - d;
                out(3) = out(3) - d;
            case 2
                out(1) = out(1) + d;
                out(3) = out(3) - d;
            case 3
                out(1) = out(1) + d;
                out(3) = out(3) + d;
            case 4
                out(1) = out(1) - d;
                out(3) = out(3) + d;
        end
    end
    
end
