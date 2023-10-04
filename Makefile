SRV_SRC_DIR := ./server/src
SRV_SRC_FILES := $(wildcard $(SRV_SRC_DIR)/*.cpp)

SRV_OBJ_DIR := ./server/obj
SRV_OBJ_FILES := $(patsubst $(SRV_SRC_DIR)/%.cpp,$(SRV_OBJ_DIR)/%.o,$(SRV_SRC_FILES))

CLN_SRC_DIR := ./client/src
CLN_SRC_FILES := $(wildcard $(CLN_SRC_DIR)/*.cpp)

CLN_OBJ_DIR := ./client/obj
CLN_OBJ_FILES := $(patsubst $(CLN_SRC_DIR)/%.cpp,$(CLN_OBJ_DIR)/%.o,$(CLN_SRC_FILES))

LDFLAGS := -lwsock32
SRV_TARGET := socketServer.exe
CLN_TARGET := socketClient.exe
CFLAGS := -Wall -g

all: $(CLN_TARGET) $(SRV_TARGET)

$(CLN_TARGET): $(CLN_OBJ_FILES)
	g++ $^ -o $@ $(LDFLAGS)

$(SRV_TARGET): $(SRV_OBJ_FILES)
	g++ $^ -o $@ $(LDFLAGS)

$(SRV_OBJ_DIR)/%.o: $(SRV_SRC_DIR)/%.cpp
	g++ $(CFLAGS) -c $< -o $@ 

$(CLN_OBJ_DIR)/%.o: $(CLN_SRC_DIR)/%.cpp
	g++ $(CFLAGS) -c $< -o $@ 
