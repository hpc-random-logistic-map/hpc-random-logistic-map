% random function R(x) on [0,1]
% ln(R(x)) = xi(x)
% xi(x) = ln(r) + Sigma[ xi_n * exp(2pi * i * n * x) ]
% xi_n = a_n + i*b_n

% Amy Le
% May 29, 2014

function y = R(x,a,b,r)

global N

mysum = 0;
for j = 1:N  
%     fs = (a + b*1i)*exp( 2*pi*1i*j*x );
%     fs = (a + b*1i)*(cos(2*pi*j*x)+1i*sin(2*pi*j*x));
%     fs = 2*a*cos(2*pi*j*x);
    fs = 2*(a(j)*cos(2*pi*j*x) - b(j)*sin(2*pi*j*x));
    mysum = mysum + fs;
end

xi = log(r) + mysum;

y = exp(xi);
if y > 4 
%     y = 1/y + 3*rand;
    disp('more than 4')
%     y = 8- y;
    y = y * (1 + tanh(10*(4-y)));
%     y = mod(y,4);
end

end