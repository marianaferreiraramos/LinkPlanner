clear all
close all
clc

name3='TI_AMP03.sgn';
[data3,symbolPeriod3,samplingPeriod3,type3,numberOfSymbols3]=readSignal(name3);
time3=0:samplingPeriod3:samplingPeriod3*(length(data3)-1);

name4='TI_AMP04.sgn';
[data4,symbolPeriod4,samplingPeriod4,type4,numberOfSymbols4]=readSignal(name4);
time4=0:samplingPeriod4:samplingPeriod4*(length(data4)-1);

time=0:samplingPeriod3:samplingPeriod3*(length(data4)-1);
Databefore=fftshift(fft(data3));
Databeta0=fftshift(fft(data4));

figure
plot(time,data3,time,data4)
ylabel('Amp')
xlabel('Time [s]')
legend('Output before filter','Filter with \beta=0')
set(gca,'FontSize',15,'fontWeight','bold')
set(findall(gcf,'type','text'),'FontSize',20,'fontWeight','bold')


N=length(time);
dw=2*pi/(N*samplingPeriod3);
w=-N*dw/2:dw:(N/2-1)*dw;


figure 
plot(w,abs(Databefore),w,abs(Databeta0))

ylabel('Amp')
xlabel('Angular Frequency [rad/s]')
legend('Output before filter','Filter with \beta=0')
set(gca,'FontSize',15,'fontWeight','bold')
set(findall(gcf,'type','text'),'FontSize',20,'fontWeight','bold')

%%
delay=1;
time7=time7(1:(length(time7)-delay+1));
data7=data7(delay:length(data7));
figure
plot(time7,data7,'*',time6,data6)

clc

figure
plot(time3,data3,time4,data4)
legend('noise','filtered')

var(data3)
var(data4)
