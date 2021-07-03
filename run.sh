rm ./results/negamax_minmax.txt
rm ./results/negamax_alphabeta.txt
rm ./results/scout.txt
rm ./results/negascout.txt
./main 1 >> ./results/negamax_minmax.txt
./main 2 >> ./results/negamax_alphabeta.txt
./main 3 >> ./results/scout.txt
./main 4 >> ./results/negascout.txt