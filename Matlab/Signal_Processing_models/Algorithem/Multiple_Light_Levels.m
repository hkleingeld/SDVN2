%Attempt at creating an algorithem working with 2 light levels to abuse the
%following relation:
%
% PD1 = x*L1 + y*E + noise
% PD2 = x*L2 + y*E + noise
% PD1-PD2 = x*(L1-L2) + ?noise?
%
% Where PDx = Light received by the PD
% x = Some factor describing the enviorment from pov from the light source
% y = some factor describing the enviorment from pov from the enviourment light source
% Lx = Intencity of light source x
% E = Sum of enviourmental light sources
% noise = noise on measurments (not enviourment light)
%
% The idea was that this could illiminate the enviourment influences, and
% that it might influence the noise in a posetive way.
%
% Spoiler. It made the noise worse and was therfore abandonned. 

clear all
f = csvread('rawdata_switcher.txt') %data with two different light levels.
[N dc] = size(f);

high  = f(2:2:N); %samples with high intensity
low = f(1:2:N); %samples with low intensity
[dc N] = size(low);
data = high - low;

plot(data)