%human, 180cm, albedo of 0.25 with floor of 0.65, at 25,50,75 and 100 cm.
a180_065_25 =     [5.7077    5.7548    5.4254    5.2106    5.1939    5.2315    5.3678    5.4533    5.4623    5.5248    5.5747    5.6421 ...
    5.6703    5.6893    5.7038    5.7049    5.7220    5.7280    5.7320    5.7322    5.7349    5.7382    5.7392    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    7.7640    6.5169    5.7800    5.4641    5.3864    5.3716 ...
    5.4665    5.5149    5.5172    5.5615    5.5994    5.6548    5.6779    5.6946    5.7071    5.7080    5.7239    5.7291 ...
    5.7327    5.7330    5.7354    5.7384    5.7393    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412 ...
    5.7653    5.7615    5.6411    5.5826    5.5664    5.5472    5.5968    5.6157    5.6092    5.6345    5.6525    5.6869 ...
    5.7016    5.7114    5.7192    5.7193    5.7293    5.7328    5.7352    5.7354    5.7371    5.7392    5.7399    5.7403 ...
    5.7407    5.7409    5.7410    5.7411    5.7412    5.7413    5.6144    5.6924    5.6472    5.6349    5.6323    5.6318 ...
    5.6615    5.6723    5.6630    5.6756    5.6871    5.7086    5.7159    5.7220    5.7266    5.7258    5.7329    5.7351 ...
    5.7369    5.7370    5.7382    5.7397    5.7402    5.7406    5.7408    5.7410    5.7411    5.7412    5.7413    5.7413]

b180_065_25 = reshape(a180_065_25,120/4,4)
c180_065_25_025= [flipud(b180_065_25(2:30,1));b180_065_25(:,1)];
c180_065_25_050= [flipud(b180_065_25(2:30,2));b180_065_25(:,2)];
c180_065_25_075= [flipud(b180_065_25(2:30,3));b180_065_25(:,3)];
c180_065_25_100= [flipud(b180_065_25(2:30,4));b180_065_25(:,4)];

Xloc = -2.8:0.1:3;
fig0 = figure;
hold on
plot(Xloc,c180_065_25_100,'LineWidth',2,'DisplayName','ditance = 1m')
plot(Xloc,c180_065_25_075,'LineWidth',2,'DisplayName','ditance = 0.75m')
plot(Xloc,c180_065_25_050,'LineWidth',2,'DisplayName','ditance = 0.5m')
plot(Xloc,c180_065_25_025,'LineWidth',2,'DisplayName','ditance = 0.25m')

ylabel('Received light (lx)','FontSize',12);
xlabel('distance from light (m)','FontSize',12);
legend('show')
title('Distance influcence on reflection')
print('DistanceReflection','-dpng')
close(fig0)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55


%human, 180cm, albedo of 0.25, 0.35, 0.45 and 0.55, while the floor is 0.65 at 0.25 cm. 
a180_065_v = [5.7077    5.7548    5.4254    5.2106    5.1939    5.2315    5.3678    5.4533    5.4623    5.5248    5.5747    5.6421 ...
    5.6703    5.6893    5.7038    5.7049    5.7220    5.7280    5.7320    5.7322    5.7349    5.7382    5.7392    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    6.0166    6.0495    5.6612    5.3382    5.2626    5.2690 ...
    5.3887    5.4651    5.4691    5.5288    5.5770    5.6435    5.6711    5.6897    5.7040    5.7050    5.7221    5.7281 ...
    5.7320    5.7322    5.7350    5.7382    5.7392    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412 ...
    6.3254    6.3442    5.8970    5.4658    5.3312    5.3065    5.4095    5.4770    5.4760    5.5328    5.5793    5.6448 ...
    5.6719    5.6902    5.7043    5.7052    5.7222    5.7281    5.7320    5.7322    5.7350    5.7382    5.7392    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    6.6343    6.6390    6.1328    5.5935    5.3999    5.3439 ...
    5.4304    5.4889    5.4828    5.5367    5.5817    5.6461    5.6726    5.6906    5.7046    5.7053    5.7223    5.7282 ...
    5.7321    5.7322    5.7350    5.7382    5.7392    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412]

b180_065_v = reshape(a180_065_v,120/4,4)
c180_065_25= [flipud(b180_065_v(2:30,1));b180_065_v(:,1)];
c180_065_35= [flipud(b180_065_v(2:30,2));b180_065_v(:,2)];
c180_065_45= [flipud(b180_065_v(2:30,3));b180_065_v(:,3)];
c180_065_55= [flipud(b180_065_v(2:30,4));b180_065_v(:,4)];


fig1 = figure;
hold on
plot(Xloc,c180_065_55,'LineWidth',2,'DisplayName','Albedo = 0.55')
plot(Xloc,c180_065_45,'LineWidth',2,'DisplayName','Albedo = 0.45')
plot(Xloc,c180_065_35,'LineWidth',2,'DisplayName','Albedo = 0.35')
plot(Xloc,c180_065_25,'LineWidth',2,'DisplayName','Albedo = 0.25')

ylabel('Received light (lx)','FontSize',12);
xlabel('distance from light (m)','FontSize',12);
legend('show')
title('Albedo influcence on reflection')
print('AlbedoReflection','-dpng')
close(fig1)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%humans of 180, 160, 140 and 120cm, with an abedo of 0.25 and the walls
%have albedo of 0.65 at 0.5m
av_065_025_05 =     [7.7640    6.5169    5.7800    5.4641    5.3864    5.3716    5.4665    5.5149    5.5172    5.5615    5.5994    5.6548 ...
    5.6779    5.6946    5.7071    5.7080    5.7239    5.7291    5.7327    5.7330    5.7354    5.7384    5.7393    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    7.7307    6.5608    5.8381    5.4927    5.4025    5.3839 ...
    5.4680    5.5148    5.5152    5.5598    5.5982    5.6537    5.6773    5.6942    5.7069    5.7079    5.7238    5.7290 ...
    5.7326    5.7330    5.7354    5.7384    5.7393    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412 ...
    7.7996    6.5893    5.8567    5.5356    5.4317    5.4018    5.4827    5.5224    5.5199    5.5620    5.5987    5.6539 ...
    5.6774    5.6941    5.7068    5.7078    5.7238    5.7290    5.7326    5.7330    5.7354    5.7384    5.7393    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    7.8496    6.5401    5.8109    5.5513    5.4456    5.4129 ...
    5.4908    5.5372    5.5285    5.5667    5.6026    5.6556    5.6789    5.6948    5.7072    5.7081    5.7239    5.7291 ...
    5.7327    5.7330    5.7354    5.7384    5.7393    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412]

%humans of 180, 160, 140 and 120cm, with an abedo of 0.25 and the walls
%have albedo of 0.65 at 0.25m
av_065_025_025 = [5.7077    5.7548    5.4254    5.2106    5.1939    5.2315    5.3678    5.4533    5.4623    5.5248    5.5747    5.6421 ...
    5.6703    5.6893    5.7038    5.7049    5.7220    5.7280    5.7320    5.7322    5.7349    5.7382    5.7392    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    5.4310    5.7129    5.5385    5.2871    5.2453    5.2693 ...
    5.3798    5.4583    5.4618    5.5236    5.5736    5.6410    5.6695    5.6888    5.7034    5.7046    5.7219    5.7280 ...
    5.7319    5.7322    5.7349    5.7382    5.7392    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412 ...
    5.4267    5.7051    5.5722    5.3840    5.3135    5.3119    5.4132    5.4756    5.4719    5.5283    5.5749    5.6414 ...
    5.6698    5.6888    5.7034    5.7046    5.7219    5.7279    5.7319    5.7322    5.7349    5.7382    5.7392    5.7399 ...
    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412    5.4789    5.5956    5.4890    5.4159    5.3409    5.3327 ...
    5.4279    5.5004    5.4859    5.5359    5.5807    5.6440    5.6720    5.6898    5.7039    5.7050    5.7221    5.7280 ...
    5.7320    5.7322    5.7349    5.7382    5.7392    5.7399    5.7404    5.7407    5.7409    5.7411    5.7412    5.7412]

bv_065_025_025 = reshape(av_065_025_025,120/4,4)
c180_065_025_025= [flipud(bv_065_025_025(2:30,1));bv_065_025_025(:,1)];
c160_065_025_025= [flipud(bv_065_025_025(2:30,2));bv_065_025_025(:,2)];
c140_065_025_025= [flipud(bv_065_025_025(2:30,3));bv_065_025_025(:,3)];
c120_065_025_025= [flipud(bv_065_025_025(2:30,4));bv_065_025_025(:,4)];

bv_065_025_05 = reshape(av_065_025_05,120/4,4)
c180_065_025_05= [flipud(bv_065_025_05(2:30,1));bv_065_025_05(:,1)];
c160_065_025_05= [flipud(bv_065_025_05(2:30,2));bv_065_025_05(:,2)];
c140_065_025_05= [flipud(bv_065_025_05(2:30,3));bv_065_025_05(:,3)];
c120_065_025_05= [flipud(bv_065_025_05(2:30,4));bv_065_025_05(:,4)];


fig2 = figure;
hold on
plot(Xloc,c180_065_025_05,'LineWidth',2,'DisplayName','H = 1.8m, y = 0.5')
plot(Xloc,c180_065_025_025,'LineWidth',2,'DisplayName','H = 1.8m, y = 0.25')
%plot(Xloc,c160_065_025_05,'LineWidth',2,'DisplayName','H = 1.6m, y = 0.5')
%plot(Xloc,c160_065_025_025,'LineWidth',2,'DisplayName','H = 1.6m, y = 0.25')
plot(Xloc,c140_065_025_05,'LineWidth',2,'DisplayName','H = 1.4m, y = 0.5')
plot(Xloc,c140_065_025_025,'LineWidth',2,'DisplayName','H = 1.4m, y = 0.25')
%plot(Xloc,c120_065_025_05,'LineWidth',2,'DisplayName','H = 1.2m, y = 0.5')
%plot(Xloc,c120_065_025_025,'LineWidth',2,'DisplayName','H = 1.2m, y = 0.25')

ylabel('Received light (lx)','FontSize',12);
xlabel('distance from light (m)','FontSize',12);
legend('show')
title('Hight influcence on reflection')
print('HightReflection','-dpng')
close(fig2)