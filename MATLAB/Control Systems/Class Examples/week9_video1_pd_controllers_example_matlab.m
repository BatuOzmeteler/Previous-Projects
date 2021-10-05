

G = tf(1, [1 10 24 0]);
H = 1;

T = feedback(G, H);

rlocus(G*H);
sgrid(0.504, []);


% reminder: stability of the system depending on K
%T = feedback(447*G, H);
%T = feedback(205*G, H);


% from the root-locus, we can see that K = 44 gives zeta=0.504, wn=2.40
%
% new_wn = 3*wn = 3*2.40 = 7.2



rlocus(G*H);
sgrid(0.504, 7.2);

% we designed a controller on paper in the previous video.
%
% G_pd = K ( s + 2.98 )

Gpd = tf(50*[1 2.98],1);
Gc = series(G, Gpd)

Tc = feedback( Gc , H);

rlocus(Gc*H);
sgrid(0.504, 7.2);



T = feedback(44*G, H);
Tc = feedback(50*Gc , H);


subplot(2,1,1);step(T);
subplot(2,1,2);step(Tc);


% Is the 2nd order assumption valid?
pzmap(T); % valid for T
pzmap(Tc); % valid for Tc






