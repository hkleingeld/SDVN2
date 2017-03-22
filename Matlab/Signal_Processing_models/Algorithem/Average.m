% This file shows an attempt to make sense of data sampled by the system
% The data was gatherd as follows:
%   - rawdata_noobject.txt contains data with no object in its los
%   - rawdata_coffecup.txt contains data with a coffeecup at the edge of
%     the los standing on the floor
%   - rawdata_cofeecup_fewerdist contains data with a coffeecup at the edne
%     of the los but much closer to the light and photo diode
%
% Main method tried in this file: Averaging.


clear all

f = csvread('rawdata_noobject.txt')
j = csvread('rawdata_coffecup.txt')
k = csvread('rawdata_cofeecup_fewerdist.txt')
f = [(f(:,1));(j(:,1));(k(:,1))] 


a = 10 % samples in averager's
b = 20
c = 40
d = 80
e = 160

am = mean(f(e-a:e))
bm = mean(f(e-b:e))
cm = mean(f(e-c:e))
dm = mean(f(e-d:e))
em = mean(f(1:e))

[N  dc] = size(f)
for n = 1:1:N-e
    am = [am mean(f(e-a+n:e+n))];
    bm = [bm mean(f(e-b+n:e+n))];
    cm = [cm mean(f(e-c+n:e+n))];
    dm = [dm mean(f(e-d+n:e+n))];
    em = [em mean(f(e-e+n:e+n))];
end

plot(am)
hold on
plot(bm)
plot(cm)
plot(dm)
plot(em)