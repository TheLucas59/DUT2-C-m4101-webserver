CC=gcc
LD=gcc
CFLAGS=-Wall -W -Werror
LDFLAGS=-lhttp_parse -L ../parselib -lpthread

EXE=pawnee

HEADERS=$(wildcard *.h)
CFILES=$(wildcard *.c)

OBJS=$(CFILES:.c=.o)

.PHONY: all clean mrproper

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

makefile.dep: $(CFILES) $(HEADERS)
	$(CC) -MM $(CFILES) > $@

clean:
	$(RM) $(OBJS) makefile.dep

mrproper: clean
	$(RM) $(EXE)

include makefile.dep
