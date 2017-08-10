clear all
close all
%in fdatool, lowpass, Fir, 8th order, chebyshev,sidelobe atten = 100, 
%Fs = 125Hz, Fc = 5Hz
filter = [0.0049    0.0348    0.1126    0.2152    0.2650    0.2152    0.1126    0.0348    0.0049]

simulation_time = 3

ts = 1/200000;
t = 0:ts:simulation_time;

diff = 10;
T = 3;

[dc n] = size(t);

mu(1:n) = 10;
sigma(1:n) = 3;

noise  = normrnd(mu,sigma);
ac     = 2.5+5* cos(t*2*pi*50);
acL    = 1.25+2.5* cos(t*2*pi*200);
dc(1:n)= 200;
signal(1:n) = 0;
sin    = 4* sin(t*2*pi*1);
signal(ceil(1/ts):floor(1/ts+1/ts/2)) = sin(1:ceil(1/ts/2));
signal(floor(1/ts+1/ts/2)-25000:n) = 3;