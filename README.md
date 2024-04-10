# Exchange matching engine

## Overview of Implementation

- Use pugixml to parse and generate xml request/response
- Add thread pool to support multi-thread
- Implement atomic database operations under high concurrency
- With great exception handling
- Test scalability according to different CPU core counts

## How to run it

Set up docker and run it directly: 
```Bash
sudo docker-compose up
```
