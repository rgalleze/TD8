#!/bin/bash

read -p "N : " N
read -p "Itérations : " iterations

for (( NUM_THREADS=1; NUM_THREADS<=7;NUM_THREADS++  ))
do
for (( i=1; i<=$iterations; i++ ))
do
  ./SOE_multithreading_opt1 $N $NUM_THREADS
done > time_exe.txt

sum=$(cat time_exe.txt | awk '{ sum += $1 } END { print sum }')
count=$(cat time_exe.txt | wc -l)
mean=$(echo "scale=7; $sum / $count" | bc)

echo "Mean exec ($iterations iterations), N = $N, $NUM_THREADS thread(s) --> $mean sec" >> output1.txt
echo "Analyse done !"
done
