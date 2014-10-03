% Make a cobweb plot for a 1D difference equation map
% R0 parameter, 0 <= R <= 4
% x0 Initial condition
% N Number of iterations

% Amy Le
% May 29, 2014

function [xv, t, mymap] = cobweb(x0, iter, a, b,r)
global xlen
if length(x0) == 1
    % logistic map
    x = zeros(iter+1,1);
    myR = zeros(iter+1,1);
    x(1) = x0;
    
    for ic = 1:iter
        %     x(ic+1) = R0*x(ic)*(1-x(ic));
        myR(ic) = R(x(ic),a,b,r);
        x(ic + 1) = myR(ic) * x(ic) * (1 - x(ic));
    end
    xv = x(end-xlen+1:end);
    t = 0;
    mymap = 0;
else
    % the function at every place in [0,1]
    t = x0;
    myR2 = zeros(iter,1);
    for j = 1:length(t)
        myR2(j) = R(t(j),a,b,r);
    end
%     disp([t', x0', myR2, a, b])
    mymap = myR2'.*(t.*(1-t));
    xv = 0;    
end
end