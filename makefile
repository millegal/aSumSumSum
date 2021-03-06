CC      = /usr/bin/g++
CFLAGS  = -Wall -g -D_GNU_SOURCE -c -std=c++11
LDFLAGS = -lm
OBJ =  set.o inout.o som.o

foo:
	cl /W4 /EHsc main.cpp som.cpp set.cpp inout.cpp /O2 /link /out:som.exe
	del *.obj

test:
	cl /W4 /EHsc test\test.cpp test\gtest.lib


LinuxStatic: $(OBJ)
	ar crs libsom.a $(OBJ)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c -fPIC $<

clean:
	del *.obj
	del *.set
	del *.exp
