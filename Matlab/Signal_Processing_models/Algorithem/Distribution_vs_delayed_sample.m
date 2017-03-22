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
%  <--- Old samples                                          New samples ----->
% [300 samples for the STD Dev] [99 samples of fifo] [1 sample for comparrison]
%
% This works "alright", still gives some false positives but its clearly
% better than the original ("Original_algorithem.m").

clear all
f = csvread('rawdata_noobject.txt')
j = csvread('rawdata_coffecup.txt')
k = csvread('rawdata_cofeecup_fewerdist.txt')
f = [(f(:,1));(j(:,1));(k(:,1));(f(:,1))];
[N  dc] = size(f)
d = 300 % samples in distribution
e = 100 %delay of compared sample

gs = std(f(1:d))
gm = mean(f(1:d))
g  = f(d+100)
res = 0;

[N  dc] = size(f);
for n = 1:1:N-400
    gs = [gs std(f(n:n+d))];
    gm = [gm mean(f(n:n+d))];
    g = [g (f(n+d+e)+f(n+d+e-1))/2];
    
    if(res(n) == 0)
        res = [res ((g(n) < gm(n)-3*gs(n)) | (g(n) > gm(n)+3*gs(n)))];
    else
        i = i + 1;
        if i > d
            if(gs(n) > 15)
                res = [res 1];
            else
                res = [res 0];
                i = 0;
            end
        else
            res = [res 1];
        end
    end
end

plot(gm+3*gs)
hold on
plot(gm-3*gs)
plot(g)
plot(res*50+mean(f))