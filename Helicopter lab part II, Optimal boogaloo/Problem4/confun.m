function [c, ceq] = confun(z)
%global N mx alpha beta lambda_t %globals not working. Ask studass maybe???

N = 40; % Steps
mx = 6;
% Constraint params
alpha = 0.2;
beta = 20;
lambda_t = 2*pi/3;

c = zeros(N,1);
for k=1:N
    lambda = z(1+(k-1)*mx);
    elev = z(5+(k-1)*mx);
    c(k) = alpha * exp(-beta*(lambda-lambda_t)^2) - elev;
end
%c = [1 2 3];
ceq = [];