#! /bin/bash

TEST_ORDER=test3.xml
TEST_QUERY=test4.xml

REQUESTS_NUM=10

./client test1.xml
./client test2.xml

for ((i = 0; i < $REQUESTS_NUM; ++i))
do
    echo "Request $i:"
    ./client $TEST_ORDER &  
    ./client $TEST_QUERY & 
     

done

wait 

echo "All requests completed."
