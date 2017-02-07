/*

name : Gonul AYCI
e-mail : aycignl@gmail.com

*/
function [] = linePlot ()

x = 0:0.05:5;
y = sin(x.^2);

plot(x, y);
xlabel('Time')
ylabel('Amplitude')


end
