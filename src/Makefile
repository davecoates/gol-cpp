SRCS = main.cpp rle.cpp
OBJS = $(SRCS:%.cpp=../obj/%.o)
LDFLAGS =-lsfml-graphics -lsfml-window -lsfml-system
CXXFLAGS = -c -Wall -std=c++11

TARGET = ../bin/gol

$(TARGET): dirs $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $@

../obj/%.o: %.cpp
	g++ $(CXXFLAGS) $< -o $@

play: $(TARGET)
	$(TARGET)

.PHONY: dirs
dirs:
	@mkdir -p ../obj ../bin

.PHONY: clean
clean:
	-rm -r ../obj ../bin
