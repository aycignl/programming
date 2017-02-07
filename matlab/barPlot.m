%name : Gonul AYCI
%e-mail : aycignl@gmail.com

function [] = barPlot ()

x=-2.9:0.2:2.9;
bar(x,exp(-x.*x));


x=0:0.25:10;
stairs(x,sin(x));


X= 0:pi/15:4*pi;
Y=exp(2*cos(X));
plot(X,Y,'b+')


t=0:pi/100:2*pi;
y=sin(t);
plot(t,y)
xlabel('sinüs sinyali')


t=0:pi/50:10*pi;
plot3(sin(t),cos(t),t)
grid on;axis square


x=0:0.05:5;
y=sin(x.^2);
plot(x,y);
xlabel('Time')
ylabel('Amplitude')


[X,Y]=meshgrid(-3:.125:3)
Z=peaks(X,Y);
meshz(X,Y,Z)


[X,Y]=meshgrid([-2:.25:2]);
Z=X.*exp(-X.^2 -Y.^2);
surf(X,Y,Z)
 
end
