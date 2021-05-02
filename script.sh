for i in {007..014}; 
do
  echo "Testing $i";
  time ./main < Instances/instance$i.gr 2> Logs/instance$i.out
  echo "Done $i"
done
