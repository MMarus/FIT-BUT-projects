#  Author : Marek Marusic, xmarus05
#  Email:  xmarus05@stud.fit.vutbr.cz
#  purpose: Projekt1 to IPK
#  description : Create client-server protokol
#  file: Makefile
#
SERVER          = server
CLIENT          = client
SERVER_SOURCES  = server.cpp
CLIENT_SOURCES  = client.cpp

CFLAGS         = -g -std=c++11

LIBRARIES       = -static-libstdc++

CC              = g++
SERVER_OBJECTS  = $(SERVER_SOURCES:.cpp=.o)
CLIENT_OBJECTS  = $(CLIENT_SOURCES:.cpp=.o)
INCLUDES        = #-I.
LIBDIRS         = 
LDFLAGS         = $(LIBDIRS) $(LIBRARIES)

###########################################

.SUFFIXES: .cpp .o

.cpp.o:
		$(CC) $(CFLAGS) -c $<

###########################################

all:		$(SERVER) $(CLIENT)

rebuild:	clean all

$(SERVER):	$(SERVER_OBJECTS)
		$(CC) $(SERVER_OBJECTS) $(LDFLAGS) -o $@

$(CLIENT):	$(CLIENT_OBJECTS)
		$(CC) $(CLIENT_OBJECTS) $(LDFLAGS) -o $@

###########################################

clean:
	rm -fr core* *~ $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(SERVER) $(CLIENT) .make.state .sb

