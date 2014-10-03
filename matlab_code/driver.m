clear
clc
close all

global xlen N L r 

L = 0.1;   % max length
N = 100;   % max number of modes
r = 3.2;     % expectation of xi, E[xi] = ln(r)
xlen = 25;    % number of results to print

iter = 1000;    % number of x values in the cobweb
xrng = 20;    % number of initial conditions
kmax = 500;    % number of different realizations
tol = 10e-6;
maxp = 20;    % max number of orbit periods
st = linspace(0+tol,1-tol,xrng);    % vector of initial conditions
myresults = zeros(xlen,xrng,kmax);
% myrand = zeros(N,2,xrng);
orbits = ones(xrng*kmax, xrng + 1)*-1;
flag = 0;    % plot maps on top of each other
u = 1;
hc = figure;

for k = 1:kmax
    [a,b] = myrand(N);    % make a new random draw for ea realization
    
    for j = 1:xrng                        
        [xv, t, mymap] = cobweb(st(j),iter, a, b);        % iterate thru init conds
        myresults(:,j,k) = xv;
        
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
    
    % plot maps on top of each other
    if flag ~= 0
        if mod(k,flag) == 0    % plot every other map
            movegui(hc, 'northwest')
            plot(t,mymap,'k')
            hold on
            axis('square'); axis([0 1 0 1]);
            set(gca,'XTick',(0:0.1:1),'YTick',(0:0.1:1))            
            xlabel('x_n')
            ylabel('x_{n+1}')
            title('Period 1: P(x)')
        end
    end
end

% plot number of orbits that come from a period
[u_orbits, ia, ic] = unique(orbits,'rows');
u_orbits = u_orbits(2:end,:);
num_periods = u_orbits(1:end,1);
y = zeros(maxp,2);
for w = 1:maxp
    y(w,1) = w;
    y(w,2) = sum(num_periods == w);
end
plot(y(:,1),y(:,2),'o')
xlabel('Period')
ylabel('Number of Orbits')
title('Number of Orbits as a Function of Period')

% % cobweb plots
% line([x(1) x(1)],[0 x(2)],'Color','g')
% plot(x(1), x(1),'k*');
% hold on
% for ic = iter-50:iter-1
%     line([x(ic) x(ic+1)],[x(ic+1) x(ic+1)],'Color','g')
%     line([x(ic+1) x(ic+1)],[x(ic+1) x(ic+2)],'Color','g')
%     plot(x(ic+1), x(ic+1),'k*');
% end
% line([x(iter) x(iter+1)],[x(iter+1) x(iter+1)],'Color','g')
%     fplot('1*y',[0 1],'r');
% xlabel('x_n')
% ylabel('x_{n+1}')
% title('Period 1: P(x)')


% % histogram of periods
% figure
% hist(orbits(:,1),1:maxp)
% h = findobj(gca,'Type','patch');
% set(h,'FaceColor',[0 .5 .5],'EdgeColor','w')
% xlabel('periods')
% ylabel('frequency')

% % histogram of orbit locations
% figure
% hist(orbits(:,2),st)
% h2 = findobj(gca,'Type','patch');
% set(h2,'FaceColor',[.5 .5 0],'EdgeColor','w')
% xlabel('orbit locations')
% ylabel('frequency')

% % higher period maps
% [yvs,ind] = sort(yv);
% yv2 = cobwebp2( yvs, xv, av, bv, iter );
% [yv2s,ind] = sort(yv2);
% cobwebp3( yv2s, xv, av, bv, iter )