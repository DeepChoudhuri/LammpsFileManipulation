#ifndef GUARD_DumpClass_h
#define GUARD_DumpClass_h

#include "DumpClass.h"
#include "AtomClass.h"

class DumpClass{

  public:

    DumpClass();
    DumpClass(std::string*, int, std::string*);
    ~DumpClass();

    void readDumpFile(int, std::string);
    void writeHeaderInfo();
    void writeAtomInfo();
    void printDumpFile();

    void setAtomInfo(AtomClass*);
    std::string* getAtomInfoAsString(); // This function is a very specific to the test example - we need to generalize this later
    std::vector<long> getAtomIDVector();
    AtomClass* getAtomInfoClass();

    std::string *headerInfo, *lammpsItemsList, *AtomInfo;
    AtomClass *atoms;
    int numberOfHeaderLines;
    long int timeStep,numberOfAtoms,totalLinesInFile;
};

#endif
