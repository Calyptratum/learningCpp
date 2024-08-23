CXX = g++
CXXFLAGS = -Wall -std=c++11 -g

# 源文件
SRCS1 = SearchPre.cc test.cc DictProducer.cc SplitTool.cc CutEnglishWord.cc

# 目标文件
OBJS1 = $(SRCS1:.cc=.o)
#OBJS2 = $(SRCS2:.cc=.o)

# 可执行文件名
TARGET1 = searchEngine
#TARGET2 = createDict

# 默认目标
all: $(TARGET1) 

# 链接目标文件生成第一个可执行文件
$(TARGET1): $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 链接目标文件生成第二个可执行文件
#$(TARGET2): $(OBJS2)
#	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译每个源文件为目标文件
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS1) $(OBJS2) $(TARGET1) $(TARGET2)

# 伪目标
.PHONY: all clean
