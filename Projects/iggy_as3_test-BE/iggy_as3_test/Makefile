CC = gcc
CXX = g++

OUTPUT := iggy_as3_test.exe
OBJS=main.o debug.o Utils.o crypto/sha1.o crypto/md5.o crypto/rijndael.o 
OBJS += BaseFile.o Stream.o MemoryStream.o FixedMemoryStream.o BitStream.o FixedBitStream.o
OBJS += IggyFile.o SwfFile.o
OBJS += tinyxml/tinyxml.o tinyxml/tinystr.o tinyxml/tinyxmlerror.o tinyxml/tinyxmlparser.o
CFLAGS=-Wall -I. -I -std=gnu99 -mno-ms-bitfields -O2 -masm=intel 
CFLAGS += -static
CXXFLAGS=-Wall -I -O2 -std=c++11 -DTIXML_USE_STL -masm=intel -fpermissive
LDFLAGS=-L. 
LIBS =  -lz -lstdc++

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) *.o
	rm -f /*.o
	rm -f /tinyxml/*.o
	rm -f /crypto/*.o

$(OUTPUT): $(OBJS)
	$(LINK.c) $(LDFLAGS) -o $@ $^ $(LIBS)
