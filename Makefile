CXX=/usr/bin/g++

CXXFLAGS=-c -Wall -O2

all: lammpsManip.x

lammpsManip.x: main.o AtomClass.o DumpClass.o LammpsUtilities.o
	$(CXX) main.o AtomClass.o DumpClass.o LammpsUtilities.o -o lammpsManip.x -O2
# $(CXX) main.o InputHandlerClass.o UtilitiesClass.o FieldHandlerClass.o EshelbySolverClass.o -o eshelby.x -O2 -larmadillo --lgfortran

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

AtomClass.o: AtomClass.cpp AtomClass.h
	$(CXX) $(CXXFLAGS) AtomClass.cpp

DumpClass.o: DumpClass.cpp DumpClass.h
	$(CXX) $(CXXFLAGS) DumpClass.cpp

LammpsUtilities.o: LammpsUtilities.cpp LammpsUtilities.h
	$(CXX) $(CXXFLAGS) LammpsUtilities.cpp


#run:
#	./lammpsManip.x > log

clean:
	rm -f *o *.x *.new.* log*
	rm dumpfiles/*Mod*

clear:
	rm -f *o *.x *.new.* *.dump.*
	rm dumpfiles/*Mod*
