CXX = g++ -std=c++17
CXXFLAGS = -pthread -g -w

TARGETS = bakery.out chaosBakery.out

SRCS = utility.cpp queue.cpp customer.cpp baker.cpp oven.cpp deliverySpace.cpp

MAIN_SRCS = main.cpp
CHAOS_MAIN_SRCS = chaosMain.cpp

MAIN_OBJS = $(MAIN_SRCS:.cpp=.o) $(SRCS:.cpp=.o)
CHAOS_MAIN_OBJS = $(CHAOS_MAIN_SRCS:.cpp=.o) $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGETS)

bakery.out: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJS)

chaosBakery.out: $(CHAOS_MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(CHAOS_MAIN_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN_OBJS) $(CHAOS_MAIN_OBJS) $(TARGETS)