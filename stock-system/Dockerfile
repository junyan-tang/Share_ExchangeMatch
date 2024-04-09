FROM ubuntu:latest
RUN mkdir /code
RUN apt-get update && apt-get install -y make && apt-get install -y g++
RUN apt-get update && apt-get install -y sudo
RUN apt-get update && apt-get install -y libpqxx-dev
RUN apt-get update && apt-get install -y libpugixml-dev
RUN apt-get update && apt-get install -y libicu-dev
RUN apt-get update && apt-get install -y libbz2-dev
RUN apt-get update && apt-get install -y postgresql-client
COPY wait-for-db.sh /usr/local/bin/wait-for-db.sh
COPY wait-for-db.sh /usr/local/bin/wait-for-db.sh
RUN chmod +x /usr/local/bin/wait-for-db.sh
WORKDIR /code
ADD . /code