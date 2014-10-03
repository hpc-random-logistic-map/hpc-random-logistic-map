function [ w ] = uorbits( orbits )
% get unique orbits from an array of all orbit orders + 
% locations for 1 map realization
global tol

orbits = sort(orbits,2,'descend');
u_orbits = unique(orbits,'rows');
[row, col] = size(u_orbits);
for i = 2:row
    if u_orbits(i,1) == u_orbits(i-1,1)
        if norm(u_orbits(i,:) - u_orbits(i-1,:)) <= tol
            u_orbits(i-1,:) = ones(1,col)*-1;            
        end
    end    
end
w = unique(u_orbits,'rows');
if w(1,1) == -1
    w = w(2:end,:);
end
end