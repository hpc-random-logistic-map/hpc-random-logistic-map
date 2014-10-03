% compute the probability of a P1, P2, etc. orbit

function prob_of_orbits(L, r, tmp)
global xlen N tol

N = 10/L;   % max number of modes
xlen = 57;    % number of results to print

iter = 1000;    % number of x values in the cobweb
xrng = 20;    % number of initial conditions
kmax = 1;    % number of different realizations
maxp = xlen-5;    % max number of orbit periods
st = linspace(0+tol,1-tol,xrng);    % vector of initial conditions
y = zeros(maxp,2);
[a,b] = myrand(L,N,r);    % make a new random draw for ea realization
orbits = ones(maxp, maxp + 1)*-1;    % as many rows as periods and as many cols as x vals
u = 1;

% iterate thru init conds
% find periodic orbits
for j = 1:xrng
    [xv, ~, ~] = cobweb(st(j),iter, a, b, r);        
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
num_periods = u_orbits(1:end,1);
for w = 1:maxp
    y(w,1) = w;
    y(w,2) = sum(num_periods == w);
end

p = zeros(maxp,2);
for i = 1:maxp
    p(i,1) = mean(y(i,2,:));
    p(i,2) = sum(y(i,2,:));
end
mytotal = sum(p(:,2));
probs = p(:,2)./mytotal;
labels = 1:maxp;

mydata = [labels' probs];
myname = strcat(strcat('C:\Users\amy\Documents\code\high res prob of orbit\prob_of_orbit_L',num2str(L)),num2str(tmp));
csvwrite(strcat(myname,'.csv'),mydata)
end

% plotting
% average number of orbits = p(:,1)
% total number of orbits = p(:,2)
% % probability of an orbit = # of orbits of order X / total orbits (of all orders)
%     plot(labels,probs,'--ks',...
%         'LineWidth',0.5,...
%         'MarkerSize',3,...
%         'MarkerEdgeColor','b',...
%         'MarkerFaceColor',[0.5,0.5,0.5])
%     xlabel('Period Order')
%     ylabel('Probability')
%     title('Probability of Period Orders')