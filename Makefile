all: program

program: cudacode.o
	g++ -o program -L/usr/local/cuda/lib64 -lcuda -lcudart cudacode.o 

cudacode.o:
	nvcc -c -arch=sm_60 cudacode.cu -ccbin "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx84\x84\cl.exe"

clean: rm -f *.o program