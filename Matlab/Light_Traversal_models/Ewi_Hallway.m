%         Based on hallway 9th floor EWI
%   
%                     I0
% ^ |                                     | ^
% | |                                     | |   
% z |                                     | |
% - |                                     | |        
% a |                                     | |            
% x |                                     | | H = 280 cm             
% i |                                     | |       
% s |                                     | |             
%   |               W = 222cm             | |
%   |<---------------- W ---------------->| |
%___|_____________________________________| v
%//////////////////////////////////////////
%                   x-axis ------>
% y axis positive goes "In the screen", y negative comes "out of the screen"
NrOfSteps = 25;
FOV = 120/180*pi;               %120 degrees in radians
halfconeapex = (16*pi)/180;     %Angle at Half Power in radiance
m = -1/log2(cos(halfconeapex)); %some index describing the radiation pattern
lumI = 260;

H = 2.80;
W = 2.2;
step = (W-2*0.01)/NrOfSteps;
Alb_Floor = 0.75;
Alb_Wall = 0.75;
Alb_Human = 0.5;

Floor = @(x,y) (x<W/2+0.001) .* (x>-W/2+0.001);
Wall_1= @(x,z) (x == -W/2) .* (z > 0) .* (z < H);
Wall_2= @(x,z) (x == W/2) .* (z > 0) .* (z < H);

Norm_Floor  = [0 0 1];
Norm_Wall_1 = [1 0 0];
Norm_Wall_2 = [-1 0 0];
Norm_Light = [0 0 -1];


dist = @(x,y,z) sqrt(x.*x+y.*y+z.*z);
COSinvalshoek = @(x,y,z,n1,n2,n3) dot([x y z]/dist(x,y,z), [n1 n2 n3]/dist(n1,n2,n3));
COSuitvalshoek = @(x,y,z,n1,n2,n3) COSinvalshoek(x,y,z,n1,n2,n3);

COSinvalshoek_= @(v1,v2) dot(v1/norm(v1),v2/norm(v2));
COSuitvalshoek_= @(v1,v2) COSinvalshoek_(v1,v2);

R = @(v1,v2) 2 .* dot(v1/norm(v1),v2/norm(v2)) * (v2/norm(v2)) - (v1/norm(v1));

%I = I0 * cos(phi)^m
I = @(x,y,z) lumI .* ((m+1)/(2*pi)) .* COSuitvalshoek_([x y -(H-z)],Norm_Light).^m;
I_= @(x,y) I(x,y,H);

Ehor = @(x,y,z,n1) I(x,y,z) .* COSinvalshoek_([-x -y H-z],n1) ./ dist(x,y,H-z).^2;
Ehor_ = @(x,y) Ehor(x,y,0,[0 0 1])


%L1= (m+1)/(2*pi*d^2) * cos(uitvalshoek)
Lamb =@(x,y,z,n1)  (1/pi) * COSuitvalshoek_([-x,-y,z],n1);
Spec =@(x,y,z,n1,s) ((s+1)/(2*pi)) * COSinvalshoek_(R([-x -y z],n1),[-x -y z]);
    
Lamb_ = @(x,y) Lamb(x,y,1,0,0,1);
Spec_ = @(x,y) Spec(x, y, 1,[0 0 1],10);

%phong = @(x,y,z,v1,v2,v3,rd,s) I(x,y,z) * dot([-x,-y,z],[v1,v2,v3]) * (rd * Lamb(x,y,z,v1,v2,v3) + (1-rd) * Spec(x,y,z,v1,v2,v3,s))/norm([x,y,z])^2;
phong = @(x,y,z,v1,v2,v3,rd,s) COSinvalshoek(-x,-y,z,v1,v2,v3) * (rd * Lamb(x,y,z,v1,v2,v3) + (0) * Spec(x,y,z,[v1 v2 v3],20))/norm([x,y,z])^2;

%PD = 1/d^2 * cos(invalshoek @PD)
rec = @(x,y,z) (acos(dot(NormPD,[x,y,-z]/norm([x,y,-z])))/FOV) < 1;
PD =@(x,y,z) 1/dist(x,y,z)^2 * COSinvalshoek(x,y,-z,0,0,-1) * rec(x,y,z);
PD_=@(x,y) PD(x,y,1);

I_Floor = @(x,y) (Ehor(x,y,0,Norm_Floor) + Ehor(x,y-2,0,Norm_Floor) + Ehor(x,y+2,0,Norm_Floor) + Ehor(x,y-4,0,Norm_Floor) + Ehor(x,y+4,0,Norm_Floor))* Floor(x,y);
I_Wall_1=@(y,z) Ehor(-W/2,y,z,Norm_Wall_1).* Wall_1(-W/2,z) %+ Ehor(-W/2,y-2,z,Norm_Wall_1) + Ehor(-W/2,y+2,z,Norm_Wall_1) + Ehor(-W/2,y-4,z,Norm_Wall_1) + Ehor(-W/2,y+4,z,Norm_Wall_1)) ;
I_Wall_2=@(y,z) Ehor(W/2,y,z,Norm_Wall_2) .* Wall_2(W/2,z)%+ Ehor(W/2,y-2,z,Norm_Wall_2) + Ehor(W/2,y+2,z,Norm_Wall_2)+Ehor(W/2,y-4,z,Norm_Wall_2) + Ehor(W/2,y+4,z,Norm_Wall_2)) ;

[Ehor_Floor floorx floory] = HalfNumericIntegration(I_Floor, -W/2+0.01,W/2-0.01,-5,5,step);
[Ehor_Wall1 wally wallz] = HalfNumericIntegration(I_Wall_1,-5,5,0.01,H-step,step);
[Ehor_Wall2 wally wallz] = HalfNumericIntegration(I_Wall_2,-5,5,0.01,H-step,step);

Ehor_Floor_Via_1 = zeros(floory,floorx);
Ehor_Floor_Via_2 = zeros(floory,floorx);

for fx = 1:floorx
    for fy = 1:floory
        for wy = 1:wally
            for wz = 1:wallz
                x = (fx-1)*step+0.01;
                y = ((fy-1) - (wy-1)) * step;
                z = (wz-1)*step + 0.01;
                Ehor_Floor_Via_1(fy,fx) = Ehor_Floor_Via_1(fy,fx) + Alb_Wall * Ehor_Wall1(wz,wy) * 1/pi * COSuitvalshoek_(Norm_Wall_1,[x y -z]) * COSinvalshoek_(Norm_Floor,[-x -y z])/dist(x,y,z).^2 ;
            end
        end
    end
end

parfor fx = 1:floorx
    for fy = 1:floory
        for wy = 1:wally
            for wz = 1:wallz
                x = (fx-1)*step - W;
                y = ((fy-1) - (wy-1)) * step;
                z = (wz-1)*step + 0.01;
                Ehor_Floor_Via_2(fy,fx) = Ehor_Floor_Via_2(fy,fx) + Alb_Wall * Ehor_Wall2(wz,wy) * 1/pi * COSuitvalshoek_(Norm_Wall_2,[x y -z]) * COSinvalshoek_(Norm_Floor,[-x -y z])/dist(x,y,z).^2 ;
            end
        end
    end
end
tot = Ehor_Floor_Via_1+Ehor_Floor_Via_2+Ehor_Floor;

Ehor_Floor_Via_1(:,1) = 0;
Ehor_Floor_Via_2(:,26)= 0;
surf((Ehor_Floor_Via_1+Ehor_Floor_Via_2+Ehor_Floor))

LightAtPD = zeros([floory floorx]);
parfor fx = 1:floorx
    for fy = 1:floory
        x = (fx-1) * step - W/2
        y = (fy-1) * step - 5;
        LightAtPD(fy,fx) = tot(fy,fx) * Alb_Floor / pi * COSuitvalshoek_(Norm_Floor,[x y H]) * COSinvalshoek_(Norm_Light,Norm_Light)
    end
end

