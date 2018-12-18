#include "LammpsUtilities.h"
#include <cmath>

std::string *parseDump(std::string dataString, int numItems)
{
  int startptr = 0;
  int runptr = 0;
  int strCount = 0;

  //std::cout << "Number of ITEMS in dump file: "<< numItems << std::endl;
  std::string *returnString = new std::string[numItems]; // BE CAREFUL the number "15" will change depending on the number of inputs there are in the lammps input file
  //std::cout << dataString.size() << " " << dataString << std::endl;
  for (int i = 0; i < dataString.size(); i++) {
    /* code */
    if (dataString[i] == ' ' || (i == dataString.size()-1)){
      runptr = i;
      char *tempCharStr = new char[runptr-startptr];
      //std::cout << "Initial Size  of the char array: " << strlen(tempCharStr) <<std::endl;
      //tempCharStr = (char *)malloc(runptr-startptr);

      for (int j = 0; j < std::abs(runptr - startptr) ; j++) {
        tempCharStr[j] = dataString[startptr+j];
        //std::cout << tempCharStr[j] << " " << dataString[startptr+j] << std::endl;
      }

      //std::cout << "Space Char at = " << i << " Location of start = " << startptr <<" " << "Length = " << std::abs(runptr - startptr)<< std::endl;
      //std::cout << "Size  of the char array: " << strlen(tempCharStr) <<std::endl;
      //std::string tempStr(tempCharStr);
      //std::cout <<tempStr <<std::endl;
      //std::cout << strlen(tempCharStr)<< std::endl;;

      //std::cout << "New string:  " << lineCount << " " << tempStr[lineCount] << std::endl;
      //std::cout <<  dataString << std::endl;
      //std::cout << "Parse string  and size:  " << tempCharStr << " " << std::abs(startptr - runptr) << std::endl;
      returnString[strCount] = tempCharStr;
      //std::cout << returnString[strCount] << " " << std::endl;

      tempCharStr = NULL;
      //std::cout << "Size  of the char array: " << tempCharStr <<std::endl;
      startptr = i+1;
      strCount++;
      //++lineCount;

    } //END if statement
  } //END for statement

  //std::cout << returnString << std::endl;
  return returnString;
}



/* Return all the atoms IDs that are common to both the source (initial configuration)
 and the destination (deformed configuration) classes*/
std::vector<long> getCommonAtomIDs(DumpClass* source, DumpClass* destination)
{
  std::vector<long> sourceIDVec = source->getAtomIDVector();
  std::vector<long> destinationIDVec = destination->getAtomIDVector();
  std::vector<long> commonIDVec = std::vector<long>();
  long counter = 0;

  std::sort(sourceIDVec.begin(), sourceIDVec.end()); // Sort source vector to facilitate binary search

  if(source->numberOfAtoms > destination->numberOfAtoms){
    counter = destination->numberOfAtoms;
  }else{
    counter = source->numberOfAtoms;
  }

  for(int i = 0; i < counter; i++){
    if(std::binary_search(sourceIDVec.begin(),sourceIDVec.end(), destinationIDVec[i])){
      commonIDVec.push_back(destinationIDVec[i]);
    }
  }
  return commonIDVec;
}



void copyInitialAtomArrayData(AtomClass* dest, AtomClass* src, long numAtoms)
{
  for(int i = 0; i< numAtoms; ++i){
    dest[i].id   = src[i].id;
    dest[i].type = src[i].type;

    dest[i].xpos = src[i].xpos;
    dest[i].ypos = src[i].ypos;
    dest[i].zpos = src[i].zpos;

    dest[i].vol  = src[i].vol;
    dest[i].csym = src[i].csym;
    dest[i].pe   = src[i].pe;

    for(int k = 0; k < 8; ++k){
      dest[i].sij[k] = src[i].sij[k];
      dest[i].delSij[k] = src[i].delSij[k];
    }
  }
}


AtomClass* getCommonIDAtomsArray(DumpClass* dump, long numAtoms, std::vector<long> comIDVec)
{
  AtomClass *dumpAtoms = dump->getAtomInfoClass();
  AtomClass *comAtomArray = new AtomClass[numAtoms];

  for(int i= 0; i < numAtoms; ++i){

    for(int j = 0; j < dump->numberOfAtoms; ++j){
      if(dumpAtoms[j].id == comIDVec[i]){
        //std::cout << "Dump ID = "<< dumpAtoms[j].id << " " << "Common ID = " << comIDVec[i] << std::endl;
        comAtomArray[i].id   = comIDVec[i];
        comAtomArray[i].type = dumpAtoms[j].type;

        comAtomArray[i].xpos = dumpAtoms[j].xpos;
        comAtomArray[i].ypos = dumpAtoms[j].ypos;
        comAtomArray[i].zpos = dumpAtoms[j].zpos;

        comAtomArray[i].vol  = dumpAtoms[j].vol;
        comAtomArray[i].csym = dumpAtoms[j].csym;
        comAtomArray[i].pe   = dumpAtoms[j].pe;

        for(int k = 0; k < 8; ++k){
          comAtomArray[i].sij[k] = dumpAtoms[j].sij[k];
          comAtomArray[i].delSij[k] = dumpAtoms[j].delSij[k];
        }
        //comAtomArray[i].writeAtomData();
     }
    }
  }

 return comAtomArray;
}



DumpClass* setDumpParameters(DumpClass* destination,long numAtoms){

  std::string atomHeader = std::string("ITEM: ATOMS ") + "id " + "type " + "x " + "y " + "z " + \
                            "v_volatm " +  "c_csym " + "c_peratom " + \
                            "v_s11 " + "v_s22 " + "v_s33 " + "v_s12 " + \
                            "v_s23 " + "v_s31 " + "v_sii " + "v_svm " + \
                            "v_dels11 " + "v_dels22 " + "v_dels33 " + "v_dels12 " + \
                            "v_dels23 " + "v_dels31 " + "v_delsii " + "v_delsvm ";

  DumpClass *newDump = new DumpClass(destination->headerInfo,numAtoms, &atomHeader);

  return newDump;
}


void calculateStressDifference(DumpClass* source, DumpClass* destination)
{
  std::vector<long> commonIDVec = getCommonAtomIDs(source, destination);
  long numAtoms = commonIDVec.size();
  AtomClass *atoms = new AtomClass[numAtoms];
  AtomClass *a1 = getCommonIDAtomsArray(source, numAtoms, commonIDVec); // source or the initial configuration
  AtomClass *a2 = getCommonIDAtomsArray(destination, numAtoms, commonIDVec); //target or destination or the final configuration

  copyInitialAtomArrayData(atoms, a2, numAtoms); // Copy a2 to atoms

  for(int i = 0; i< numAtoms; ++i){
ß
    for(int j = 0; j < 7; ++j){
      //atoms[i].delSij[j] = std::abs(std::abs(a2[i].sij[j]) - std::abs(a1[i].sij[j]));
      atoms[i].delSij[j] = a2[i].sij[j] - a1[i].sij[j];
    }
    atoms[i].delSij[7] = std::abs(a2[i].sij[7] - a1[i].sij[7]);
    //atoms[i].delSij[7] = a2[i].sij[7] - a1[i].sij[7];
    //atoms[i].writeAtomData();
  }

  //atoms->writeAtomData();

  std::string filePath = std::string("dumpfiles/") + "DumpMod" + "." + std::to_string(destination->timeStep);
  DumpClass *newDump = setDumpParameters(destination, numAtoms);

  for(int i = 0; i < 9; ++i){
    std::cout << "i = "<< i << " "<< newDump->headerInfo[i] << std::endl;
  }
  newDump->setAtomInfo(atoms);
  writeDumpInfoToFile(newDump, filePath);

  delete newDump;
  delete []atoms;
}



/*
Writes the Dump Class object to a file.

Uses the following format for per-atom quantities
"id " + "type " + "x " + "y " + "z " + \
"v_volatm " +  "c_csym " + "c_peratom " + \
"v_s11 " + "v_s22 " + "v_s33 " + "v_s12 " + \
"v_s23 " + "v_s31 " + "v_sii " + "v_svm " + \
"v_dels11 " + "v_dels22 " + "v_dels33 " + "v_dels12 " + \
"v_dels23 " + "v_dels31 " + "v_delsii " + "v_delsvm "
*/


void writeDumpInfoToFile(DumpClass* dump, std::string filepath)
{
  std::ofstream outFile;
  outFile.open(filepath);

  for(int i = 0; i < 9; ++i){
    outFile << dump->headerInfo[i] << "\n";
  }

  std::cout << "Numner of atoms = " << dump->numberOfAtoms << std::endl;

  std::string temp;
  for(int i = 0; i < dump->numberOfAtoms; ++i){
    temp =  std::to_string(dump->atoms[i].id)        + " " + std::to_string(dump->atoms[i].type)      + " " + std::to_string(dump->atoms[i].xpos)      + " " + \
            std::to_string(dump->atoms[i].ypos)      + " " + std::to_string(dump->atoms[i].zpos)      + " " + std::to_string(dump->atoms[i].vol)       + " " + \
            std::to_string(dump->atoms[i].csym)      + " " + std::to_string(dump->atoms[i].pe)        + " " + std::to_string(dump->atoms[i].sij[0])    + " " + \
            std::to_string(dump->atoms[i].sij[1])    + " " + std::to_string(dump->atoms[i].sij[2])    + " " + std::to_string(dump->atoms[i].sij[3])    + " " +\
            std::to_string(dump->atoms[i].sij[4])    + " " + std::to_string(dump->atoms[i].sij[5])    + " " + std::to_string(dump->atoms[i].sij[6])    + " " +\
            std::to_string(dump->atoms[i].sij[7])    + " " + std::to_string(dump->atoms[i].delSij[0]) + " " + std::to_string(dump->atoms[i].delSij[1]) + " " +\
            std::to_string(dump->atoms[i].delSij[2]) + " " + std::to_string(dump->atoms[i].delSij[3]) + " " + std::to_string(dump->atoms[i].delSij[4]) + " " +\
            std::to_string(dump->atoms[i].delSij[6]) + " " + std::to_string(dump->atoms[i].delSij[6]) + " " + std::to_string(dump->atoms[i].delSij[7]);
    //std::cout << temp << std::endl;
    outFile << temp << "\n";
  }

  outFile.close();
}
