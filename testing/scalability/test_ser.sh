#!/bin/bash

TEST_ORDER=test2.xml
TEST_QUERY=test3.xml

REQUESTS_NUM=100

./client test1.xml

for ((i = 0; i < REQUESTS_NUM; ++i))
do
    echo "Order Request $i:"
    ./client $TEST_ORDER
    echo "Query Request $i:"
    ./client $TEST_QUERY
done

echo "All requests completed."
