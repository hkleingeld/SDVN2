clear all
% Assumes wave lenght the emitted light to be equal.
% Based on following LED: http://lamptest.ru/images/graph/ikea-70288022-led1336r4.png
% which is the acutal LED in testing setup

%
%   
%   I0.. (dotted line is @ShaddowAngleX
%   |   ...
%   |      ..
%   |        ..
%   |          ..
%   |            ...
%  H|               ..
%   |          _______..
%   |         |        |..
%   |       Zl| Object |  ..
%   |         |        |    ..XShaddow
%___|_________|________|______|______________
%   <- xLoc -> <- Xl ->           x-axile ->

%3D Phythagoras
Norm = @(x,y,z) sqrt(x.*x+y.*y+z.*z); %lenght of 3D vector
Dist2 = @(x,y,z) (x.*x+y.*y+z.*z); %lenght of 3D vector ^2 (for optimization purposes)

FOV = 120/180*pi; %120 degrees in radians
rec = @(x,y,z) (acos(z./Norm(x,y,z)))./(FOV) <= 1;
PDA = 7.2 * 10^-6; %7.2mm^2 or 2,65x2,65 

Rd = 1; %fraction of light that is reflected diffusely
mFloor = 1;%shininess of the floor (1 is diffuse higher is more mirror like)
H = 0.5 %hight of the light
lumI = 264; %total amount of lumen emitted by the light

%Light source description%
halfconeapex = (35*pi)/180; %Halfpower angle
m = -1/log2(cos(halfconeapex)); %some factor describing the light source

%omschrijving licht bron
%I(phi) = @(x,y) I0 .* cos(acos(H./Norm(x,y,H))).^2;
I  = @(x,y,z) lumI .* ((m+1)/(2*pi)) .* (z./Norm(x,y,z)).^m;
I_= @(x,y) I(x,y,1);

%Ehor = @(x,y) (m+1) .*I(x,y) .* cos(acos(H./Norm(x,y,H))) ./ Norm(x,y,H).*Norm(x,y,H)
%             lichtbron           Invalshoek            Afstand^2
L1 = @(x,y,z) I(x,y,z) .*(z ./ Norm(x,y,z)) ./ (Dist2(x,y,z));
L1_= @(x,y) L1(x,y,H);

%Lambertian reflection
Lamb = @(x,y,z) 1/pi * (z ./ Norm(x,y,z));
%specular reflection. For this case, Thetha == -Thetha', thus
%Thetha - Thetha' = 2Thetha.
Spec = @(x,y,z,r) ((r+1)/(2*pi)) .* cos(2 * acos(dot([0 0 1],[-x -y z])/Norm(x,y,z))).^r;

phong = @(x,y,z,d,r) d * Lamb(x,y,z) + (1 - d) * Spec(x,y,z,r);

PD = @(x,y,z) rec(x,y,z) * dot([0 0 -1],[x y -z]/Norm(x,y,z)) / Dist2(x,y,z);
PD_= @(x,y) PD(x,y,1);
NoObject = @(x,y)  phong(x,y,H,1,20) * L1(x,y,H) * PD(x,y,H);

fmesh(NoObject,[-1 1 -1 1])
NumericIntegration(NoObject,-1, 1, -1, 1,0.01)
%References
%[1] http://link.springer.com/article/10.1007/s11107-015-0507-1
% Esmail, M.A. & Fathallah, H.A. Photon Netw Commun (2015) 30: 159. doi:10.1007/s11107-015-0507-1