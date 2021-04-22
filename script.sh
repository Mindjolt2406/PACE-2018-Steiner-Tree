for i in {001..200}; 
do
  echo "Testing $i";
  time ./main < Instances/instance$i.gr 2> Logs/instance$i.out
  echo "Done $i"
done
