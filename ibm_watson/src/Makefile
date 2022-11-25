BIN:=watson_datasender
SRC:=$(wildcard *.c)
LDFLAGS:=-liotp-as-device -lblobmsg_json -lubox -lubus -largp
OBJS:=$(SRC:.c=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS)

clean: 
		$(RM) $(BIN)
