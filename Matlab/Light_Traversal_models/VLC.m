%Based on https://en.wikipedia.org/wiki/Phong_reflection_model

%
%   
%   I0 = Light intensity                  PD = Photodiode location
%   |                                     |
%   |                                     |    
%   |                                     |
%   |                                     |         
%   |Lamppost 1 with hight H              |Lamppost 2 with hight H            
%   |                                     |               
%   |                                     |         
%   |                                     |                
%   |                                     |    
%   |<---------------- D ---------------->|
%___|_____________________________________|
%//////////////////////////////////////////
%Ground with reflection factors "Groundreflection" (diffuse) and
%"Mirrorflection" (specular).
clear all
H = 3;
D = 5;
Groundreflection = 0.5;
Mirrorflection = 1 - 0.5;
a = 150;


Ks = @(x,y) (y>-0.1) .* (y<0.1) .* Mirrorflection;
Kd = @(x,y) ~Ks(x,y) .* Groundreflection;
halfconeapex = (120/2*pi)/180;
m = -1/log2(cos(halfconeapex));

%light emitted divided by distance squared? Distance seems to be missing in
%phong model
I = @(x,y) 1./(norm([x y H]))^2 .* (H./norm([x y H])).^m ;

N = [0 0 1];
PD = [-0.5*D 0 -H] / norm([-0.5*D 0 -H]); %aim of the PD normalized

L = @(x,y) [-x -y H]./norm([x y H]);
V = @(x,y) [D-x -y H]./norm([D-x y H]);

%L.N
LN = @(x,y) dot(L(x,y),N);

%N.V
NV = @(x,y) dot(N,V(x,y));

%R = 2(L.N)N-L
R = @(x,y) 2 .* (LN(x,y)) .* N - L(x,y);

%V.R
RV = @(x,y) dot(R(x,y),V(x,y));

%PD.V - This should give cos(thetha) where thetha is angle of irradiance. 
%Distance seems to be missing in phong model..
PDV = @(x,y) dot(PD,-V(x,y));

Phong = @(x,y) Kd(x,y) .* max(LN(x,y),0) .* max(NV(x,y),0) + Ks(x,y) .* max(RV(x,y).^a,0);
tot = @(x,y) I(x,y) .* Phong(x,y) ./ (norm([D-x -y H]).^2) .* PDV(x,y);

fmesh(tot,[0 5 -5 5]) %shows how much light is reflected to the photo diode from each surface
