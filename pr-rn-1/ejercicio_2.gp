set terminal qt  2 size 1000,750 enhanced font 'Verdana,26'

j(ai, ao, q, x) = rho*q*x*((ao-ai*e**(q*x))/(1-e**(q*x)))

ai= 430 
ao= 20 
q=1

rho=0.01
e=2.7182818284

set key left top


set xlabel "{/Symbol a}"

set ylabel "j_A / {/Symbol r}_A"


plot j(430, 20, 1, x) lw 3 t "K^+"
replot j(50, 440, 1, x) lw 3 t "Na^+"
replot j(65, 550, -1, x) lw 3 t "Cl^-"
