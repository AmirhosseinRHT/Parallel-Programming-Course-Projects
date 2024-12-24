CXX = g++ -std=c++17
CXXFLAGS = -pthread -g -w

TARGETS = bakery.out chaosBakery.out

SRCS = utility.cpp queue.cpp customer.cpp baker.cpp oven.cpp deliverySpace.cpp
MAIN_SRCS = main.cpp
CHAOS_MAIN_SRCS = chaosMain.cpp

OBJ_DIR = objects
MAIN_OBJS = $(addprefix $(OBJ_DIR)/, $(MAIN_SRCS:.cpp=.o) $(SRCS:.cpp=.o))
CHAOS_MAIN_OBJS = $(addprefix $(OBJ_DIR)/, $(CHAOS_MAIN_SRCS:.cpp=.o) $(SRCS:.cpp=.o))

.PHONY: all clean

all: $(TARGETS)

bakery.out: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJS)

chaosBakery.out: $(CHAOS_MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(CHAOS_MAIN_OBJS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGETS)
	rm -rf $(OBJ_DIR)
