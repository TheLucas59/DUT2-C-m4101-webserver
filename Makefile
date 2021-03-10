FOLDERS=parselib webserver

.PHONY: all $(FOLDERS)

all: $(FOLDERS)

webserver:
	make -C webserver

parselib:
	make -C parselib
