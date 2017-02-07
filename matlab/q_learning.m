clc
clear all

%name : Gonul AYCI
%e-mail : aycignl@gmail.com

%Homework_Reinforcement_Learning


%%%%%%%%%      The Q-Learning algorithm goes as follow:      %%%%%%%%%%%%%%
%1. Set the gamma parameter, and environment rewards in matrix R.
%2. Initialize matrix Q to zero.
%3. For each episode:
%Select a random initial state.
%Do While the goal state hasn't been reached.
%Select one among all possible actions for the current state.
%Using this possible action, consider going to the next state.
%Get maximum Q value for this next state based on all possible actions.
%Compute: Q(state, action) = R(state, action) + Gamma * Max[Q(next state, all actions)]
%Set the next state as the current state.
%End Do
%End For



%%Inputs
%%given to us:
%The discount factor determines the importance of future rewards:
gamma = 0.8;% discount factor  % TODO : we need learning rate schedule
N = 5;
learn_rate =0.1; % learning rate    % TODO : we need exploration rate schedule
%right:1,left:2,up:3,down:4;
number_of_action = 4;
exploration_rate=0.1;
state = [1:25];
action=[1:4];
epsilon = 0.9;  % exploration probability (1-epsilon = exploit / epsilon = explore)



%**********************************************

Q_matrix = zeros(length(state),length(action));
Q_matrix(N*N, 2) = -inf;   
Q_matrix(1, 2) = -inf;
Q_matrix(1:N, 4) = -inf;
Q_matrix(N*4+1 : N*N, 3) = -inf;

for i = 1 : N
    Q_matrix(i*N,2) = -inf;
    Q_matrix(i*N + 1, 1) = -inf;
end

Q_matrix = Q_matrix( 1:N*N, :);


%***********************************************

for episode = 1 : 5000 %number_of_iteration=5000;
    
disp(['iteration: ' num2str(episode)]);

  r = rand; % get 1 uniform random number
  
    x=sum(r>=cumsum([0, 1-epsilon, epsilon])); % check it to be in which probability area
    
    
%********Start state:right_down------>goal state: left_up**************
first = 1;
goal_state = N*N %=25;
path = [];

 state = first;
    while state ~= goal_state
        
       if rand < exploration_rate
          i=1:4;
          action =rand(Q_matrix(state,i));
       else
          [value, action] = max( Q_matrix(state, action) );
       end
       
        next_state =find(state);
        path(i)  = next_state;
        i = i + 1;

%*********************************
Maze=zeros(N,N);
for i=1:N
    for j=1:N
       if rand<.01*r
           Maze(i,j)=1;
       end
    end
end
maze=Maze;     
%*******************************
        if maze(next_state) == 1
            reward = -4;
        else
            reward = -1;
        end
      
        % update ,% : means :all actions,%%R(state, action)=reward;
        Q_matrix(state, action) = (1-learn_rate)*Q_matrix(state, action)+learn_rate*( reward + gamma * max(Q_matrix(next_state,:)));
      
        state = next_state ;

    end    
end




