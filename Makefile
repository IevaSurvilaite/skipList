# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Targets
TARGET_DEMO = demo
TARGET_TEST = test
MODULE = libskiplist.a

# Source files
SRC = skipList.cpp
demo_SRC = demo.cpp
test_SRC = test.cpp

# Object files
OBJ = $(SRC:.cpp=.o)
demo_OBJ = $(demo_SRC:.cpp=.o)
test_OBJ = $(test_SRC:.cpp=.o)

# Build the SkipList module
$(MODULE): $(OBJ)
	ar rcs $@ $^

# Build the demo program
$(TARGET_DEMO): $(demo_OBJ) $(MODULE)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

# Build the test program
$(TARGET_TEST): $(test_OBJ) $(MODULE)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

# Run tests
run_tests: $(TARGET_TEST)
	./$(TARGET_TEST)

# Clean build files
clean:
	del /F /Q $(OBJ) $(demo_OBJ) $(test_OBJ) $(MODULE) $(TARGET_DEMO).exe $(TARGET_TEST).exe 2>nul

# Rebuild everything
rebuild: clean $(MODULE) $(TARGET_DEMO) $(TARGET_TEST)