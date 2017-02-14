# Name: Gonul AYCI
# Date: Bogazici University, Feb. 2017
# Description: Bayesian Network

def bn(Atotal, Btotal, AB):

  total = Atotal + Btotal
  P_A = (Atotal*1.0)/total
  P_B = ((Btotal)*1.0)/total
  P_AB = AB*1.0/Btotal
  P_BA = (P_B*P_AB)/(P_A*1.0)

  '''
  Bayes Theorem
  p(lamda/D) = p(D, lamda)/p(D) 
  			 = (p(D/lamda)*p(lamda))/p(D)

  params: D: model, and lamda
  p(lamda): prior probability
  p(D/lamda): likelihood
  p(D): prior probability
  
  '''

  print "posterior probability: ", P_BA
  
bn(10, 10, 5)