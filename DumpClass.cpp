#include <iostream>
#include <fstream>

#include "LammpsUtilities.h"

DumpClass::DumpClass()
{
  this->numberOfHeaderLines = 9;
  this->headerInfo = new std::string[this->numberOfHeaderLines];
}

DumpClass::DumpClass(std::string *headInfo,int numAtoms, std::string *atomHeader)
{
  this->numberOfHeaderLines = 9;
  this->numberOfAtoms = numAtoms;
  this->headerInfo = new std::string[this->numberOfHeaderLines];

  //std::cout<< atomHeader << std::endl;

  for(int i =0 ; i < this->numberOfHeaderLines-1; ++i){
    if(i!=3){
      this->headerInfo[i] = headInfo[i];
    }else{
      this->headerInfo[3] = std::to_string(this->numberOfAtoms);
    }
  } // Initializes the hearder information string
  this->headerInfo[8] = *atomHeader; //Final header information

  this->atoms = new AtomClass[numAtoms]; //Initializes the atom class
}


void DumpClass::readDumpFile(int numLammpsItems, std::string filePath)
{
  std::ifstream inFile(filePath);

  std::string line;
  for (int i = 0; i < 9; i++) {
    getline(inFile, line);
    this->headerInfo[i] = line;
    //std::cout << this->headerInfo[i] << std::endl;
  }
  try{
      // For the time step given int he dump files
      //std::cout<< this->headerInfo[1] << std::endl;
      //std::cout<< this->headerInfo[3] << std::endl;
      this->timeStep = std::stoi(this->headerInfo[1]);
      // Total number of atoms given in the dump files
      this->numberOfAtoms = std::stoi(this->headerInfo[3]);
    }catch(std::exception e)
    {
      std::cout << "Bad string! Error in string to integer/double conversion" << std::endl;
      std::cout << e.what() << std::endl;
    }

  this->lammpsItemsList = parseDump(this->headerInfo[8], numLammpsItems+2); // header info for all per atom quantities (e.g. represented by AtomInfo )

  this->totalLinesInFile = this->numberOfAtoms + this->numberOfHeaderLines; //Total number of lines present in the dump file

// This one gets  data from individual rows, containing per atom info like coordinates, energies etc., from the dump file.
  this->AtomInfo = new std::string[this->numberOfAtoms];
  for (int i = 9; i < this->totalLinesInFile ; i++) {
    getline(inFile, line);
    this->AtomInfo[i-9] = line;
  }

  this->atoms = new AtomClass[this->numberOfAtoms]; // Each row of atom data is stored in a class named AtomClass

  std::string *tempAtomQuantList;
  for(int i = 0; i < this->numberOfAtoms; ++i)
  //for(int i = 0; i < 2; ++i) //For testing
  {
    tempAtomQuantList = parseDump(this->AtomInfo[i], numLammpsItems);
    // each row of per-atom data is saved in the variables defined in the atom class
    this->atoms[i].readAtomValues(numLammpsItems, this->lammpsItemsList, tempAtomQuantList);
    //this->atoms[i].writeAtomData();
    tempAtomQuantList = NULL;
  }
}


void DumpClass::setAtomInfo(AtomClass *AtomArrayData)
{
  for(int i = 0; i < this->numberOfAtoms; ++i){

    //std::cout << "Inside" <<std::endl;
    atoms[i].id = AtomArrayData[i].id;
    atoms[i].type = AtomArrayData[i].type;

    atoms[i].xpos = AtomArrayData[i].xpos;
    atoms[i].ypos = AtomArrayData[i].ypos;
    atoms[i].zpos = AtomArrayData[i].zpos;

    atoms[i].vol = AtomArrayData[i].vol;
    atoms[i].csym = AtomArrayData[i].csym;
    atoms[i].pe = AtomArrayData[i].pe;

    for(int j = 0; j < 8; ++j){
      //std::cout << "More Inside" <<std::endl;
      atoms[i].sij[j] = AtomArrayData[i].sij[j];
      atoms[i].delSij[j] = AtomArrayData[i].delSij[j];
    }
  }
}


std::string *DumpClass::getAtomInfoAsString()
{
  std::string* atomData = new std::string[this->numberOfAtoms];

  for(int i = 0; i < this->numberOfAtoms; ++i){
    try{

      atomData[i] = std::to_string(atoms[i].id) + " " + std::to_string(atoms[i].type) + " " + std::to_string(atoms[i].xpos) + " " + \
                    std::to_string(atoms[i].ypos) + " " + std::to_string(atoms[i].zpos) + " " +  std::to_string(atoms[i].pe)+ " " + std::to_string(atoms[i].vol) + " " + \
                    std::to_string(atoms[i].sij[0]) + " " + std::to_string(atoms[i].sij[1]) + " " + std::to_string(atoms[i].sij[2]) +  " " + \
                    std::to_string(atoms[i].sij[3]) + " " + std::to_string(atoms[i].sij[4]) + " " + std::to_string(atoms[i].sij[5]) +  " " + \
                    std::to_string(atoms[i].sij[6]) + " " + std::to_string(atoms[i].sij[7]);
      //std::cout<<"pe "<< atoms[i].pe << " " << std::to_string(atoms[i].pe) << std::endl;
    }catch(std::exception e)
     {
       std::cout << "Bad string! Error in integer/double to string conversion" << std::endl;
       return 0;
     }

  }
  return atomData;
}


//Print out the dheader information for debuggin purposes
void DumpClass::writeHeaderInfo()
{
  for(int i = 0; i < this->numberOfHeaderLines; ++i){
    std::cout<< this->headerInfo[i] << std::endl;
  }
}

void DumpClass::writeAtomInfo()
{
  std::string* atomData = this->getAtomInfoAsString();
  for(int i = 0; i < this->numberOfAtoms; ++i){
    std::cout<< atomData[i] << std::endl;
  }
}


void DumpClass::printDumpFile()
{
  this->writeHeaderInfo();
  this->writeAtomInfo();
}



std::vector<long> DumpClass::getAtomIDVector()
{
  std::vector<long> atomID = std::vector<long>();

  for(int i = 0; i < this->numberOfAtoms; i++){
    atomID.push_back(this->atoms[i].id);
  }

  return atomID;
}


AtomClass* DumpClass::getAtomInfoClass()
{
  return atoms;
}


DumpClass::~DumpClass()
{
  delete []this->headerInfo;
  delete []this->AtomInfo;
  delete []this->lammpsItemsList;
  delete []this->atoms;

}
