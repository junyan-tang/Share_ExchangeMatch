#! /bin/bash

TEST_ORDER=test2.xml
TEST_QUERY=test3.xml

REQUESTS_NUM=500

./client test1.xml

for ((i = 0; i < $REQUESTS_NUM; ++i))
do
    echo "Request $i:"
    ./client $TEST_ORDER &  
    ./client $TEST_QUERY & 
     

done

wait 

echo "All requests completed."
