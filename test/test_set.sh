SRCDIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SRCDIR/test_functions.sh

echo "Running csv-set tests..."

# Union
EXPECTED=$(printf "int1,int2\n0,0\n0,1\n1,0\n1,1")
GOT=$(csv-set -u data/set_test_1.csv data/set_test_2.csv)
test_equal "$EXPECTED" "$GOT"

# Intersection
EXPECTED=$(printf "int1,int2\n0,1\n1,0")
GOT=$(csv-set -i data/set_test_1.csv data/set_test_2.csv)
test_equal "$EXPECTED" "$GOT"

# Difference
EXPECTED=$(printf "int1,int2\n1,1")
GOT=$(csv-set -d data/set_test_1.csv data/set_test_2.csv)
test_equal "$EXPECTED" "$GOT"
