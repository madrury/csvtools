SRCDIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SRCDIR/test_functions.sh

echo "Running csv-chdelim tests..."

# Basic functionality
EXPECTED=$(printf "int1|int2|int3\n1|2|3\n1||3\n1|2|\n||3\n||")
GOT=$(csv-chdelim -f data/empty_field_test.csv -d ',' -n '|')
test_equal "$EXPECTED" "$GOT"

# Quoting awareness
EXPECTED=$(printf "1|\"2,2\"|1\n2|\"3,3\"|2\n3|\"4,4\"|3")
GOT=$(csv-chdelim -f data/ambiguous_quoting.csv -d ',' -n '|')
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "1|\"2|2\"|1\n2|\"3|3\"|2\n3|\"4|4\"|3")
GOT=$(csv-chdelim -f data/ambiguous_quoting.csv -d ',' -n '|' -q 'x')
test_equal "$EXPECTED" "$GOT"
