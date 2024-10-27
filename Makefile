CC = gcc
LD = ld
FLAGS = -Wall -O3 -lreadline
RM = rm -f
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

KCONFIG_DIR := .
KCONFIG_CONFIG := .config

all: simulator

simulator: $(BUILD_DIR)/main.o $(BUILD_DIR)/simulator.o \
            $(BUILD_DIR)/instruction.o $(BUILD_DIR)/memory.o \
            $(BUILD_DIR)/system.o $(BUILD_DIR)/log.o $(BUILD_DIR)/shell.o
	$(CC) $(FLAGS) -o $@ $^

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(BUILD_DIR)/simulator.o: $(SRC_DIR)/simulator.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(BUILD_DIR)/instruction.o: $(SRC_DIR)/instruction.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(BUILD_DIR)/memory.o: $(SRC_DIR)/memory.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(BUILD_DIR)/system.o: $(SRC_DIR)/system.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(BUILD_DIR)/log.o: $(SRC_DIR)/log.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

$(BUILD_DIR)/shell.o: $(SRC_DIR)/shell.c buildFolder
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

menuconfig:
	$(MAKE) -C $(KCONFIG_DIR) menuconfig

oldconfig:
	$(MAKE) -C $(KCONFIG_DIR) oldconfig

buildFolder:
	mkdir -p $(BUILD_DIR)

clean:
	$(RM) simulator $(BUILD_DIR)/*.o

.PHONY: all clean
