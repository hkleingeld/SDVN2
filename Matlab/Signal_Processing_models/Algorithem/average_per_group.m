% This file shows an attempt to make sense of data sampled by the system
% The data was gatherd as follows:
%   - rawdata_noobject.txt contains data with no object in its los
%   - rawdata_coffecup.txt contains data with a coffeecup at the edge of
%     the los standing on the floor
%   - rawdata_cofeecup_fewerdist contains data with a coffeecup at the edne
%     of the los but much closer to the light and photo diode
%
% Main method tried in this file: Trying to find a relation between average
% and standard deviation

clear all
f = csvread('rawdata_noobject.txt')
j = csvread('rawdata_coffecup.txt')
k = csvread('rawdata_cofeecup_fewerdist.txt')
f = [(f(:,1));(j(:,1));(k(:,1));f(:,1)] 


a = 5 % samples in averager's
b = 100

am = mean(f(1:a))
bm = mean(f(1:b))
stdbm = std(1:b) + 660;

[N  dc] = size(f)
for n = b:1:N/a
    am = [am mean(f((n-1)*a+1:a*n+1))];
    bm = [bm mean(f(n*a+1-b:a*n+1))];
    stdbm = [stdbm std(f(n*a+1-b:a*n+1))];
end

plot(am)
hold on
plot(bm)
plot(stdbm+600)


