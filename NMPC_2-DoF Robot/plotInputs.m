% Plot Inputs
function plotInputs(t, U, nu)

    figure();
    sgtitle('Inputs') 
    
    for i = 1:nu
        subplot(2,1,i)
        hold on;
        stairs(t(1:end-1), U(i,:), 'k', 'LineWidth', 1.5)
        xlabel('time (s)')
        switch i
            case 1
                ylabel('$\tau_{1}$ (rad/s)', 'Interpreter', 'latex')
            case 2
                ylabel('$\tau_{2}$ (rad/s)', 'Interpreter', 'latex')
        end
        grid ON;
        hold off;
    end  
    
end
