

G = parallel( tf(1,[1 0]), tf(1, [1 2]) );
H = series( tf(1,[1 1]), tf(1,[1 10]) );

T = feedback(G, H)

