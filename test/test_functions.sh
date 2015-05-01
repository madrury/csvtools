test_equal() {
    if [[ "$1" == "$2" ]]; then
        echo "Passed."
    else
        echo "Failed. Expected: $1 Got: $2"
    fi
}

test_matches() {
    if [[ $1 =~ $2 ]]; then
        echo "Passed."
    else
        echo "Failed to match $1 to $2"
    fi
}
