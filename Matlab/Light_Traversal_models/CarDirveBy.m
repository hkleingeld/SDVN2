clear all
H = 5;          %hight of the light and PD (located at (0,0,H)

s = 150;        %shininess factor
floorRefl = 1;  %amount of light that is NOT converted to light on impact
FOV = 80/180*pi; %80 degrees in radians

halfconeapex = (35*pi)/180; %Angle at Half Power in radiance
m = -1/log2(cos(halfconeapex)); %some index describing the radiation pattern
lum = 264;
I0 = (m+1)/(2*pi)*lum;       %?Peak? Intencity of the light

h = 7;
XLOC = 14.1:-0.5:-14;%X coordinate of the object
YLOC = [1.5 4.5]

[dontcare heights] = size(h);
[dontcare locations] = size(XLOC);
[dontcare nOfYLocs] = size(YLOC);
a = zeros(locations,nOfYLocs,heights);
r = zeros(locations,nOfYLocs,heights);
c = zeros(locations,nOfYLocs,heights);
d = zeros(locations,nOfYLocs,heights);
e = zeros(locations,nOfYLocs,heights);

for n = 1:1:locations
        for r = 1:nOfYLocs
n
H = h
%Object:
Yloc = YLOC(r); %Y coordinate of the ojbect, should only be positive!
Xloc = XLOC(n);
CarW = 2; %Car whidth
CarL = 3; %Car lenght
CarH = 1; %Car Hight
CarB = 0; %start of the windshield
objRefl = 1; %how much of the light is NOT converted to heat on impact.

%vectors used for angle calculations
NormPD = [0,0,-1];
NormLight = [0,0,-1];
NormTop = [0,0,1];
NormFront =[1,0,0];
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

%I = I0 * cos(phi)^m
I = @(x,y,z) I0 * dot(NormLight,[x,y,-z]/norm([x,y,-z])).^m;
I_= @(x,y) I(x,y,H);

%L1= (m+1)/(2*pi*d^2) * cos(invanlshoek) * cos(uitvals hoek) where
%invalshoek == uitvalshoek
L1 = @(x,y,z,v1,v2,v3) (1 /(pi*norm([x,y,z])^2)) * dot([v1,v2,v3],[-x,-y,z]/norm([x,y,z]))^2;

R = @(x,y,z,n1,n2,n3) 2*dot([x,y,z],[n1,n2,n3])*[n1,n2,n3]-[x,y,z];
S1 = @(x,y,z,v1,v2,v3) (m+1)/(pi*norm([x,y,z])^2) * dot([v1,v2,v3],[-x,-y,z]/norm([x,y,z]))


%PD = 1/d^2 * cos(invalshoek @PD)
rec = @(x,y,z) (acos(dot(NormPD,[x,y,-z]/norm([x,y,-z])))/FOV) < 1;
PD =@(x,y,z) 1/norm([x,y,z]) * dot(NormPD,[x,y,-z]/norm([x,y,z])) * rec(x,y,z);
PD_=@(x,y) PD(x,y,H);

NoObj = @(x,y) I(x,y,H) * 0.08 * L1(x,y,H,NormTop(1),NormTop(2),NormTop(3)) * PD(x,y,H) * ~TopReflection(x,y);
ObjTop = @(x,y) I(x,y,H-CarH) * 0.5 * L1(x,y,H-CarH,NormTop(1),NormTop(2),NormTop(3)) * PD(x,y,H-CarH) * TopReflection(x,y);
ObjSide= @(x,z) I(x,Yloc-0.5*CarW,z) * 0.5 * L1(x,Yloc-0.5*CarW,z,NormSide(1),NormSide(2),NormSide(3)) * PD(x,Yloc-0.5*CarW,z) * SideReflection(x,z);
ObjBack= @(y,z) I(Xloc+CarL,y,z) * 0.5 * L1(Xloc+CarL,y,z,NormBack(1),NormBack(2),NormBack(3)) * PD(Xloc+CarL,y,z) * BackReflection(y,z);
ObjFront=@(y,z) I(Xloc,y,z) * 0.5 * L1(Xloc,y,z,NormFront(1),NormFront(2),NormFront(3)) * PD(Xloc,y,z) * FrontReflection(y,z);

ActualNoObject = @(x,y) I(x,y,H) * 0.08* L1(x,y,H,NormTop(1),NormTop(2),NormTop(3)) * PD(x,y,H);

a(n,r) = NumericIntegration(NoObj,-10,10,-10,10,0.05);
b(n,r) = NumericIntegration(ObjTop,-10,10,Yloc-0.5*CarW-1,Yloc+0.5*CarW+1,0.05);
c(n,r) = NumericIntegration(ObjSide,-5,5,0,CarH+0.5,0.05); %object is never going to be in the ground and higher than CarH
d(n,r) = NumericIntegration(ObjBack,-5,5,0,CarH+0.5,0.05); %object is never going to be in the ground and higher than CarH
e(n,r) = NumericIntegration(ObjFront,-5,5,0,CarH+0.5,0.05); %object is never going to be in the ground and higher than CarH
   end
end

no_obj = NumericIntegration(ActualNoObject,-10,10,-10,10,0.05)

total = a+b+c+d+e

plotCarDriveBy
