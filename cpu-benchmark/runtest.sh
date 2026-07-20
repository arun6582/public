file=/tmp/cpu-test
curl https://raw.githubusercontent.com/arun6582/public/master/cpu-benchmark/cpu-test --output $file
chmod +x $file

job() {
    echo "TEST - #$1"
    time $file $2 $3
}


set -eu

for i in {1..3}
do
  job $i $1 $2
done