% compute the relative size of a basin of attraction about an orbit

function basin_size(r)

global xlen N L tol

L = 0.1;   % max length
N = 100;   % max number of modes
% r = 3.2;     % expectation of xi, E[xi] = ln(r)
xlen = 25;    % number of results to print

iter = 1000;    % number of x values in the cobweb
xrng = 20;    % number of initial conditions
kmax = 1;    % number of different realizations
maxp = 20;    % max number of orbit periods
st = linspace(0+tol,1-tol,xrng);    % vector of initial conditions
myresults = zeros(xlen,xrng,kmax);

for k = 1:kmax
    [a,b] = myrand(N,r);    % make a new random draw for ea realization
    orbits = ones(xrng, xrng + 1)*-1;
    u = 1;
    
    for j = 1:xrng                        
        [xv, ~,~] = cobweb(st(j),iter, a, b, r);        % iterate thru init conds
        myresults(:,j,k) = xv;
%         disp(strcat('cob',num2str(j)))
        for w = 1:maxp    % check period of orbit
%             disp(strcat('checkp',num2str(w)))                        
            if abs(xv(end-w) - xv(end)) < tol
                orbits(u,1) = st(j);    %initial condition that sent it to this orbit
                orbits(u,2) = w;    % w is period order
                orbits(u,3) = xv(end);        % xv has orbit location
                
                for v = 4:w+2    % get higher order period orbit locations
                    orbits(u,v) = xv(end-v+3);
                end
                
                u = u + 1;    % row counter
                break                
            end            
        end        
    end
    
    % count up the number of initial coniditions that ended up in each unique orbit
    mybasins = bsize(orbits,st);        
    mydata = [mybasins(:,2) mybasins(:,1)];
    myname = strcat(strcat('basin_size_',num2str(kmax)),num2str(r));
    csvwrite(strcat(myname, '.csv'),mydata)
end
end

% plot(mybasins(:,2),mybasins(:,1),'rs','MarkerSize',10,...
%     'MarkerEdgeColor','r',...
%     'MarkerFaceColor',[1,0,1])
% xlabel('period order')
% ylabel('percentage of initial conditions drawn to orbit')
% title('basin of attraction size')
% axis([0 max(mybasins(:,2)) 0 1])
% set(gca,'XTick',[0:max(mybasins(:,2))])