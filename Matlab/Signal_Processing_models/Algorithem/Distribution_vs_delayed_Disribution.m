% This file shows an attempt to make sense of data sampled by the system
% The data was gatherd as follows:
%   - rawdata_noobject.txt contains data with no object in its los
%   - rawdata_coffecup.txt contains data with a coffeecup at the edge of
%     the los standing on the floor
%   - rawdata_cofeecup_fewerdist contains data with a coffeecup at the edne
%     of the los but much closer to the light and photo diode
%
% Main method tried in this file: Adding a delay between the standard
% deviation calculation and the sample to compare it with. Data is
% distributed as follows:
%
%  <--- Old samples                                                 New samples ----->
% [ 200 samples for the STD Dev] [200 - x samples of fifo] [x samples for comparrison]
%
% This works very well with x beeing 3 and comparing it with mean +- 2
% sigma.


clear all
f = csvread('rawdata_noobject.txt')
j = csvread('rawdata_coffecup.txt')
k = csvread('rawdata_cofeecup_fewerdist.txt')
f = [(f(:,1));(j(:,1));(f(:,1));(k(:,1));(f(:,1))];

delay = 90
d = 300 %samples in main std
e = 10  %samples in deleayed distribution

stdAtDetect = 0;

gs = std(f(1:d));
gm = mean(f(1:d))

hs = std(f(1:d))
hm = mean(f(1:d))
ga = std(f(1:d));
res = 0;
[N  dc] = size(f)
for n = 2:1:N-501
    ga = [ga std(f(n:n+d))];
    hm = [hm mean(f(n+d+delay:n+d+e+delay))];
    gs = [gs std(f(n:n+d))];
    gm = [gm mean(f(n:n+d))];
    
    if res(n-1) == 0
        stdAtDetect = gs(n);
        i = 0;
        res = [res ((hm(n) < gm(n)- 4*gs(n)/sqrt(e)) | (hm(n) > gm(n)+4*gs(n)/sqrt(e)))];
    else
        i = i + 1;
        if i > d+delay
            if stdAtDetect > gs(n)
                res = [res 0];
            else
                res = [res 1];
            end
        else
            res = [res 1];
        end
    end
end

plot(gm+3*gs)
hold on
plot(gm+4*gs(n)/sqrt(e))
plot(gm-3*gs)
plot(gm-4*gs(n)/sqrt(e))
plot(hm)
plot(mean(f)+res*50)
hold on