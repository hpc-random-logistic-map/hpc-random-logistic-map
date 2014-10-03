% noise stabilizes the function?

clear; clc; close all

tol = 10e-6;
n = 1; % fourier mode
L = 0.1;
r = 3.2;

sigma = (log(4/r) * tanh(L/4) / sqrt(1.5*tanh(L/2)));
alpha = sigma^2 * tanh(L/2);
Sn = alpha * exp(-L * abs(n));
Mn = sqrt(1.5*Sn);
p = tol/Mn;
perc = 100*p;
disp(strcat(num2str(perc),'%'))