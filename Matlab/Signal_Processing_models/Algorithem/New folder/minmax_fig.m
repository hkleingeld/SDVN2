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

n = 400;

%init
a = f(1);
b = f(1);
aa = a;
bb = b;
res = 0;
j = 0;

for i = 1:1000
    
    if i < n %init
        res = [res 0];
        
        if f(i) > a
            aa = [aa f(i)];
            a = f(i);
        else
            aa = [aa a];
        end
        
        if f(i) < b
            bb = [bb f(i)];
            b = f(i);
        else
            bb = [bb b];
        end
 
    else %running
        bb = [bb b];
        aa = [aa a];
       if (f(i) > a) || (f(i) < b) || (j > 1)
           j = 100
       end
       
        if (j < 1)
            res = [res 0];
        else
            res = [res 1];
        end
    end
end
a = 1:1000;

p = plot(f(1:1000));

hold on
p = plot(aa);
p(1).LineWidth = 2;
p = plot(bb);
p(1).LineWidth = 2;
p = plot(res*30 + 695,'black');
p(1).LineWidth = 2;
legend('s','s_m_a_x', 's_m_i_n', 'Result');

ylabel('RSS')
xlabel('Sample number')
title('Min-Max Example')
axis([0 1000 695 745])
