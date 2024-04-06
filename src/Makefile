CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
LDFLAGS = -lpthread -lpqxx -lpugixml

MAIN_SRCS = service.cpp database.cpp server.cpp parser.cpp business.cpp global_var.cpp market.cpp
CLIENT_SRCS = client.cpp database.cpp

TARGETS = service client

all: $(TARGETS)

service: $(MAIN_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
