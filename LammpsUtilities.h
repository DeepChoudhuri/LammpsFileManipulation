#ifndef GUARD_LammpsUtilities_h
#define GUARD_LammpsUtilities_h

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>

#include "DumpClass.h"
#include "AtomClass.h"

std::string *parseDump(std::string, int);

void copyInitialAtomArrayData(AtomClass*, AtomClass*, long);

std::vector<long> getCommonAtomIDs(DumpClass*, DumpClass*);

DumpClass* setDumpParameters(DumpClass*, long);

AtomClass* getCommonIDAtomsArray(DumpClass*, long, std::vector<long>);

void calculateStressDifference(DumpClass*, DumpClass*);

void writeDumpInfoToFile(DumpClass*, std::string);

#endif
