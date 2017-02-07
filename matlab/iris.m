/*

name : Gonul AYCI
e-mail : aycignl@gmail.com

*/
function [] = iris ()

  clc
  clear all
  close all force
  
    data=load('iris.txt');

    data1=data(1:50,1:5);
    data2=data(51:100,1:5);
    data3=data(101:150,1:5);

    d1 = splitData(data1,0.75);
    m1=mean(d1.train);
    v1=var(d1.train);

    d2 = splitData(data2,0.75);
    m2=mean(d2.train);
    v2=var(d2.train);

    d3 = splitData(data3,0.75);
    m3=mean(d3.train);
    v3=var(d3.train);

    p_C1=size(d1.train)/(size(d1.train)+size(d2.train)+size(d3.train));
    p_C2=size(d2.train)/(size(d1.train)+size(d2.train)+size(d3.train));
    p_C3=size(d3.train)/(size(d1.train)+size(d2.train)+size(d3.train));

    test = [d1.test;d2.test;d3.test];
    success=0;
    for I=1:size(test,1)
        s=test(I,:);
        p_F_C1=(p_C1)*normpdf( s(1) ,m1(1),v1(1)^0.5)*normpdf( s(2) ,m1(2),v1(2)^0.5)*normpdf( s(3) ,m1(3),v1(3)^0.5)*normpdf( s(4) ,m1(4),v1(4)^0.5);
        p_F_C2=(p_C2)*normpdf( s(1) ,m2(1),v2(1)^0.5)*normpdf( s(2) ,m2(2),v2(2)^0.5)*normpdf( s(3) ,m2(3),v2(3)^0.5)*normpdf( s(4) ,m2(4),v2(4)^0.5);
        p_F_C3=(p_C3)*normpdf( s(1) ,m3(1),v3(1)^0.5)*normpdf( s(2) ,m3(2),v3(2)^0.5)*normpdf( s(3) ,m3(3),v3(3)^0.5)*normpdf( s(4) ,m3(4),v3(4)^0.5);

        p_F=p_F_C1+p_F_C2+p_F_C3;
        p_F_C1=p_F_C1/p_F;
        p_F_C2=p_F_C2/p_F;
        p_F_C3=p_F_C3/p_F;

        xxx=[p_F_C1, p_F_C2, p_F_C3];
        if find(xxx==max(xxx))==s(5)
            success = success + 1;
        end
    end
    performance = (success/size(test,1))*100;
return


function d = splitData(data,r)
    d.train=[];
    d.test=[];
    nTrain = r*size(data,1);
    while size(d.train,1) < nTrain
        I=floor(rand(1)*size(data,1))+1;
        d.train = [d.train;data(I,:)];
        data = [data(1:I-1,:);data(I+1:size(data,1),:)];
    end
    d.test=data;
    
return




