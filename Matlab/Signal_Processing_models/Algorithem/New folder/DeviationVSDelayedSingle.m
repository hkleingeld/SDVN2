clear all
color = 3; %Black, Red, Silver
meterial = 1; %Floor, Stone, Paper

f = csvread('Paper_nothing_with_noise.txt');

if color == 1
    if meterial == 1;
        f = csvread('Black_Floor_center.txt');
        f = [f csvread('Black_Floor_middle.txt')];
        f = [f csvread('Black_Floor_side.txt')];
    end
    
    if meterial == 2;
        f = csvread('Black_Stone_center.txt');
        
        %f = [f csvread('Black_Stone_side.txt')];
        f = [f csvread('Black_Stone_left.txt')];
        f = [f csvread('Black_Stone_middle.txt')];
    end
    
    if meterial == 3;
        f = csvread('Black_Paper_center.txt');
        f = [f csvread('Black_Paper_middle.txt')];
        f = [f csvread('Black_Paper_side.txt')];
        f = [f csvread('Black_Paper_left.txt')];
    end
end

if color == 2
    if meterial == 1;
        f = csvread('Red_Floor_center.txt');
        f = [f csvread('Red_Floor_middle.txt')];
        f = [f csvread('Red_Floor_side.txt')];
    end
    if meterial == 2;
        f = csvread('Red_Stone_center.txt');
        f = [f csvread('Red_Stone_middle.txt')];
        f = [f csvread('Red_Stone_side.txt')];
        f = [f csvread('Red_Stone_left.txt')];
    end
    
    if meterial == 3;
        f = csvread('Red_Paper_center.txt');
        f = [f csvread('Red_Paper_middle.txt')];
        f = [f csvread('Red_Paper_side.txt')];
        f = [f csvread('Red_Paper_left.txt')];
    end
end

if color == 3
    if meterial == 1;
        f = csvread('Silver_Floor_center.txt');
        %f = [f csvread('Silver_Floor_middle.txt')];
        %f = [f csvread('Silver_Floor_side.txt')];
    end

    if meterial == 2;
        f = csvread('Silver_Stone_center.txt');
        f = [f csvread('Silver_Stone_middle.txt')];
        f = [f csvread('Silver_Stone_side.txt')];
        f = [f csvread('Silver_Stone_left.txt')];
    end
 
    if meterial == 3;
        f = csvread('Silver_Paper_center.txt');
        f = [f csvread('Silver_Paper_middle.txt')];
        f = [f csvread('Silver_Paper_side.txt')];
        f = [f csvread('Silver_Paper_left.txt')];
    end
end

f(f==0) = []; %remove 0'
f = f(1:1000)
%f = j;
T = 0.01;
delay = 100; %3 seconds
d = 200; %samples in main std
e = 1;  %samples in deleayed distribution


stdAtDetect = 0;

gs = std(f(1:d));
gm = mean(f(1:d))

hs = std(f(1:d))
hm = mean(f(1:d))
ga = std(f(1:d));
res = 0;
[dc N] = size(f)

for n = 2:1:N-(delay+d)
    ga = [ga std(f(n:n+d))];
    hm = [hm mean(f(n+d+delay-e+1:n+d+delay))];
    gs = [gs std(f(n:n+d))];
    gm = [gm mean(f(n:n+d))];
    
    if res(n-1) == 0
        stdAtDetect = gs(n);
        i = 0;
        %res = [res ((hm(n) < gm(n)- gm(n)*T) | (hm(n) > gm(n)+gm(n)*T))];
        res = [res ((hm(n) < gm(n)- gs(n)*4) | (hm(n) > gm(n)+gs(n)*4))];
    else
        i = i + 1;
        if i > d+2*delay
            if stdAtDetect > gs(n)
                res = [res 0];
            else
                res = [res 1];
                stdAtDetect = stdAtDetect + 0.01
            end
        else
            res = [res 1];
        end
    end
    
    e = 1;
end

%plot(gm+3*gs)
hold on
plot(gm+gs*4)
%plot(gm-3*gs)
plot(gm-gs*4)
plot(hm)
plot(mean(f)+res*50)

