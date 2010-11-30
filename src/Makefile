CC = gcc
PROGS = kftserver kftclient 
BINPATH = ../bin

all: $(PROGS)

dirs:
	mkdir -p $(BINPATH)

debug:
	make DEBUG=-DDEBUG $(PROGS)

kftserver: kftserver.c dropper.c netio.c
	$(CC) $(DEBUG) -o $(BINPATH)/$@ $^

kftclient: kftclient.c dropper.c netio.c
	$(CC) $(DEBUG) -o $(BINPATH)/$@ $^

testserver: kftserver
	$(BINPATH)/kftserver -d 1234 90

testclient: kftclient
	$(BINPATH)/kftclient -d 127.0.0.1 1234 ../doc/README.tex testcl 1024 90

clean:
	rm -rf $(BINPATH)/*
