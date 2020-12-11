CC=gcc
CFLAGS=-g -std=c11 -Iinclude
LIBS=-lm
TARGET= ami

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
_DEPS = bitmap.h annotate.h conversion.h 
_OBJS = ami.o bitmap.o annotate.o conversion.o 

DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET)
