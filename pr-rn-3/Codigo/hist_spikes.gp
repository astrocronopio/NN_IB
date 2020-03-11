binwidth=1
bin(x,width)=width*floor(x/width)

plot 'sum_spikes.dat' using (bin($2,binwidth)):(1.0) smooth freq with boxes