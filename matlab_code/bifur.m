% compute the probability of a P1, P2, etc. orbit

function bifur(L,r)
global xlen N tol

% L = 0.1;   % max length
N = 10/L;   % max number of modes
xlen = 37;    % number of results to print

iter = 1000;    % number of x values in the cobweb
xrng = 20;    % number of initial conditions
% kmax = 1;    % number of different realizations
maxp = xlen-5;    % max number of orbit periods
st = linspace(0+tol,1-tol,xrng);    % vector of initial conditions
% y = zeros(maxp,2,kmax);


orbits = ones(maxp, maxp + 1)*-1;    % as many rows as periods and as many cols as x vals
u = 1;
[a,b] = myrand(L,N,r);
for j = 1:xrng
    [xv, ~, ~] = cobweb(st(j),iter, a, b,r);        % iterate thru init conds
    %         myresults(:,j,k) = xv;
    
    for w = 1:maxp    % check period of orbit
        if abs(xv(end-w) - xv(end)) < tol
            orbits(u,1) = w;    % w is period order
            orbits(u,2) = xv(end);        % xv has orbit location
            
            for v = 3:w+1    % get higher order period orbit locations
                orbits(u,v) = xv(end-v+2);
            end
            
            u = u + 1;    % row counter
            break
        end
    end
    
end

% get number of unique P1,P2... orbits for this map
u_orbits = uorbits(orbits);
[rows, ~] = size(u_orbits);


hold on
for i = 1:rows    
    myperiod = u_orbits(i,1);    
    switch myperiod
        case 1
            plot(r*ones(myperiod,1),u_orbits(i,2:myperiod+1),'b.')
        case 2
            plot(r*ones(myperiod,1),u_orbits(i,2:myperiod+1),'r.')
        case 3 
            plot(r*ones(myperiod,1),u_orbits(i,2:myperiod+1),'c.')
        case 4
            plot(r*ones(myperiod,1),u_orbits(i,2:myperiod+1),'m.')
        case 5
            plot(r*ones(myperiod,1),u_orbits(i,2:myperiod+1),'g.')
    end
    if myperiod > 5
            plot(r*ones(myperiod,1),u_orbits(i,2:myperiod+1),'k.')
    end
end        
text(.1,.9,['\fontsize{10}{\color{blue}1 \color{red}2 '...
'\color[rgb]{0 1 1}3 \color{magenta}4 \color{green} 5} >5'])
title('Bifurcation diagram')
xlabel('r')
ylabel('x')
end