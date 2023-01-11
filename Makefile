# make && ./main
# source files list

SRC = $(wildcard *.cpp)


OBJ = $(addprefix build/,$(SRC:.cpp=.o))
DEP = $(addprefix build/,$(SRC:.cpp=.d))

# compilator name
CC = g++

# executable name
EXE = main

# compilation flages (CFLAGS) and links edition (LDFLAGS)
CFLAGS += -Wall -g `sdl2-config --cflags`
LDFLAGS = -lm -lSDL2_ttf `sdl2-config --libs`

# principal rule : make the executable file
all: $(OBJ)
	$(CC) -o $(EXE) $^ $(LDFLAGS)

# standart rule to make a .o file from a .c file
build/%.o: %.cpp
	@mkdir -p build
	$(CC) -c $(CFLAGS) -o $@ -cpp $<

# standart rule to clean
clean:
	rm -rf build core *.gch

# automatic inclution of the dependencies
-include $(DEP)