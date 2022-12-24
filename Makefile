##
# JSON thingy
#
# @file
# @version 0.1

CC			:= gcc
RMDIR		:= rm -rf
RM			:= rm -f
MKDIR		:= mkdir

SRC			:= ./src
OBJ			:= ./obj
BIN			:= ./bin
INCLUDE		:= ./include

SRCS		:= $(wildcard $(SRC)/*.c)
OBJS		:= $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
TARGET		:= $(BIN)/json_thingy

CFLAGS		:= -I$(INCLUDE)
DEBUGFLAGS	:= -O0 -ggdb

.PHONY: all clean run debug

run: all
	./$(TARGET)

debug: CFLAGS := $(CFLAGS) $(DEBUGFLAGS)
debug: all

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ) $(BIN):
	$(MKDIR) $@

clean:
	$(RMDIR) $(BIN) $(OBJ)

# end
