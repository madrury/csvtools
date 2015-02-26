test_equal() {
    if [ "$1" == "$2" ]; then
        echo "Passed."
    else
        echo "Failed. Expected: $1 Got: $2"
    fi
}
