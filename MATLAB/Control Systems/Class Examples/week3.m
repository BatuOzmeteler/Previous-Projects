% 
% %[r,p,k] = residue(1, conv([1 0 0], [1 2 1]));
% 
% 
% H = tf(16, [1 4 16]);
% 
% impulse(H)
% step(H)
% 
% % ramp response
% step( tf(16, [1 4 16 0]) )
% 
% 
% H = tf([1 0 0], [2 2 1]);
% 
% impulse(H)
% roots([2 2 1])
% step(H)
% step( tf([1 0 0], [2 2 1 0]) ); % ramp response 
% 
% impulse ( tf([1 0], [1 0 1/64]) * tf([1 0 0], [2 2 1]) );


H = tf(1, [1 0 1]);
impulse(H);
step(H)
