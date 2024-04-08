#! /bin/bash

TEST_ORDER = test2.xml
TEST_QUERY = test3.xml

REQUESTS_NUM=0

./client test1.xml

for ((i = 0; i < $NUM_REQUESTS; ++i))
do
	./test $TEST_ORDER &
	./test $TEST_QUERY &
done