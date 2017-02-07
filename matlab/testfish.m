%name : Gonul AYCI
%e-mail : aycignl@gmail.com

function test_fish

clc;

%Prepare a dataset
sample_size = 10;
n_hamsi = sample_size;
n_istavrit = sample_size;
n_uskumru = sample_size;

data_hamsi=4 + rand(1,n_hamsi)* (7-4); % samples from hamsi population (length in cm)
data_istavrit= 7 + rand(1,n_istavrit)* (10-5); % samples from istavrit population (length in cm)
data_uskumru= 9 + rand(1,n_uskumru)* (18-5); % samples from uskumru population (length in cm)

% Build your model for classes
p_h = length(data_hamsi) / (length(data_hamsi) +length(data_istavrit) +length(data_uskumru));
p_i = length(data_istavrit) / (length(data_hamsi) +length(data_istavrit) +length(data_uskumru));
p_u = length(data_uskumru) / (length(data_hamsi) +length(data_istavrit) +length(data_uskumru));

h_m = mean(data_hamsi);
h_var = var(data_hamsi);

i_m = mean(data_istavrit);
i_var = var(data_istavrit);

u_m = mean(data_uskumru);
u_var = var(data_uskumru);

% Use your model, for different lengths of fishes
for L = 4:18
    p_l_h = normpdf(L,h_m, h_var^0.5 );
    p_l_i = normpdf(L,i_m, i_var^0.5 );
    p_l_u = normpdf(L,u_m, u_var^0.5 );

    ps=[p_l_h*p_h,  p_l_i*p_i, p_l_u*p_u];

    ps = ps / sum(ps); 
    if( find(ps==max(ps)) == 1)
        disp(sprintf('hamsi if Length=%f with prob. %f',L,max(ps)));
    elseif( find(ps==max(ps)) == 2)
        disp(sprintf('istavrit if Length=%f with prob. %f',L,max(ps)));
    elseif( find(ps==max(ps)) == 3)
        disp(sprintf('uskumru if Length=%f with prob. %f',L,max(ps)));
    end
end

return

 
