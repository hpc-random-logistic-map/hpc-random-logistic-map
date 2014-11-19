CXX = icpc
OPT = -Ofast -vec-report -g -openmp

INCLUDE =-I$(H5DIR)/include
LIB_DIR =-L$(H5DIR)/lib
LIBS =-lhdf5

EXES = matmul_omp.exe
TMP = matrix_multiply.exe

all: $(EXES) 

matmul_omp.exe: matmul_omp.cpp
	$(CXX) $(OPT) matmul_omp.cpp $(INCLUDE) $(LIB_DIR) $(LIBS) -o matmul_omp.exe

matrix_multiply.exe: matrix_multiply.cpp
	$(CXX) $(OPT) -o matrix_multiply.exe -g matrix_multiply.cpp

clean:
	rm -rf *.o *.exe *~ ans*
