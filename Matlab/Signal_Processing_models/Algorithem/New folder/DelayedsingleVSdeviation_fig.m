clear all
color = 3; %Black, Red, Silver
meterial = 2; %Floor, Stone, Paper

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
f = f(301:1800);

slowsig = [zeros(1,1000) 10*(1+sin(-0.5*pi:0.01:1.5*pi)) zeros(1,600)];
f = f + slowsig(1:1500);

res = 0;
n = 0;
m = 200;
FIFO = m + n;

aa = mean(f(1:m)) + 4*std(f(1:m));
bb = mean(f(1:m)) - 4*std(f(1:m));

for i = FIFO+1:1500
    aa = [aa mean(f(i-FIFO:i-n))+4*std(f(i-FIFO:i-n))];
    bb = [bb mean(f(i-FIFO:i-n))-4*std(f(i-FIFO:i-n))];
end
j = 0;

for i = 1:1500-FIFO
    j = j - 1;
    if (f(i+FIFO) > aa(i)) || (f(i+FIFO) < bb(i))
        j = 100;
    end
    
    if (j < 1)
        res = [res 0];
    else
        res = [res 1];
    end
end

p = plot(f(FIFO:1500));

hold on
p = plot(aa);
p(1).LineWidth = 2;
p = plot(bb);
p(1).LineWidth = 2;
p = plot(res*200 + 375,'black');
p(1).LineWidth = 2;
legend({'s_0','\mu + 4\sigma', '\mu - 4\sigma', 'Result'},'FontSize',16);

ylabel('RSS','FontSize',16)
xlabel('Sample number','FontSize',16)
title('n = 0, d = 0, m = 200','FontSize',16)
axis([0 1100 375 650])
