SRCDIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

test_equal() {
    if [ "$1" == "$2" ]; then
        echo "Passed."
    else
        echo "Failed. Expected: $1 Got: $2"
    fi
}

# Basic functionality
EXPECTED=$(printf "int1\n1\n1\n1\n\n")
GOT=$(csv-cut -f data/empty_field_test.csv int1)
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "int1,int2\n1,2\n1,\n1,2\n,\n,")
GOT=$(csv-cut -f data/empty_field_test.csv int1 int2)
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "int1,int3\n1,3\n1,3\n1,\n,3\n,")
GOT=$(csv-cut -f data/empty_field_test.csv int1 int3)
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "num,int\n1,1\n0,2\n0,3\n1,4")
GOT=$(csv-cut -f data/quoting_test.csv num int)
test_equal "$EXPECTED" "$GOT"

# Integer indexing
EXPECTED=$(printf "int1,int2\n1,2\n1,\n1,2\n,\n,")
GOT=$(csv-cut -f data/empty_field_test.csv 0 1)
test_equal "$EXPECTED" "$GOT"

# No header in file
EXPECTED=$(printf "1\n1\n1\n\n")
GOT=$(csv-cut -R -f data/empty_field_test.csv int1)
test_equal "$EXPECTED" "$GOT"

# Quoted field
EXPECTED=$(printf "greeting\n\"Hello.\"\n\"Goodbye.\"\n\"Hello, Dave.\"\n\"Hello, \\\\\"Matt\\\\\", if that is your real name!\"")
GOT=$(csv-cut -f data/quoting_test.csv greeting) 
test_equal "$EXPECTED" "$GOT"

# Pipe delimited
EXPECTED=$(printf "int1|int2\n1|2\n3|2\n-1|0")
GOT=$(csv-cut -d '|' -f data/pipe_delimited.csv int1 int2)
test_equal "$EXPECTED" "$GOT"

# Ambigious quoting
EXPECTED=$(printf "1,1\n2,2\n3,3")
GOT=$(csv-cut -f data/ambiguous_quoting.csv 0 2)
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "\"2,2\"\n\"3,3\"\n\"4,4\"")
GOT=$(csv-cut -f data/ambiguous_quoting.csv 1)
test_equal "$EXPECTED" "$GOT"

EXPECTED=$(printf "1,2\"\n2,3\"\n3,4\"")
GOT=$(csv-cut -q 'x' -f data/ambiguous_quoting.csv 0 2)
test_equal "$EXPECTED" "$GOT"
