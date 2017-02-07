%name : Gonul AYCI
%e-mail : aycignl@gmail.com
function mse = prostateData

load('prostate');

X=trainData;
Y=trainResponse;
Xt=testData;
Yt=testResponse;

X =  [ones(length(Y),1) X];
Xt =  [ones(length(Yt),1) Xt]; 

C=grad_descent(X,Y);
mse=0;
for I=1:length(Yt)
       x=Xt(I,:);
       f=x*C';
       mse= (f-Yt(I))^2;
end
mse = mse / length(Yt);
return

function C=grad_descent(X,Y)
nu=0.00001; 
C=rand(1,size(X,2));
 err=1000;
 it=0;
 while err > 0.001
 it=it +1;
 Cnew=C;
 for I=1:length(C)
     Cnew(I)=C(I)-nu*grad(X,Y,I,C);
 end
 err=sum(abs(Cnew-C));
 C=Cnew;
 if mod(it,100) == 0
    nu = nu / 2;
 end
 end
return

function g =grad (X,Y,k,C)
   g=0;
   for I=1:length(Y)
       x=X(I,:);
       f=x*C';
       g = g+(f-Y(I))*x(k);
   end
    g=g/length(Y);
return
