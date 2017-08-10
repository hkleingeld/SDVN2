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
f = f(1:1500);

res = 0;
n = 0;
m = 200;
q = 20;
FIFO = m + n;

aa = mean(f(1:m)) + 4*std(f(1:m)/sqrt(q));
bb = mean(f(1:m)) - 4*std(f(1:m)/sqrt(q));

for i = FIFO+1:1500
    aa = [aa mean(f(i-FIFO:i-n))+4*std(f(i-FIFO:i-n))/sqrt(q)];
    bb = [bb mean(f(i-FIFO:i-n))-4*std(f(i-FIFO:i-n))/sqrt(q)];
end
j = 0;
g = mean(f(1:FIFO))
for i = 1:1500-FIFO
    j = j - 1;
    g = [g mean(f(i+FIFO-q:i+FIFO))];
    if (g(i) > aa(i)) || (g(i) < bb(i))
        j = 100;
    end
    
    if (j < 1)
        res = [res 0];
    else
        res = [res 1];
    end
end

p = plot(g);

hold on
p = plot(aa);
p(1).LineWidth = 2;
p = plot(bb);
p(1).LineWidth = 2;
p = plot(res*100 + 450,'black');
p(1).LineWidth = 2;
legend('s','\mu + 4\sigma', '\mu - 4\sigma', 'Result');

ylabel('RSS')
xlabel('Sample number')
title('Delayed sample Example')

