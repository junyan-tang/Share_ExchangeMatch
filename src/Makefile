CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
LDFLAGS = -lpthread -lpqxx -lpugixml

# 添加 market.cpp 到编译列表中
MAIN_SRCS = main.cpp database.cpp server.cpp parser.cpp business.cpp global_var.cpp market.cpp
CLIENT_SRCS = client.cpp database.cpp

TARGETS = main client

all: $(TARGETS)

main: $(MAIN_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
