CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
TARGET := electricity_bills
SOURCE := ELECTRIC.C

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -x c $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) current.txt
