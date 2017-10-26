MYDEF = 
OBJ1 = main.o FPGrowth.o FPTree.o
OBJ2 = FPTree.o FPGrowth.o
EXEC = asso


CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror $(MYDEF)


all: $(EXEC)

$(EXEC): $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ1)

$(OBJ1): FPGrowth.h FPTree.h
$(OBJ2): FPTree.h

.PHONY: clean

clean:
	rm -f *.o $(EXEC)
