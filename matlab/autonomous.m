%name : Gonul AYCI
%e-mail : aycignl@gmail.com

function [] = autonomous ()

A = [-2 1 0; 1 -2 1; 0 1 -1]

t = 0:.01:10;
x = zeros(3,length(t));
x0 = [1;0;.5];

for ii = 1 : length(t)
    x(:,ii) = expm(t(ii)*A)*x0;
end

figure,plot(t, x');
legend('v_1','v_2','v_3');
xlabel('time');
ylabel('voltages');


end
