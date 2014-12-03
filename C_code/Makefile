CXX = g++
OPT = -g

EXES = myfunc.exe
R = generate_rands.exe
CMDL = generate_cmdlines.exe

all: $(EXES) $(R) $(CMDL)

generate_cmdlines.exe: generate_cmdlines.cpp
	$(CXX) $(OPT) generate_cmdlines.cpp -o generate_cmdlines.exe

myfunc.exe: myfunc.cpp
	$(CXX) $(OPT) myfunc.cpp -o myfunc.exe

generate_rands.exe: generate_rands.cpp
	$(CXX) $(OPT) generate_rands.cpp -o generate_rands.exe

clean:
	rm -rf *.o *.exe *~ 
