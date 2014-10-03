% bound the map at each position 

function envelope(r)

global xlen N L 

L = 0.1;   % max length
N = 100;   % max number of modes
% r = 3.2;     % expectation of xi, E[xi] = ln(r)
xlen = 25;    % number of results to print

iter = 1000;    % number of x locations
kmax = 1000;    % number of different realizations
st = linspace(0,1,iter);    % vector of initial conditions

myextrema = zeros(length(st),3);
myextrema(:,1) = st';
myextrema(:,2) = ones(length(st),1);

allmydata = zeros(kmax,iter);

for k = 1:kmax
    [a,b] = myrand(N,r);    % make a new random draw for ea realization
    [~, ~, mymap] = cobweb(st,iter, a, b, r);       
    
    allmydata(k,:) = mymap;
%     if k == 50
%         plot(st,mymap,'r')
%         hold on
%     elseif k == 75
%         plot(st,mymap,'r')        
%     end
    
end

myextrema(:,2) = min(allmydata)';
myextrema(:,3) = max(allmydata)';

myname = strcat(strcat('envelope_',num2str(kmax)),num2str(r));
csvwrite(strcat(myname, '.csv'),myextrema)
% plot(myextrema(:,1),myextrema(:,2),'b',myextrema(:,1),myextrema(:,3),'k')
% xlabel('x_n')
% ylabel('x_{n+1}')
% title('Upper and lower bounds on the random logistic map')
% myname = strcat(strcat('envelope_k',num2str(k)),num2str(iter));
% savefig(strcat(myname,'.fig'));
end