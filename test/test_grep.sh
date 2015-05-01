SRCDIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SRCDIR/test_functions.sh

echo "Running csv-grep tests..."

# Basic functionality
EXPECTED=$(printf "int1,int2,int3\n1,2,3\n1,,3\n1,2,")
GOT=$(csv-grep -f data/empty_field_test.csv -c int1 -p "1")
test_equal "$EXPECTED" "$GOT"

# Test for empty field match
EXPECTED=$(printf "int1,int2,int3\n,,3\n,,")
GOT=$(csv-grep -f data/empty_field_test.csv -c int1 -p "^$")
test_equal "$EXPECTED" "$GOT"

# Test for exact match
EXPECTED=$(printf "num,greeting,int\n1,\"Hello.\",1\n0,\"Hello. Dave.\",3")
GOT=$(csv-grep -f data/quoting_test.csv -c greeting -p "Hello\.")
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "num,greeting,int\n1,\"Hello.\",1")
GOT=$(csv-grep -f data/quoting_test.csv -c greeting -p "^Hello\.$")
test_equal "$EXPECTED" "$GOT"

# Pipe delimited
EXPECTED=$(printf "int1|int2|int3\n1|2|3\n3|2|1")
GOT=$(csv-grep -f data/pipe_delimited.csv -d "|" -c int2 -p "2")
test_equal "$EXPECTED" "$GOT"

# Match an or
EXPECTED=$(printf "int1|int2|int3\n1|2|3\n3|2|1")
GOT=$(csv-grep -f data/pipe_delimited.csv -d "|" -c int1 -p "^1$\|^3$")
test_equal "$EXPECTED" "$GOT"

# Reverse a match
EXPECTED=$(printf "int1|int2|int3\n-1|0|-1")
GOT=$(csv-grep -f data/pipe_delimited.csv -d "|" -v -c int1 -p "^1$\|^3$")
test_equal "$EXPECTED" "$GOT"

# Changing the quote qhar
EXPECTED=$(printf "2,\"3,3\",2")
GOT=$(csv-grep -r /dev/null -f data/ambiguous_quoting.csv -c 2 -p "2")
test_equal "$EXPECTED" "$GOT"

# Changing the quote qhar
EXPECTED=$(printf "1,\"2,2\",1")
GOT=$(csv-grep -r /dev/null -f data/ambiguous_quoting.csv -q "x" -c 2 -p "2")
test_equal "$EXPECTED" "$GOT"
