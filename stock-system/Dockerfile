FROM ubuntu:latest
RUN mkdir /code
RUN apt-get update
RUN apt-get install -y g++ make
RUN apt-get install -y libpugixml-dev
WORKDIR /code
ADD . /code