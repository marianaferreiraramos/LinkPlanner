clear all
close all
clc

data=load('translate.txt');
SpS=160;

cut=length(data)/SpS-floor(length(data)/SpS);
data(length(data)-cut*SpS+1:length(data))=[];

n=length(data)/SpS;

Data=zeros(n,SpS);

for i=1:n
    Data(i,:)=data(1+(i-1)*SpS:i*SpS);
end
plot(1:SpS,Data)

%Data([4 5 12 13 20 21 28 29 36 37 44 45 52 53 60 61 68 69 76 77 84 85 92],:)=[];

n=size(Data);
n=n(1);

T=5e-10;
dt=T/SpS;
t=0:dt:dt*(SpS-1);

dt=t(2)-t(1);
dw=2*pi/(SpS*dt);
W=-SpS*dw/2:dw:(SpS/2-1)*dw;
u=zeros(1,length(W));
u(W>0)=1;

w1=1.9441e+11;

ini=40/2;
fim=120/2;

IQ=zeros(1,n);
tDS=t(1:2:SpS);

for i=1:n
    Datafft=fftshift(fft(Data(i,:)));
    Datafftplus=2*u.*Datafft;
    w=W(abs(Datafftplus)==max(abs(Datafftplus)));
    Dataifftplus=ifft(Datafftplus);
    DataifftplusDS=Dataifftplus(1:2:length(Dataifftplus));
    %DataifftplusRS=interp1(t(1:2:length(Dataifftplus)),DataifftplusDS,t);
    Datadc=DataifftplusDS.*exp(-1i*w1*tDS+(tDS-T/2).^2/(T/6)^2);
    I=mean(real(Datadc(ini:fim)));
    Q=mean(imag(Datadc(ini:fim)));
    IQ(i)=I+1i*Q;
end

w1=1.9441e+11;
plot(tDS(1:2:length(Datadc)),real(Datadc(1:2:length(Datadc))),tDS(2:2:length(Datadc)),imag(Datadc(2:2:length(Datadc))))


theo=exp(-(t-T/2).^2/(T/6)^2).*cos(w1*t+phase(IQ(i)));
figure
plot(t,theo)
hold on
plot(t,Data(i,:)/max(Data(i,:)))

theofft=fftshift(fft(theo));

theofftplus=2*u.*theofft;
theoifftplus=ifft(theofftplus);
theoifftplusDS=theoifftplus(1:2:length(theoifftplus));
tDS=t(1:2:length(theoifftplus));
tRS=t;
theoifftplusRS=interp1(tDS,theoifftplusDS,tRS);
theodc=theoifftplusRS.*exp(-1i*w1*t+(t-T/2).^2/(T/6)^2);
figure
plot(tRS,real(theodc),tRS,imag(theodc))
I=mean(real(theodc(ini:fim)));
Q=mean(imag(theodc(ini:fim)));
IQtheo=I+1i*Q;

PhaseData=phase(IQ);
PhaseData(i)
Phasetheo=phase(IQtheo)
figure
plot(IQ,'*')