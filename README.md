Set C++ programs that extracts the information from the dump files e.g. atomic positions, types, etc., and uses them to carry out additional analysis.  (Dumpfiles are output files from LAMMPS simulations.)

Specific C++ files present in the repository are:
1. AtomClass.h and AtomClass.cpp: Class for storing information for each atom read for the dum file
2. DumpClass.h and DumpClass.cpp: Class for stroing formation in the dump file.  We will use mutiple dump files to conduct our analysis.
3. LammpsUtilities.h and LammpsUtilities.cpp: Utility file that is meant for conduting additional analyses of the Dump files. basically this program is meant to enhance our analysis capabilities. More functions can and needs to be added.
4. main.cpp: Driver program
5. Makefile: Tested only on OSX

NOTE:
A) This is work in progress, and I will not be responsible if you plan to use this code in your research. (Have fun indulging yourself in with all the bugs!!)
B) MPI has not been implemented.


