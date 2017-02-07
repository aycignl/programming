%name : Gonul AYCI
%e-mail : aycignl@gmail.com

function [x,y,vx,vy,ax,ay,nt, curve] = getUserTraj(dt, fig)
% Erhan Oztop July 31, 2013. 
% Interactivelt draw and return a 2D trajectory. Press the mouse button and
% draw. The function will return when the button is released.
% Use with [x,y,vx,vy,ax,ay,nt, curve] = getUserTraj(dt, fig)
% dt:  period of data to be returned (e.g. 0.01)
% fig: is the figure where drawing will be made (e.g. 1)
% If no parameters are entered it willd default to dt=0.01, fig=123
% Returns: x,y as posions in x,y coordinates. vx, vy as velocities, 
% ax, ay: accelarations, nt is the time line so that you can
% for example do plot(nt, vx)
% curve is a parametric representation of the drawn shape as a function of
% curve length

global keyDown keyUp
global mousePos;
global mouseState;
global tm;


if (~exist('dt','var'))
    dt  = 0.01;
    fig = 123;
end;
tm =0;
tm_tick = 0.001;
tic;

figh = figure(fig); clf;
plot(0,0,'+'); hold on; 
axis([-2,2,-2,2]); 
drawnow;
set(figh,'WindowButtonDownFcn',{@mousePressLocalCallback ,'DOWN'});
set(figh,'WindowButtonUpFcn',{@mousePressLocalCallback ,'UP'});
set(figh,'WindowButtonMotionFcn',{@mousePressLocalCallback ,'MOVE'});

keyDown = [];
keyUp   = [];

mousePos   = [-1,-1];
mouseState = 0;  % depressed


t=.005;
iter=0;
chck=1;
oldcoords = 0;
coords = [-1,-1];
cx = []; 
cy = [];
ct = [];
rt = [];
drawing = 0;
tic;
while (coords(1)~=1),
    coords = mousePos;
    if (mouseState==1),
        drawing = 1;
        %coords;
        cx = [cx coords(1)];
        cy = [cy coords(2)];
        ct = [ct tm];
        rt = [rt coords(end)];
        plot(coords(1),coords(2),'o'); 

    end;
    if (drawing == 1 && mouseState==0)
        drawing = 0;
        plot(cx,cy,'r-');
        break;
    end;
     if (keyDown==113), 
        break; 
    end;
    pause(tm_tick);
    tm = tm + tm_tick;
   
end
trajduration = toc
SMOOTHVAL = 11;
ct = ct - ct(1);
ct = ct/ct(end);
cx = smooth(cx,SMOOTHVAL);
cy = smooth(cy,SMOOTHVAL);
spx0 = spline(ct,cx);
spy0 = spline(ct,cy);

myN0 = 50;
myt0 = [0:ct(end)/(myN0-1):ct(end)];

x0 = ppval(spx0, myt0);
y0 = ppval(spy0, myt0);

spx1 = spline(myt0,x0);
spy1 = spline(myt0,y0);
myN1 = round(1/dt);
myt1 = [0:myt0(end)/(myN1-1):myt0(end)];

mydt = myt1(2)-myt1(1);

x = ppval(spx1, myt1);
y = ppval(spy1, myt1);
plot(x,y,'g-');
plot(x,y,'m.');
title('green with magenta dots is the returned trajectory');

x = x - x(1);
vx = [0 x(2:end)-x(1:end-1)]/mydt;
%vx = smooth(vx,SMOOTHVAL)';
ax = [0 vx(2:end)-vx(1:end-1)]/mydt;


y = y - y(1);
vy = [0 y(2:end)-y(1:end-1)]/mydt;
%vy = smooth(vy,SMOOTHVAL)';
ay = [0 vy(2:end)-vy(1:end-1)]/mydt;

nt = ct*trajduration;



dels = [0 (x(2:end)-x(1:end-1)).^2 + (y(2:end)-y(1:end-1)).^2];
s    = cumsum(dels);  % curve length parameter
spx1 = spline(s,x);   % x as a function of path length
spy1 = spline(s,y);   % y as a function of path length
time2s  = spline(myt1,s); % path length as a function of time

myN1 = round(trajduration/dt);
mys1 = [0:s(end)/(myN1-1):s(end)]; 
myds = myt1(2)-myt1(1);       % infinitesimal path length

curve.t = myt1;
curve.s = s;
curve.spx = spx1;
curve.spy = spy1;

curve.x = x; %ppval(spx1, mys1);
curve.y = y; %ppval(spy1, mys1);


curve.vs = [0 curve.s(2:end)-curve.s(1:end-1)]/mydt;
curve.as = [0 curve.vs(2:end)-curve.vs(1:end-1)]/mydt;

%curve.vx = [0 curve.x(2:end)-curve.x(1:end-1)]/myds;
%curve.ax = [0 curve.vx(2:end)-curve.vx(1:end-1)]/myds;

%curve.vy = [0 curve.y(2:end)-curve.y(1:end-1)]/myds;
%curve.ay = [0 curve.vy(2:end)-curve.vy(1:end-1)]/myds;
ggg=1;
% Draws some of the returned parameters
% figure(2); clf;
% subplot(2,2,1);
% plot(ct,cx,'b.');
% subplot(2,2,2);
% plot(ct,cy,'b.');
% subplot(2,2,3);
% plot(myt1,x,'g.'); hold on;
% 
% plot(myt1,vx,'k.');
% 
% plot(myt1,ax,'c.');
% subplot(2,2,4);
% plot(myt1,y,'g.');



function mousePressLocalCallback(varargin)
global mouseState;
global mousePos;

s = varargin{3};
if (strcmp(s,'UP')),
    mouseState = 0;
end;
if (strcmp(s,'DOWN')),
    mouseState = 1;
end;
if (strcmp(s,'MOVE') || strcmp(s,'DOWN')),

    C = get (gca, 'CurrentPoint');  
    t = toc;
    mousePos=[C(1,:), t];
end;
