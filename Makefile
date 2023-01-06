cs3421_emul: clock.o cpu.o device.o dMemory.o iMemory.o cache.o io.o parser.o
	g++ -g -o cs3421_emul $^

clock.o: clock.cpp clock.h
cpu.o: cpu.cpp cpu.h
device.o: device.cpp device.h
dMemory.o: dMemory.cpp dMemory.h
iMemory.o: iMemory.cpp iMemory.h
cache.o: cache.cpp cache.h
io.o: io.cpp io.h
parser.o: parser.cpp
	g++ -c *.cpp

clean:
	rm *.o
