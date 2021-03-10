curl https://raw.githubusercontent.com/arun6582/public/master/cpu-benchmark/cpu-test --output cpu-test
chmod +x cpu-test
echo 'Running test'
time ./cpu-test 100 1000000
echo 'Running 10 * test'
time ./cpu-test 100 10000000
rm -rf ./cpu-test
