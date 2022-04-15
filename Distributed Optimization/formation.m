
function out = formation(x_goal, d, num_of_robots, agent_id)

    out = x_goal;
    d = d/2;
    
    if num_of_robots == 2
        switch agent_id
            case 0
                out(1) = out(1) - d;
            case 1
                out(1) = out(1) + d;
        end
    else 
        switch agent_id
            case 0
                out(1) = out(1) - d;
                out(3) = out(3) - d;
            case 1
                out(1) = out(1) + d;
                out(3) = out(3) - d;
            case 2
                out(1) = out(1) + d;
                out(3) = out(3) + d;
            case 3
                out(1) = out(1) - d;
                out(3) = out(3) + d;
        end
    end
end