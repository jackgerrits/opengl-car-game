PLATFORM := $(shell uname)

# Use pkg-config to get the right libraries for your platform
GL_LIBS = `pkg-config --static --libs glfw3` -lGLEW
EXT =
DEFS = `pkg-config --cflags glfw3`

# Any other platform specific libraries here...
ifneq (, $(findstring CYGWIN, $(PLATFORM)))
    GL_LIBS = -lopengl32 -lglfw3 -lglew32
	EXT = .exe
    DEFS =-DWIN32
endif

EXE=driver$(EXT)

DEFS +=-std=c++0x -Wall -Wextra

FILES += $(wildcard *.cpp)
FILES += $(wildcard shaders/*.cpp)
FILES += $(wildcard particles/*.cpp)
FILES += $(wildcard entities/*.cpp)
FILES += $(wildcard libs/*.cpp)
FILES += $(wildcard renderers/*.cpp)
FILES += $(wildcard water/*.cpp)
LINK = $(notdir $(subst .cpp,.o, $(FILES)))

.PHONY:  clean

all : $(EXE)

$(EXE) : $(LINK)
	g++ $(DEFS) -o $(EXE) $(LINK) $(GL_LIBS)

main.o : main.cpp
	g++ $(DEFS) -c main.cpp

%.o : %.cpp %.h
	g++ $(DEFS) -c $<

%.o : shaders/%.cpp shaders/%.h
	g++ $(DEFS) -c $<

%.o : entities/%.cpp entities/%.h
	g++ $(DEFS) -c $<

%.o : particles/%.cpp particles/%.h
	g++ $(DEFS) -c $<

%.o : renderers/%.cpp renderers/%.h
	g++ $(DEFS) -c $<

%.o : water/%.cpp water/%.h
	g++ $(DEFS) -c $<

%.o : libs/%.cpp libs/%.h
	g++ $(DEFS) -c $<

clean:
	rm -f *.o $(EXE)
