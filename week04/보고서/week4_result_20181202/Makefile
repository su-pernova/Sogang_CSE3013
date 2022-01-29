# --- 1. 환경변수 선언 ---
.SURFFIXES : .cpp .o
CXX = g++
CPPFLAGS = -g

HEDRS = LinkedList.h Stack.h
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = a.out

# --- 2. 수행할 명령(호출 등) 정의 ---
all : $(TARGET)

$(TARGET) : $(OBJS) $(HDRS)
	$(CXX) -o $@ $(OBJS)

# --- 3. make clean 정의 ---
clean :
	rm -rf $(OBJS) $(TARGET)	
