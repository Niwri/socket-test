SRC_DIR := ./src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR := ./obj
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -lwsock32
TARGET := socket.exe
CFLAGS := -Wall -g

all: $(TARGET)
$(TARGET): $(OBJ_FILES)
	g++ $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CFLAGS) -c $< -o $@ 
