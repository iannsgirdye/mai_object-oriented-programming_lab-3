# Переменные
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14
LDFLAGS = 

# Список исходных файлов
SRCS := $(wildcard src/*.cpp) $(wildcard src/figures/*.cpp) $(wildcard src/array/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Целевая программа
TARGET = main

# Правила сборки
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

# Псевдоним для сборки
build: $(TARGET)
