CC = g++
CFLAGS = -lpthread -lm
TARGET = cse4001_sync
SOURCES = cse4001_sync.cpp
HEADERS = semaphore_class.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
