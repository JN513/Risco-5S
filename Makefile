CC = gcc
LD = ld
FLAGS = -Wall -std=c17 -O3
RM = rm -f
SRC_DIR = src
BUILD_DIR = build

KCONFIG_DIR := .
KCONFIG_CONFIG := .config

all: simulator

simulator: $(BUILD_DIR)/main.o $(BUILD_DIR)/simulator.o \
            $(BUILD_DIR)/instruction.o $(BUILD_DIR)/memory.o \
            $(BUILD_DIR)/system.o
	$(CC) -o $@ $^

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/simulator.o: $(SRC_DIR)/simulator.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/instruction.o: $(SRC_DIR)/instruction.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/memory.o: $(SRC_DIR)/memory.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/system.o: $(SRC_DIR)/system.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@


menuconfig:
	$(MAKE) -C $(KCONFIG_DIR) menuconfig

oldconfig:
	$(MAKE) -C $(KCONFIG_DIR) oldconfig

buildFolder:
	mkdir -p $(BUILD_DIR)

clean:
	$(RM) simulator $(BUILD_DIR)/*.o

.PHONY: all clean
