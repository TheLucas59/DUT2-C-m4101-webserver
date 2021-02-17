CFLAGS=-Wall -Wextra -Werror -g
LIB=libhttp_parse.a

SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
HDR=$(wildcard *.h)

$(LIB): $(OBJS)
	ar rs $@ $^
	ranlib $@

makefile.dep: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -MM $(SRC) > $@

.PHONY: clean mrproper
clean:
	$(RM) $(OBJS)
mrproper: clean
	$(RM) $(LIB)

include makefile.dep