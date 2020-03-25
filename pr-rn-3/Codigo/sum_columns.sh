awk 'function floor(x){y=int(x); return(x<y?y-1:y)} {for (i=1; i<=NF; i++)  sum[floor(i/100)+1]+= $i/1.0;}
		END{for (i=1; i<length(sum); i++) print (i )*10, sum[i]/100;}'  	spikes.dat > sum_spikes.dat




# Para 1
awk '{if(j<1) for (i=1;i<=NF;i++) sum[i]+=$i; j=NR}
		END{	N=length(sum);	for (i=1; i<N; i=i+1) 
		print ( i )/1, (sum[i]);}'  	spikes.dat > sum_spikes_por_celda.dat


# Para 2
#		END{	N=length(sum);	for (i=2; i<N; i=i+2) 
#		print ( i + i  +1+)/2, (sum[i] + sum[i+1]);}'  	spikes.dat > sum_spikes.dat


#Para 4
#		END{	N=length(sum);	for (i=2; i<N; i=i+4) 
# 		print ( i + i + i + i  +1+2+3)/4, (sum[i] + sum[i+1] + sum[i+2] + sum[i+3] );}'  	spikes.dat > sum_spikes.dat



#Para 10
#		END{	N=length(sum);	for (i=2; i<N; i=i+10) 
# 		print ( 10*i + 45)/10, (sum[i] + sum[i+1] + sum[i+2] + sum[i+3] +sum[i+4] +sum[i+5] +sum[i+6] +sum[i+7] +sum[i+8] +sum[i+9]);}'  	spikes.dat > sum_spikes.dat


## Para el archivo stimulus
#media=$(awk 'BEGIN {counter=0; sum=0;sum_2=0;} {counter++; sum+=$2; sum_2+=$2*$2;} END {print sum_2/counter - (sum/counter)**2}' stimulus.dat)
#echo "$media"
#Total=$(awk 'BEGIN {counter=0} {counter=counter+1} END {print counter}'  $file_utctprh) 
##Sum of all events

#
#awk '
#{ 
#    for (i=1; i<=NF; i++)  {
#        a[NR,i] = $i
#    }
#}
#NF>p { p = NF }
#END {    
#    for(j=1; j<=p; j++) {
#        str=a[1,j]
#        for(i=2; i<=NR; i++){
#            str=str" "a[i,j];
#        }
#        print str
#    }
#}' spikes.dat > trans_spikes.dat#