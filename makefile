CXX = g++
CXXFLAGS = -Wall -g
IODIR =../../proj0_IO/

proj0: show.o mytest.cpp
	$(CXX) $(CXXFLAGS) show.o mytest.cpp -o proj0

show.o: show.h show.cpp
	$(CXX) $(CXXFLAGS) -c show.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj0

val:
	valgrind ./proj0