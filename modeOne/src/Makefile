CXX = g++
CXXFLAGS = -Wall -std=c++11 -g

# 源文件
SRCS = SearchPre.cc test.cc DictProducer.cc SplitTool.cc CutEnglishWord.cc
# 目标文件
OBJS = $(SRCS:.cc=.o)
# 可执行文件名
TARGET = searchEngine

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译每个源文件为目标文件
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标
.PHONY: all clean
