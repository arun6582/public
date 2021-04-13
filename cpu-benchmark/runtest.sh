curl https://raw.githubusercontent.com/arun6582/public/master/cpu-benchmark/cpu-test --output cpu-test
chmod +x cpu-test
echo "Running test $2 times"
time ./cpu-test $1 $2
echo "Running test $(($2 * 10)) times"
time ./cpu-test $1 $(($2 * 10))
rm -rf ./cpu-test
