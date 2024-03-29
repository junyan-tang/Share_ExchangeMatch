CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
LDFLAGS = -lpthread -lpqxx -lpugixml # 根据需要添加其他库

MAIN_SRCS = main.cpp server.cpp database.cpp #helper.cpp
CLIENT_SRCS = client.cpp  database.cpp #helper.cpp

TARGETS = main client

all: $(TARGETS)

main: $(MAIN_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean
