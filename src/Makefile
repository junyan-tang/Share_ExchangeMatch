# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
LDFLAGS = -lpthread -lpqxx -lpugixml # 根据需要添加其他库

# 源文件列表
MAIN_SRCS = main.cpp server.cpp #database.cpp helper.cpp server.cpp
CLIENT_SRCS = client.cpp #helper.cpp database.cpp

# 目标可执行文件
TARGETS = main client

# 默认目标
all: $(TARGETS)

# 编译并链接 main 程序
main: $(MAIN_SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# 编译并链接 client 程序
client: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# 清理目标，用于删除构建的产品
clean:
	rm -f $(TARGETS)

.PHONY: all clean
