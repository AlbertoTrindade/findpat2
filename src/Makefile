MYPROGRAM = findpat2

SOURCES = main.cpp IndexProcessor.cpp SearchProcessor.cpp SuffixArrayIndexer.cpp LZ78Compressor.cpp
INCLUDES = ../libs/
TARGET_DIR = ../bin/

CC = g++
CFLAGS = -std=c++0x -Wall -g

MKDIR = mkdir -p
RM = rm -r

#---------------------------------------------

all:
	$(MKDIR) $(TARGET_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -I $(INCLUDES)  -Wall -o $(TARGET_DIR)$(MYPROGRAM)

clean:
	$(RM) $(TARGET_DIR)