clear all
H = 7;          %hight of the light and PD (located at (0,0,H)

s = 150;        %shininess factor
floorRefl = 1;  %amount of light that is NOT converted to light on impact
FOV = 120/180*pi; %80 degrees in radians

halfconeapex = (60*pi)/180; %Angle at Half Power in radiance
m = -1/log2(cos(halfconeapex)); %some index describing the radiation pattern
lumI = 1600;
total = 0;
a = 0;
b = 0;
c = 0;
stepsize = 0.1;

for(Xloc =4.5/2)
    for(Yloc = 1:0.5:10)
%Object:
%Yloc = 0; %Y coordinate of the ojbect, should only be positive!
%Xloc = 0;
CarW = 1.9; %Car whidth
CarL = 4.5; %Car lenght
CarH = 1.3; %Car Hight
CarB = 0; %start of the windshield
objRefl = 1; %how much of the light is NOT converted to heat on impact.

%vectors used for angle calculations
NormPD = [0,0,-1];
NormLight = [0,0,-1];
NormTop = [0,0,1];
NormFront =[-1,0,0];
NormBack =[1,0,0];
NormSide =[0,-1,0];

%Windshield = cross([Xloc,Yloc,0] - [Xloc + CarB,Yloc,CarH] , [Xloc,Yloc,0] - [Xloc,Yloc+1,0]);
%NormWindshield = Windshield / norm(Windshield);

%For what X,Y and Z should these the reflections of the object be
%calculated?
TopReflection = @(x,y) (x>Xloc+CarB) * (x<Xloc+CarL) * (y > Yloc-CarW/2) * (y < Yloc+CarW/2) *objRefl;
SideReflection = @(x,z) (x>Xloc) * (x<Xloc+CarL) * (z>0) * (z<CarH) * (z<CarH/CarB*(x-Xloc)) * (Yloc-0.5*CarW>0) *objRefl;
BackReflection = @(y,z) (y>Yloc-0.5*CarW) * (y<Yloc+0.5*CarW) * (z>0) * (z<CarH) * (Xloc+CarL<0) *objRefl;
FrontReflection= @(y,z) (y>Yloc-0.5*CarW) * (y<Yloc+0.5*CarW) * (z>0) * (z<CarH) * (Xloc>0) *objRefl;

dist= @(x,y,z) sqrt(x.*x+y.*y+z.*z);
COSinvalshoek = @(x,y,z,n1,n2,n3) dot([x y z]/dist(x,y,z), [n1 n2 n3]/dist(n1,n2,n3));
COSuitvalshoek = @(x,y,z,n1,n2,n3) COSinvalshoek(x,y,z,n1,n2,n3);

R = @(x,y,z,v1,v2,v3) 2 .* dot([x,y,z]/dist(x,y,z),[v1,v2,v3]/dist(v1,v2,v3)) .* ([v1,v2,v3]/dist(v1,v2,v3)) - [x,y,z]/dist(x,y,z);

%I = I0 * cos(phi)^m
I = @(x,y,z) lumI .* ((m+1)/(2*pi)) .* COSuitvalshoek(x,y,-z,0,0,-1).^m;
I_= @(x,y) I(x,y,H);

Ehor = @(x,y) I(x,y,H) * COSinvalshoek(0,0,1,-x,-y,H);

%L1= (m+1)/(2*pi*d^2) * cos(uitvalshoek)
Lamb =@(x,y,z,v1,v2,v3)  (1/pi) * COSuitvalshoek(-x,-y,z,v1,v2,v3);
Spec =@(x,y,z,v1,v2,v3,s) (s+1/2*pi) * COSuitvalshoek(-x,-y,z,R(-x,-y,z,v1,v2,v3))^s;
Lamb_ = @(x,y) Lamb(x,y,1,0,0,1);
Spec_ = @(x,y) Spec(x,y,1,0,0,1,150);

%phong = @(x,y,z,v1,v2,v3,rd,s) I(x,y,z) * dot([-x,-y,z],[v1,v2,v3]) * (rd * Lamb(x,y,z,v1,v2,v3) + (1-rd) * Spec(x,y,z,v1,v2,v3,s))/norm([x,y,z])^2;
phong = @(x,y,z,v1,v2,v3,rd,s) COSinvalshoek(-x,-y,z,v1,v2,v3) * (rd * Lamb(x,y,z,v1,v2,v3) + 0)/norm([x,y,z])^2;

%PD = 1/d^2 * cos(invalshoek @PD)
rec = @(x,y,z) (acos(dot(NormPD,[x,y,-z]/norm([x,y,-z])))/FOV) < 1;
PD =@(x,y,z) 1/dist(x,y,z)^2 * COSinvalshoek(x,y,-z,0,0,-1) * rec(x,y,z);
PD_=@(x,y) PD(x,y,1);

NoObj = @(x,y)  I(x,y,H) * phong(x,y,H,NormTop(1),NormTop(2),NormTop(3),1,1) * PD(x,y,H) * floorRefl * ~TopReflection(x,y);
ObjTop = @(x,y) I(x,y,H-CarH) * phong(x,y,H-CarH,NormTop(1),NormTop(2),NormTop(3),1,1) * PD(x,y,H-CarH) * TopReflection(x,y);
ObjSide= @(x,z) I(x,Yloc-0.5*CarW,z) * phong(x,Yloc-0.5*CarW,z,NormSide(1),NormSide(2),NormSide(3),1,1) * PD(x,Yloc-0.5*CarW,z) * SideReflection(x,z);
ObjBack= @(y,z) I(Xloc+CarL,y,z) * phong(Xloc+CarL,y,z,NormBack(1),NormBack(2),NormBack(3),1,1) * PD(Xloc+CarL,y,z) * BackReflection(y,z);
ObjFront=@(y,z) I(Xloc,y,z) * phong(Xloc,y,z,NormFront(1),NormFront(2),NormFront(3),1,1) * PD(Xloc,y,z) * FrontReflection(y,z);

a= [a NumericIntegration_(NoObj,-2,6,-7.5,7.5,stepsize)];
b= [b NumericIntegration_(ObjTop,Xloc-10*stepsize,Xloc+CarL+10*stepsize,Yloc-0.5*CarW-stepsize*10,Yloc+0.5*CarW+stepsize*10,stepsize)];
c= [c NumericIntegration_(ObjSide,-H,H,0,CarH+stepsize*10,stepsize)]; %object is never going to be in the ground and higher than CarH
%d= NumericIntegration(ObjBack,-1,1,0,CarH+0.5,0.01); %object is never going to be in the ground and higher than CarH
%e= NumericIntegration(ObjFront,-1,1,0,CarH+0.5,0.01); %object is never going to be in the ground and higher than CarH
a
    end
end