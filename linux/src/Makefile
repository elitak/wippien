CPLUS=g++
CC=gcc
CFLAGS=-Wall -fexceptions -D_UNIXVPN -D_LIB -O2
LDFLAGS=-l pthread libstrophe.a libexpat.a -l crypto unixVPN.a
SOURCESSRV=wiptun.c
SOURCESCLN=clnt.cpp buffer.c
EXECUTABLESRV=../bin/wiptun
EXECUTABLECLN=../bin/wipclnt

all: $(EXECUTABLESRV) $(EXECUTABLECLN)
	echo "all done"

$(EXECUTABLESRV): $(SOURCESRV) 
	$(CC) $(CFLAGS) $(SOURCESSRV) -o $(EXECUTABLESRV)
	strip $(EXECUTABLESRV)
	
$(EXECUTABLECLN): $(OBJECTSCLN) 
	$(CPLUS) $(CFLAGS) $(SOURCESCLN) $(LDFLAGS) -o $(EXECUTABLECLN)
	strip $(EXECUTABLECLN)
	

clean:
	rm $(EXECUTABLESRV) $(EXECUTABLECLN) *.o