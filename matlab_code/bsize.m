function [ w ] = bsize( orbits, st )
%returns an array with percent of all initial conditions that end up in a
%unique orbit

n = length(st);    %number of initial conditions
tol = 10e-6;

% sort the orbits array by column (period order), but preserve the order in the rows.
[~, ind2] = sort(orbits(:,2));
osort = orbits(ind2,:);
i = 1;
[maxr,~] = size(osort);

while i <= maxr
    [row,~] = size(osort);
    if i <= row && osort(i,1) ~= -1
        osort = osort(i:end,:);
        break
    end
    i = i + 1;
end

if i <= maxr
    % sort ea row st period locations are in desc order
    [row,col] = size(osort);
    for i = 1:row
        osort(i,3:end) = sort(osort(i,3:end),'descend');
    end
    
    %proportion of init conds that found an orbit
    o_counter = zeros(row,col);
    
    for i = 1:row
%         disp(strcat('bsize',num2str(i)))
        tmp_o = osort(i,2:end);
        
        if osort(i,2) > 0    % check if i've already looked at this orbit
            o_counter(i,:) = [1 tmp_o];
        else
            continue
        end
        
        for k = 1:row
            if k == i    % skip?
                
            elseif tmp_o(1) == osort(k,2)    % this is a new orbit that has the same period
                if norm(osort(k,3:end) - tmp_o(2:end)) <= tol
                    o_counter(i,1) = o_counter(i,1) + 1;
                    osort(k,2) = osort(k,2)*-1;    % mark it as seen by using neg sign
                end
            elseif tmp_o(1) > osort(k,2)
                continue
            else
                break
            end
        end
    end
    
    o_counter = unique(o_counter,'rows');
    lost_o = (n-sum(o_counter(:,1)))/n;
    [row, col] = size(o_counter);
    for k = 1:row
        o_counter(k,1) = o_counter(k,1)/n;
    end
    
    if o_counter(1,2) == 0
        o_counter(1,1) = lost_o;
        w = o_counter;
    else
        w = zeros(row+1,col);
        w(1:row,:) = o_counter;
        w(end,1) = lost_o;
    end
else
    w = [0 0];
end

end