#/bin/sh
echo '' > results
for i in 2 3 4 5 6 7  8 9 10 11 12 13 14 15 16 17 18 19 20 #21 22 23 24 25 26 27 28 29 30
do
	for j in 2 3 4 5 6 7 8 9 10 #11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
	do
		mpirun -np $j bin/GaussJacobiDiff data/data$i >> results
	done
done
cat results | grep 'Metoda'

