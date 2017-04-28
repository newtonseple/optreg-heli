%init; % NB: Change this to the init file corresponding to your helicopter
delta_t	= 0.25; % sampling time

A = [0 1         0         0; 
     0 0        -K_2       0; 
     0 0         0         1; 
     0 0 -K_1*K_pp -K_1*K_pd];
  
A1 = A*delta_t + eye(4);

B = [0 0 0 K_1*K_pp]';

B1 = B*delta_t;

Q = diag([50 1 10 10]);
R = diag(1);



[K,S,e] = dlqr(A1, B1,Q,R);