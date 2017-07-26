clear all
close all
clc

cd ..

data=load('translate.txt');

cd signals\

BeforePhotodiodefile='S1.sgn';
AfterPhotodiodefile='S5.sgn';
InternalTIAmpfile1='TI_AMP01.sgn';
InternalTIAmpfile2='TI_AMP02.sgn';
InternalTIAmpfile3='TI_AMP03.sgn';
AfterTIAmpfile='S6.sgn';
AfterSamplerfile='S7.sgn';

BeforePhotodiode=readSignal(BeforePhotodiodefile,1e5);
AfterPhotodiode=readSignal(AfterPhotodiodefile,1e5);
InternalTIAmp1=readSignal(InternalTIAmpfile1,1e5);
AfterTIAmp=readSignal(AfterTIAmpfile,1e5);
AfterSampler=readSignal(AfterSamplerfile,1e5);

plot(1616:1616+length(data)-1,data,0:length(AfterTIAmp)-1,AfterTIAmp)

%%
figure
plot(0:length(BeforePhotodiode)-1,BeforePhotodiode)
figure
plot(0:length(AfterPhotodiode)-1,AfterPhotodiode)

figure
plot(0:length(InternalTIAmp1)-1,InternalTIAmp1);


figure
plot(0:length(AfterTIAmp)-1,AfterTIAmp);
figure
plot(AfterSampler)