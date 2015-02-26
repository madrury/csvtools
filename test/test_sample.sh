SRCDIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SRCDIR/test_functions.sh

echo "Running csv-sample tests..."

# Fixed number of lines
PATTERN=$(printf "int1,int2,int3\n.*\n.*")
GOT=$(csv-sample -f data/empty_field_test.csv -n 2)
test_matches "$GOT" "$PATTERN"

# Proporting of lines

# Rounds down, no lines
PATTERN=$(printf "int1,int2,int3\n")
GOT=$(csv-sample -f data/empty_field_test.csv -p .1)
test_matches "$GOT" "$PATTERN"

# Exactly one line
PATTERN=$(printf "int1,int2,int3\n.*")
GOT=$(csv-sample -f data/empty_field_test.csv -p .2)
test_matches "$GOT" "$PATTERN"

# Just less than two lines
PATTERN=$(printf "int1,int2,int3\n.*")
GOT=$(csv-sample -f data/empty_field_test.csv -p .39)
test_matches "$GOT" "$PATTERN"

# Exactly two lines
PATTERN=$(printf "int1,int2,int3\n.*\n.*")
GOT=$(csv-sample -f data/empty_field_test.csv -p .4)
test_matches "$GOT" "$PATTERN"

# Round down to two lines
PATTERN=$(printf "int1,int2,int3\n.*\n.*")
GOT=$(csv-sample -f data/empty_field_test.csv -p .41)
test_matches "$GOT" "$PATTERN"

# Repeatablility
EXPECTED=$(csv-sample -f data/empty_field_test.csv -s data/empty_field_test.csv -p .4)
GOT=$(csv-sample -f data/empty_field_test.csv -s data/empty_field_test.csv -p .4)
test_equal "$GOT" "$EXPECTED"

EXPECTED=$(csv-sample -f data/pipe_delimited.csv -s data/empty_field_test.csv -p .4)
GOT=$(csv-sample -f data/pipe_delimited.csv -s data/empty_field_test.csv -p .4)
test_equal "$GOT" "$EXPECTED"

EXPECTED=$(csv-sample -f data/pipe_delimited.csv -s data/pipe_delimited.csv -p .4)
GOT=$(csv-sample -f data/pipe_delimited.csv -s data/pipe_delimited.csv -p .4)
test_equal "$GOT" "$EXPECTED"
