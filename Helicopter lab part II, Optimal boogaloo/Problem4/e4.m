global N mx alpha beta lambda_t

%Problem 4
run('../InitFiles/init.m');

delta_t	= 0.25; % sampling time
N = 40; % Steps
x0 = [pi 0 0 0 0 0]'; % Initial state
%% Constraint params
alpha = 0.2;
beta = 20;
lambda_t = 2*pi/3;

elev_const_by_lambda = @(lambda_in) alpha * exp(-beta*(lambda_in-lambda_t)^2);
%% Input weights
%r1 = 1; % pitch
%r2 = 1; % elevation
%R = diag([r1 r2]); % r1, r2 = q1, q2 in assignment

%% Continuous model
Ac = [0   1   0   0   0   0;
       0   0 -K_2  0   0   0;
       0   0   0   1   0   0;
       0   0 -K_1*K_pp -K_1*K_pd 0 0;
       0   0   0   0   0   1;
       0   0   0   0 -K_3*K_ep -K_3*K_ed];
   
Bc = [0    0;
       0    0;
       0    0;
     K_1*K_pp 0;
       0    0;
       0  K_3*K_ep];

%% Number of states and inputs
mx = size(Ac,2); % Number of states (number of columns in A)
mu = size(Bc,2); % Number of inputs(number of columns in B)

%% Discretize
A1 = Ac*delta_t + eye(mx);
B1 = Bc*delta_t;

%% Initial values
x1_0 = pi;                              % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x5_0 = 0;                               % e
x6_0 = 0;                               % e_dot

x0 = [x1_0 x2_0 x3_0 x4_0 x5_0 x6_0]';  % Initial values

%% Time horizon and initialization
N  = 60;                                % Time horizon for states
M  = N;                                 % Time horizon for inputs
z  = zeros(N*mx+M*mu,1);                % Initialize z for the whole horizon
z0 = z;                                 % Initial value for optimization

%% Bounds
ul 	    = -30*pi/180;                   % Lower bound on control -- u1
uu 	    = 30*pi/180;                    % Upper bound on control -- u1

xl      = -Inf*ones(mx,1);              % Lower bound on states (no bound)
xu      = Inf*ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = ul;                           % Lower bound on state x3
xu(3)   = uu;                           % Upper bound on state x3

%% Generate constraints on measurements and inputs
[vlb,vub]       = genbegr2(N,M,xl,xu,ul,uu); % hint: genbegr2
vlb(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                    % We want the last input to be zero

%% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = zeros(mx,mx);
Q1(1,1) = 1;                             % Weight on state x1
Q1(2,2) = 0;                            % Weight on state x2
Q1(3,3) = 0;                             % Weight on state x3
Q1(4,4) = 0;                            % Weight on state x4
P1 = zeros(mu,mu);
P1(1,1) = 1;                                 % Weight on input u1
P1(2,2) = 1;                                  % Weight on input u2
Q = 2*genq2(Q1,P1,N,M,mu);              % Generate Q
%c = zeros(N*mx+M*mu,1);                 % Generate c

%% Generate system matrixes for linear model
Aeq = gena2(A1,B1,N,mx,mu);           % Generate A, hint: gena2
beq = zeros(size(Aeq,1),1);        	  % Generate b
beq(1:mx) = A1*x0; % Initial value

%% Objective function
f = @(z) z' * Q * z;

%% Target state
target_e = alpha*exp(-beta*(0-lambda_t)^2);
target_x = [0 0 0 0 target_e+0.1 0]';
target_u = [0 target_e+0.1]';

%% SQP solvin' for optimal control
z0 = [x0;repmat(target_x, N-1, 1);repmat(target_u, N, 1)];
z = fmincon(f, z0, [], [], Aeq, beq, vlb, vub, @confun, optimset('Algorithm','sqp'));

%% LQR Calculation for feedback control
Q_LQR = diag([20 1 1 1 30 10]);
R_LQR = diag([1 1]);
[K,S,e] = dlqr(A1,B1,Q_LQR,R_LQR);

%% Extract control inputs and states

x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);z(5:mx:N*mx)];              % State x5 from solution
x6 = [x0(6);z(6:mx:N*mx)];              % State x6 from solution

u1  = [z(N*mx+1:mu:N*mx+M*mu);z(N*mx+M*mu)]; % Control input from solution
u2  = [z(N*mx+2:mu:N*mx+M*mu);z(N*mx+M*mu)]; % Control input from solution

num_variables = 5/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

u1   = [zero_padding; u1; zero_padding];
u2   = [zero_padding; u2; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];

%% Plotting
t = 0:delta_t:delta_t*(length(u1)-1);

figure(2)
subplot(811)
stairs(t,u1),grid
ylabel('u1')
subplot(812)
stairs(t,u2),grid
ylabel('u2')
subplot(813)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')
subplot(814)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')
subplot(815)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')
subplot(816)
plot(t,x4,'m',t,x4','mo'),grid
ylabel('pdot')
subplot(817)
plot(t,x5,'m',t,x5','mo',t,arrayfun(elev_const_by_lambda,x1)),grid
%plot(t,x5,'m',t,x5','mo'),grid
ylabel('e')
subplot(818)
plot(t,x6,'m',t,x6','mo'),grid
xlabel('tid (s)'),ylabel('edot')
%%Constraint plot

figure(3)
plot(x1,x5,'-o',(-0.5:0.01:3.5),arrayfun(elev_const_by_lambda,(-0.5:0.01:3.5)),'m--', ...
                x1,arrayfun(elev_const_by_lambda,x1),'r-x')
title('Optimal path')
legend('Calculated optimal path','Constraint', 'Constraint (evaluation points)')
xlabel('\lambda [rad]')
ylabel('e [rad]')


%%Simulink stuff
inputVec= [t' u1 u2];
xVec = [t' x1 x2 x3 x4 x5 x6];
