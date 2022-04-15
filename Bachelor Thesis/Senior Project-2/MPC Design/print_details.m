function print_details(N, T, xx, t)

xx_ss = xx(end);
xx1 = 0.1*xx_ss;
xx2 = 0.9*xx_ss;

t1 = 0;
t2 = 0;
for i = 1:length(xx)

    if ((xx(i) > xx1) && (t1 == 0))
        t1 = t(i);
    end
    
    if ((xx(i) > xx2) && (t2 == 0))
        t2 = t(i);
    end
end
rise_time = t2 - t1;

[xx_max, ind] = max(xx);
t(length(t)+1) = t(end) + T;
peak_time = t(ind);

percent_overshoot = 100*((xx_max - xx_ss) / xx_ss);

for i = length(xx):-1:1
    if abs(xx(i) - xx_ss) > 0.02*xx_ss
        settling_time = t(i);
        break
    end
end

is_T_ok = (rise_time/20) < N && N < (rise_time/10);
is_N_ok = N*T > settling_time; 

if is_T_ok == 1 print_T = 'yes';, else print_T = 'no';, end
if is_N_ok == 1 print_N = 'yes';, else print_N = 'no';, end
    
f = msgbox({sprintf('Rise Time: %.2f s - Peak Time: %.2f s', rise_time, peak_time);...
    sprintf('Percent Overshoot: %.2f percent - Settling Time: %.2f s', percent_overshoot, settling_time);...
    sprintf('Prediction Horizon: %u  - Is it OK?: %s', N, print_N);...
    sprintf('Sample Time: %.2f s - Is it OK?: %s', T, print_T)},'Details');
