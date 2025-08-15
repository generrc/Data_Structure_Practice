CC = gcc
CFLAGS = -Wall -g
TARGET = main
SOURCES = main.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET) graph1.txt graph2.txt graph3.txt graph4.txt

clean:
	rm -f $(TARGET)
