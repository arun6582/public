file=/tmp/cpu-test
curl https://raw.githubusercontent.com/arun6582/public/master/cpu-benchmark/cpu-test --output $file
chmod +x $file
echo "Running test $2 times"
time $file $1 $2
echo "Running test $(($2 * 10)) times"
time $file $1 $(($2 * 10))
rm -rf $file
