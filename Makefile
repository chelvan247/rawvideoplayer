CC = g++
CFLAGS = -Wno-deprecated
LIBS = -lglut32 -lglu32 -lopengl32

SOURCES = rawvideoplayer.cpp yuvconverter.cpp videoout.cpp\
		  inittab.cpp yuv420planartorgba.cpp rgbtorgba.cpp\
		  yuv422packedtorgba.cpp yuv422planartorgba.cpp\
		  yuv444packedtorgba.cpp yuv444planartorgba.cpp
		 
EXEC = rawvideoplayer 
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXEC)

$(EXEC) : $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LIBS) -o $(EXEC)

%.o : %.cpp Makefile
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~ *.o *.exe





