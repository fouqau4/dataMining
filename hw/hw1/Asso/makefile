#CUSTOMDEFINES = -DEBUG
OBJ1 = main.o FPGrowth.o
EXEC = asso


CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror #$(CUSTOMDEFINES)


all: $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ1)

$(OBJ1): FPGrowth.h

.PHONY: clean

clean:
	rm -f *.o $(EXEC)
