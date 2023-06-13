#!/bin/bash

read -p "N : " N
read -p "Iterations : " iterations

for (( i=1; i<=$iterations; i++ ))
do
  ./SOE_sequential $N
done > time_exe2.txt

sum=$(cat time_exe2.txt | awk '{ sum += $1 } END { print sum }')
count=$(cat time_exe2.txt | wc -l)
mean=$(echo "scale=7; $sum / $count" | bc)

echo "Mean exec ($iterations iterations), N = $N --> $mean sec" >> output2.txt

