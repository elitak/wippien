CPLUS=g++
CC=gcc
CFLAGS=-Wall -fexceptions -D_UNIXVPN -D_LIB -O2
LDFLAGS=-l pthread -l crypto lib/libstrophe.a lib/libexpat.a lib/unixVPN.a
SOURCESSRV=src/wiptun.c
SOURCESCLN=src/clnt.cpp src/buffer.c
EXECUTABLESRV=bin/wiptun
EXECUTABLECLN=bin/wipclnt

all: $(EXECUTABLESRV) $(EXECUTABLECLN)
	@echo "all done"

$(EXECUTABLESRV): $(SOURCESRV) 
	$(CC) $(CFLAGS) $(SOURCESSRV) -o $(EXECUTABLESRV)
	strip $(EXECUTABLESRV)
	
$(EXECUTABLECLN): $(OBJECTSCLN) 
	$(CPLUS) $(CFLAGS) $(SOURCESCLN) $(LDFLAGS) -o $(EXECUTABLECLN)
	strip $(EXECUTABLECLN)
	

clean:
	rm $(EXECUTABLESRV) $(EXECUTABLECLN) *.o