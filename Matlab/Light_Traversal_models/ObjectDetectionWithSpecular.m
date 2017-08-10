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


H = 5;
I0 = 200;
s = 150;
floorRefl = 1;
%Light source description%
halfconeapex = (120/2*pi)/180;
m = -1/log2(cos(halfconeapex));

distance = 0.5
signal = zeros(25,1);

    %Object:
    Yloc = 0;
    Xloc = distance;
    Xl = 1;
    Yl = 1;
    Zl = 1;
    objRefl = 1;

    Hobj = H - Zl;

    ShadowAngleX = atan((Xloc+Xl)/(H-Zl));
    Xshaddow = tan(ShadowAngleX) * H;

    ShadoAngleY = atan((0.5*Yl)/(H-Zl));
    Yshaddow = tan(ShadoAngleY) * H;

    SAngle = atan(sqrt(Xloc*Xloc+0.5*Yl*0.5*Yl)/(H-Zl));
    xy = H / cos(SAngle);
    XsShaddowEnd = sqrt(xy*xy - H*H);
    a = (Yshaddow-0.5*Yl)/(XsShaddowEnd - Xloc);
    b = -a * XsShaddowEnd + Yshaddow;

    NoGroundReflection = @(x,y) (x>Xloc) .* (x < Xshaddow) .* (y < Yshaddow) .* (y > -Yshaddow) .* (a.*x + b > y) .*(-a.*x-b<y);
    ObjectReflection = @(x,y) (x>Xloc) .* (x<Xloc+Xl) .* (y < 0.5.*Yl) .* (y > -0.5.*Yl) .*objRefl;
    ObjectSideRefelction = @(y,z) (y>-Yl*0.5) .* (y<Yl*0.5) .* (z>0) .* (z<Zl) .*objRefl;
    %ObjLoc = @(x,y) 

    %PD settings
    FOV = 80/180*pi; %80 degrees in radians
    rec = @(x,y,z) (acos(z./Norm(x,y,z)))./(FOV) <= 1;

    %omschrijving licht bron
    %I = @(x,y) I0 .* cos(acos(H./Norm(x,y,H))).^2;
    I  = @(x,y,z) I0 .* (z./Norm(x,y,z)).^m;

    %Ehor = @(x,y) (m+1) .*I(x,y) .* cos(acos(H./Norm(x,y,H))) ./ Norm(x,y,H).*Norm(x,y,H)
    %             lichtbron           Uitvalshoek            Afstand^2
    L1 = @(x,y,z) (m+1) .* I(x,y,z) .*(z ./ Norm(x,y,z)) ./ (Dist2(x,y,z) * 2 * pi);

    %             lichtbron             Uitvalshoek                 Afstand^2
    L11= @(x,y,z) (m+1) .* I(x,y,H-z).*((H-z) ./ Norm(x,y,H-z)) ./ (Dist2(x,y,H-z) * 2 * pi);

    %L2 = @(x,y) cos(acos(H./Norm(x,y,H))) .* cos(acos(H./Norm(x,y,H))) / (Norm(x,y,H).*Norm(x,y,H) * pi)
    %L2 = @(x,y) H./Norm(x,y,H) .* H./Norm(x,y,H) / (Dist2(x,y,H) * pi)
    %              uitvalshoek*invalshoek  Afstand^2
    L2  = @(x,y,z) (z./Norm(x,y,z)).^2 ./ (Dist2(x,y,z) * pi);

    %P2 = @(x,y,z) (m+1) / (2*pi) * cos(uitvalshoek - invalshoek)^m * invalshoekPD
    %P2 = @(x,y,z) (m+1) / (2*pi) * cos(2*acos(z./Norm(x,y,z)))^m * invalshoekPD
    P2  = @(x,y,z) (s+1) / (2*pi) * (2*(z./Norm(x,y,z))-1)^s * (z./Norm(x,y,z));
    P2_ = @(x,y) P2(x,y,H);
                   %uitvals hoek                    invalshoek               Afstand^2
    L22 = @(x,y,z) (Norm(x,y,0)./Norm(x,y,H-z)) .* ((H-z)./Norm(x,y,H-z)) ./ (Dist2(x,y,H-z) * pi);


    HnoObj = @(x,y) L1(x,y,H) .* L2(x,y,H) .* rec(x,y,H) .*floorRefl;
    HObjRest = @(x,y) L1(x,y,H) .* L2(x,y,H) .* rec(x,y,H) .* ~NoGroundReflection(x,y) .*floorRefl;
    HobjTop = @(x,y) L1(x,y,Hobj) .* (0.7*P2(x,y,Hobj)+(0.3*L2(x,y,Hobj))) .* rec(x,y,Hobj) .* ObjectReflection(x,y);
    HobjSide = @(y,z) L11(Xloc,y,z) .* L22(Xloc,y,z) .* ObjectSideRefelction(y,z);
    %Htot = @(x,y) H_(x,y) + Hobj(x,y);

    %NoObj = integral2(HnoObj,-5,5,-5,5)
    %NoObj = NumericIntegration(HnoObj,-5,5,-5,5,0.01); %riemannsom
    ObjTop =  NumericIntegration(HobjTop,-5,5,-5,5,0.01); %riemannsom
    ObjSide = NumericIntegration(HobjSide,-5,5,-5,5,0.01); %riemannsom
    ObjRest = NumericIntegration(HObjRest,-5,5,-5,5,0.01); %riemannsom

    signal(1) = ObjTop+ObjSide+ObjRest
    %absdiff = ObjTop+ObjSide+ObjRest-NoObj;
    %propdiff(n) = ((ObjTop+ObjSide+ObjRest)-NoObj)/NoObj


fmesh(HobjTop,[-4,4])
hold on
fmesh(HObjRest,[-4,4])
%References
%[1] http://link.springer.com/article/10.1007/s11107-015-0507-1
% Esmail, M.A. & Fathallah, H.A. Photon Netw Commun (2015) 30: 159. doi:10.1007/s11107-015-0507-1