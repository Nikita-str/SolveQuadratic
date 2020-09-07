Files=Source.o SolveQuadratic.o GenericFunc.o
all: SolveQuadratic.exe

SolveQuadratic.exe: $(Files)
	gcc -m32 -o $@ $(Files) -lm

%.o: %.c
	gcc -m32 -c $< -o $@ -lm

Source.o: TestMacro.h GenericFunc.h SolveQuadratic.h  GenericFunc.o SolveQuadratic.o 
SolveQuadratic.o: SolveQuadratic.h TestMacro.h GenericFunc.o
GenericFunc.o: GenericFunc.h
GenericFunc.h: GenericFunc.c

.PHONY: all clean clean_o

clean_o:
	-rm $(Files)

clean:
	-rm $(Files)
	-rm SolveQuadratic.exe
