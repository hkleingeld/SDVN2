[n dc] = size(total)
carxloc = 14.1:-0.5:-14
no_obj(1:n) = no_obj(1);
human = [0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5693 0.5693 0.5693 0.5693 0.5694 0.5696 0.5698 0.5702 0.5708 0.5721 0.5744 0.5790 0.5887 0.6105 0.6635 0.7621 0.8878 1.0752 1.1037 0.8734 0.7362 0.6472 0.6072 0.5878 0.5789 0.5745 0.5722 0.5710 0.5702 0.5698 0.5695 0.5694 0.5693 0.5693 0.5693 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692]

carclose= [0.5692 0.5692 0.5692 0.5695 0.5698 0.5702 0.5708 0.5717 0.5731 0.5749 0.5776 0.5816 0.5876 0.5965 0.6094 0.6278 0.6540 0.6896 0.7387 0.8115 0.9478 1.2922 2.9557 4.1648 4.2927 4.3059 4.2741 4.0148 2.0102 1.1044 0.8790 0.7771 0.7164 0.6736 0.6423 0.6196 0.6036 0.5925 0.5849 0.5798 0.5764 0.5741 0.5725 0.5713 0.5705 0.5700 0.5696 0.5694 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692]
carfar= [0.5692 0.5692 0.5692 0.5693 0.5695 0.5697 0.5700 0.5704 0.5710 0.5717 0.5726 0.5739 0.5757 0.5781 0.5813 0.5855 0.5910 0.5976 0.6054 0.6142 0.6236 0.6330 0.6415 0.6482 0.6513 0.6523 0.6501 0.6464 0.6376 0.6288 0.6194 0.6103 0.6019 0.5946 0.5886 0.5837 0.5799 0.5771 0.5749 0.5733 0.5722 0.5714 0.5707 0.5702 0.5699 0.5696 0.5694 0.5693 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692 0.5692]

%carclose = flipud(total(:,1));
%carfar = flipud(total(:,2));

subplot(1,2,1)
hold on
plot(carxloc,carclose,'LineWidth',2,'DisplayName','Vehicle lane 1');
plot(carxloc,carfar,'LineWidth',2,'DisplayName','Vehicle lane 2');
plot(carxloc,human,'LineWidth',2,'DisplayName','Human passing by')
plot(carxloc,no_obj,'LineWidth',2,'DisplayName','No vehicle')

legend('show')
axis([-14 14 0.5 4.5])
ylabel('RSS [lx]');
xlabel('Location in lane [m]');
title('Theoretical light response')

subplot(1,2,2)
hold on
plot(carxloc,flipud(total(:,1)),'LineWidth',2,'DisplayName','Vehicle lane 1');
plot(carxloc,flipud(total(:,2)),'LineWidth',2,'DisplayName','Vehicle lane 2');
plot(carxloc,human,'LineWidth',2,'DisplayName','Human passing by')
plot(carxloc,no_obj,'LineWidth',2,'DisplayName','No vehicle')
legend('show')
ylabel('RSS [lx]');
title('Theoretical light response (zoomed in)')
xlabel('Location in lane [m]');
axis([-14 14 0.55 0.7])



