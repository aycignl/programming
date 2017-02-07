%name : Gonul AYCI
%e-mail : aycignl@gmail.com

function [] = harmonic ()

%sketch
w=1;
A=[0 w;-w 0 ];

[X,Y]=meshgrid(-10:10);

%a = reshape(b, 2, 3)

x=reshape(X,1,numel(X));
y=reshape(Y,1,numel(Y));

pos=[x;y];
vel=A*pos;
u=vel(1,:);
v=vel(2,:);

figure,quiver(x,y,u,v),axis([-10 10 -10 10]);

end
