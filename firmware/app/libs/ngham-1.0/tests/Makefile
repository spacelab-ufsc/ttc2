TARGET=ngham_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../include
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC)

.PHONY: all
all: $(BUILD_DIR)/ccsds_scrambler.o $(BUILD_DIR)/crc_ccitt.o $(BUILD_DIR)/ngham.o $(BUILD_DIR)/test.o
	$(CC) $(FLAGS) $(BUILD_DIR)/ccsds_scrambler.o $(BUILD_DIR)/crc_ccitt.o $(BUILD_DIR)/ngham.o $(BUILD_DIR)/test.o -o $(BUILD_DIR)/$(TARGET) -lcmocka -lrsc

$(BUILD_DIR)/ccsds_scrambler.o: ../src/ccsds_scrambler.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/crc_ccitt.o: ../src/crc_ccitt.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/ngham.o: ../src/ngham.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/test.o: test.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/*.o
