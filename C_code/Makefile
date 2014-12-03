CXX = g++
OPT = -Ofast #-vec-report -g

EXES = myfunc.exe
R = generate_rands.exe

all: $(EXES) $(R)

myfunc.exe: myfunc.cpp
	$(CXX) $(OPT) myfunc.cpp -o myfunc.exe

generate_rands.exe: generate_rands.cpp
	$(CXX) $(OPT) generate_rands.cpp -o generate_rands.exe

clean:
	rm -rf *.o *.exe *~ 
