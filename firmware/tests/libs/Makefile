TARGET_BUFFER=buffer_unit_test
TARGET_QUEUE=queue_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
FLAGS=-fpic -std=c99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -I$(INC)

BUFFER_TEST_FLAGS=$(FLAGS)

QUEUE_TEST_FLAGS=$(FLAGS)

.PHONY: all
all: buffer_test queue_test

.PHONY: buffer_test
buffer_test: $(BUILD_DIR)/buffer.o $(BUILD_DIR)/buffer_test.o
	$(CC) $(BUFFER_TEST_FLAGS) $(BUILD_DIR)/buffer.o $(BUILD_DIR)/buffer_test.o -o $(BUILD_DIR)/$(TARGET_BUFFER) -lcmocka

.PHONY: queue_test
queue_test: $(BUILD_DIR)/queue.o $(BUILD_DIR)/queue_test.o
	$(CC) $(QUEUE_TEST_FLAGS) $(BUILD_DIR)/queue.o $(BUILD_DIR)/queue_test.o -o $(BUILD_DIR)/$(TARGET_QUEUE) -lcmocka

# Libraries
$(BUILD_DIR)/buffer.o: ../../libs/containers/buffer.c
	$(CC) $(BUFFER_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/queue.o: ../../libs/containers/queue.c
	$(CC) $(QUEUE_TEST_FLAGS) -c $< -o $@

# Tests
$(BUILD_DIR)/buffer_test.o: buffer_test.c
	$(CC) $(BUFFER_TEST_FLAGS) -c $< -o $@

$(BUILD_DIR)/queue_test.o: queue_test.c
	$(CC) $(QUEUE_TEST_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_BUFFER) $(BUILD_DIR)/$(TARGET_QUEUE) $(BUILD_DIR)/*.o
