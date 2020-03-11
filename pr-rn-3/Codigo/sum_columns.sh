awk '{for (i=1;i<=NF;i++) sum[i]+=$i;};
 		END{for (i in sum) print i, sum[i];}' spikes.dat > sum_spikes.dat
