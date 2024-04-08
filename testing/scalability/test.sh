#! /bin/bash

TEST_ORDER=test2.xml
TEST_QUERY=test3.xml

REQUESTS_NUM=1

./client test1.xml

for ((i = 0; i < $REQUESTS_NUM; ++i))
do
	./client $TEST_ORDER &
	./client $TEST_QUERY &
done
