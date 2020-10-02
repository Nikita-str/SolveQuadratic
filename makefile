Files=Source.o SolveQuadratic.o GenericFunc.o ReadDouble.o
all: SolveQuadratic.exe

SolveQuadratic.exe: $(Files)
	g++ -m64 -o $@ $(Files) -lm

%.o: %.c
	gcc -m64 -c $< -o $@ -lm

%.o: %cpp
	g++ -m64 -c $< -o $@

Source.o: TestMacro.h GenericFunc.h SolveQuadratic.h  GenericFunc.o SolveQuadratic.o 
SolveQuadratic.o: SolveQuadratic.h TestMacro.h GenericFunc.o
GenericFunc.o: GenericFunc.h
GenericFunc.h: GenericFunc.c
ReadDouble.o: cpp_ReadDouble.h ReadDouble.cpp

.PHONY: all clean clean_o

clean_o:
	-rm $(Files)

clean:
	-rm $(Files)
	-rm SolveQuadratic.exe
