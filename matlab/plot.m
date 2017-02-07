%name : Gonul AYCI
%e-mail : aycignl@gmail.com
function [] = plot()

% It was used from ismailari.com

figure
x = pi*[-24:24]/24;
plot(x, sin(x))
xlabel('radyan')
ylabel('sinus degeri')
title('sinus grafigi')

%*************************
x = pi*[-24:24]/24;
subplot(1, 2, 1)
plot(x, sin(x))
axis square
title('sin grafigi')
subplot(1, 2, 2)
plot(x, cos(x))
axis square
title('cos grafigi')

%**************************
x = pi*[-24:24]/24;
figure
plot(x, sin(x), '-g');
hold on
plot(x, cos(x), '--r');
legend('sin', 'cos')
hold off

%*************************
x = pi*[-24:24]/24;
stem(x, sin(x), '--ro', 'LineWidth', 2, ...
    'MarkerEdgeColor', 'k', ...
    'MarkerFaceColor', 'g', ...
    'MarkerSize', 7)
title('sin grafigi')

%*************************
m = rand(16,16);
imagesc(m)

colormap gray
axis image
axis off

close all
end
