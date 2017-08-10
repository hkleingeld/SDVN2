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
H = 7;
D = 15;

a = 150;

dist= @(x,y,z) sqrt(x.*x+y.*y+z.*z);
COSinvalshoek = @(x,y,z,n1,n2,n3) dot([x y z]/dist(x,y,z), [n1 n2 n3]/dist(n1,n2,n3));
COSuitvalshoek = @(x,y,z,n1,n2,n3) COSinvalshoek(x,y,z,n1,n2,n3);

COSinvalshoek_= @(v1,v2) dot(v1/norm(v1),v2/norm(v2));
COSuitvalshoek_= @(v1,v2) COSinvalshoek_(v1,v2);

mirror = @(x,y) (y<0.1) * (y>-0.1);

FOV = 120/180*pi; %80 degrees in radians

halfconeapex = (60*pi)/180; %Angle at Half Power in radiance
m = -1/log2(cos(halfconeapex)); %some index describing the radiation pattern
lumI = 800;

%light emitted divided by distance squared? Distance seems to be missing in
%phong model
I = @(x,y,z) lumI .* ((m+1)/(2*pi)) .* COSuitvalshoek(x,y,-z,0,0,-1).^m;
I_= @(x,y) I(x,y,H) * dist(x,y,H)^2;

Ehor = @(x,y) I(x,y,H) * COSinvalshoek(0,0,1,-x,-y,H) / dist(x,y,H)^2;

Lamb =@(x,y,z,v1,v2,v3)  (1/pi) * COSuitvalshoek(D-x,-y,z,v1,v2,v3);
R = @(v1,v2) 2 .* dot(v1/norm(v1),v2/norm(v2)) * (v2/norm(v2)) - (v1/norm(v1));
Spec =@(x,y,z,n1,s) ((s+1)/(2*pi)) * COSinvalshoek_(R([-x -y z],n1),[D-x -y z]);


rec = @(x,y,z) (acos(dot(NormPD,[x,y,-z]/norm([x,y,-z])))/FOV) < 1;
PD =@(x,y,z) 1/dist(D-x,y,z)^2 * COSinvalshoek(x-D,y,-z,0,0,-1);
PD_=@(x,y) PD(x,y,H);

tot = @(x,y) Ehor(x,y) * ((Lamb(x,y,H,0,0,1)* ~mirror(x,y))+Spec(x,y,H,[0 0 1],150)*mirror(x,y)) * PD(x,y,H);
fmesh(tot,[-7.5 20 -7.5 7.5])
%fmesh(tot,[0 5 -5 5]) %shows how much light is reflected to the photo diode from each surface
NumericIntegration_(tot, -7.5, 20, -7.5, 7.5, 0.1)