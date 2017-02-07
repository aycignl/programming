%name : Gonul AYCI
%e-mail : aycignl@gmail.com

% determine a matrix
A = [ 0.3*cos(pi/6)   1.2*sin(pi/6)
      0.3* -sin(pi/6) 1.2*cos(pi/6)];

% adjust figure
fig1 = subplot(1,2,1);
hold on; axis equal; title('U')
fig2 = subplot(1,2,2);
hold on; axis equal; title('V')
linkaxes([fig1, fig2]);
axis([-1.5 1.5 -1.5 1.5])

for theta = 0:pi/150:2*pi

    % Her açı için birim çember üstünde bir x alıp, nereye düştüğüne bakalım
    x = [cos(theta); sin(theta)];
    y = A*x;

    % Gittikçe beyazlaşacak şekilde çizelim
    plot(fig1, [0 y(1)], [0 y(2)], ...
        'Color', [theta/(2*pi) theta/(2*pi) theta/(2*pi)], ...
        'LineWidth', 3);
    plot(fig2, [0 x(1)], [0 x(2)], ...
        'Color', [theta/(2*pi) theta/(2*pi) theta/(2*pi)], ...
        'LineWidth', 3);
end

% Bakalım, matlab'ın bulduğu değerler nasıl gözüküyor
[U, S, V] = svd(A);

% Sol ve sağ tekil vektörleri çizelim
plot(fig1,[0 U(1,1)], [0 U(2,1)], 'Color', 'red', 'LineWidth', 3);
plot(fig1,[0 U(1,2)], [0 U(2,2)], 'Color', 'blue', 'LineWidth', 3);
plot(fig2,[0 V(1,1)], [0 V(2,1)], 'Color', 'red', 'LineWidth', 3);
plot(fig2,[0 V(1,2)], [0 V(2,2)], 'Color', 'blue', 'LineWidth', 3);
