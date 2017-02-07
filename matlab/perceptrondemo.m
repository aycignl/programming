

%name : Gonul AYCI
%e-mail : aycignl@gmail.com

%
% Demo of the perceptron learning rule.  Invoke with
% "perceptrondemo", then hit return to cycle through training
% examples.
%

function perceptrondemo

figure(1)
clf

pos = randn(10,1)*.5+2;
neg = randn(10,1)*.4+5;
xpos = [sin(pos) cos(pos) ones(size(pos))];
xneg = [sin(neg) cos(neg) ones(size(neg))];
xs = [xpos; xneg]; ys = [ones(size(pos)); -ones(size(neg))];
perm = randperm(20);
xs = xs(perm,:);
ys = ys(perm);

w = -[1.3 1.95 .5]';

for j = 1:100
for i = 1:length(xs)

  oldw = w;

  ex = xs(i,:);
  py = sign(ex*w)
  ty = ys(i)

  if (py ~= ty)
    w = w + ex'*ty;
  end

  subplot(1,2,1);
  plot(xpos(:,1),xpos(:,2),'go',xneg(:,1),xneg(:,2),'rx', ...
       'LineWidth',2,'MarkerSize',10)
  hold on
  plot(ex(1),ex(2),'bs','MarkerSize',20);

  h = drawline(oldw,3);
  set(h,'LineWidth',3);
  h = drawline(w,3);

  axis equal
  axis([-1.1 1.1 -1.1 1.1]);
  plot(0,0,'k+', 'MarkerSize',3);
  hold off

  subplot(1,2,2);
  plot(oldw(1),oldw(2),'bo','LineWidth',2,'MarkerSize',10)
  hold on
  line([oldw(1) w(1)], [oldw(2) w(2)]);

  axis equal
  axis([-3 3 -3 3]);
  plot(0,0,'k+', 'MarkerSize',3);
  hold off

  pause

end
end


function h = drawline(w, scale)

x = w(1);
y = w(2);
c = w(3);

len = sqrt(x*x+y*y+1e-6);
k = -c / (len*len);

nx = x/len;
ny = y/len;

h = line([k*x-ny*scale k*x+ny*scale], [k*y+nx*scale k*y-nx*scale]);
