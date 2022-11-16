BIN:=watson
SRC:=$(wildcard *.c)
LDFLAGS:=-liotp-as-device -lblobmsg_json -lubox -lubus
OBJS:=$(SRC:.c=.o)

.PHONY all clean

all: $(BIN)

$(BIN): $(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS)

clean: 
		$(RM) $(BIN)