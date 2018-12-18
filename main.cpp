#include<iostream>

#include "LammpsUtilities.h"




/*Function gives example of typical usages for command line arguments*/
void usage()
{
  std::cout << std::endl;
  std::cout << "Inputs not given. See example usages below." << std::endl;
  std::cout << "--------------------------------------------"<<std::endl;
  std::cout << "Example 1 : ./lammpsManip.x -i 15 -ref dump/dump.0 -target dump.1"<< std::endl;
  std::cout << "Example 2 : ./lammpsManip.x -items 15 -ref dump/dump.0 -t dump.1"<< std::endl;
  std::cout << "Example 3 : ./lammpsManip.x -I 15 -Ref dump/dump.0 -T dump.1"<< std::endl;
  std::cout << "Example 4 : ./lammpsManip.x -i 15 -r dump/dump.0 -t dump.1"<< std::endl;
  std::cout << std::endl;
  std::cout << "NOTE: The -i/-I/-items MUST BE BEFORE -ref/-r/-Ref"<< std::endl;
  std::cout << "Here -i/-I/-items are the per-atom values defined in the dump file"<< std::endl;
  std::cout << "Here -ref/-r/-Ref is meant for the name of the reference file"<< std::endl;
  std::cout << "Here -target/-t/-T is meant for the target file"<< std::endl;
  std::cout << std::endl;
}

void commandlineParser(int argc, char *argv[])
{

}

/* MAIN FUNCTION */

int main(int argc, char *argv[]) {

  /*-----------------------------Variables for main function------------------------------------------------*/

  /* THIS portion only deals with acquiring and setting command line arguments to
  their respective variables*/

  int numLammpsItems = 0; /* Initializes the Number of ITEMS in the dump file*/
  std::string filePath = "dumpfiles"; // This is DEFAULT. MUST create a folder with this name in the current directory
  std::string refDumpFilePath; // File and path names of the dump files
  std::string tgtDumpFilePath; // File and path names of the dump files
  std::string *cmdLine = new std::string[7]; // string carrying all the command line arguments
  int cmdCount = 0;
  /*------------------------------END variable declaration-----------------------------------------------*/

  /*--------------------------------Processing og command line parameters---------------------------------------------*/

  // Checks to see of there are more than one command line arguments
  if(argc == 1)
  {
    usage();
    return 0;
  }

  // Collects all the commandline arguments and stores them in an array
  for(int i=0; i < argc; i++)
  {
    cmdLine[cmdCount] = argv[i];
    //std::cout << "Commmandline : " << i << " "<< cmdLine[cmdCount] << std::endl;
    ++cmdCount;
  }

  /* Note: the the commandline options should follow a particular sequence.
  This code will be be changes latter to be more versatile */

  // checks for the numerical vlaue of number of ITEMS in the dump folder (CHECK your DUMP files !!)
  //The program will NOT work if this value (and one below) is not provided
  if(argc >1 && (cmdLine[1].compare("-I") || cmdLine[1].compare("-i") || cmdLine[1].compare("-items")))
  {
    try
    {
      numLammpsItems = std::stoi(cmdLine[2]);
      std::cout << "Number of Atom variables: " << numLammpsItems << std::endl;
    }catch(std::exception e)
    {
      std::cout << "Bad string! Error in string to integer conversion" << std::endl;
      std::cout << e.what() << std::endl;
      usage();
      return 0;
    }
  }else
  {
    usage();
    return 0;
  }

  //Checks for the file name of the referece dump file  in the dump folder. Typically ends with a zero ".0"
  //CHECK your DUMP files !!
  //The program will NOT work if this value (and one above) is not provided
  if(argc >1 && (cmdLine[3].compare("-ref") || cmdLine[3].compare("-Ref") \
          || cmdLine[3].compare("-r") || cmdLine[3].compare("-R")))
  {
    refDumpFilePath = cmdLine[4];
    std::cout << "Reference file path: " << cmdLine[4] << std::endl;
    //refDumpFilePath = filePath + "/"+ refFileName;
  }else
  {
    usage();
    return 0;
  }

  if(argc >1 && (cmdLine[5].compare("-t") || cmdLine[5].compare("-T") \
          || cmdLine[5].compare("-target")))
  {
    tgtDumpFilePath = cmdLine[6];
    std::cout << "Target file path: " << tgtDumpFilePath << std::endl;
    //tgtDumpFilePath = filePath + "/"+ tgtFileName;
  }else
  {
    usage();
    return 0;
  }

  /*---------------------------------END of command line parameters--------------------------------------------*/




/*---------------------------------BEGINING OF THE MAIN PROGRAM !!--------------------------------------------*/
  //Read the reference file
  DumpClass *refDump = new DumpClass();
  refDump->readDumpFile(numLammpsItems,refDumpFilePath);
  //refDump->printDumpFile();
  //std::cout<< refDump->getAtomIDVector().size() << std::endl;
  //writeDumpInfoToFile(refDump, "test.dump.0");


  //Read the target file

  DumpClass *tgtDump = new DumpClass();
  tgtDump->readDumpFile(numLammpsItems,tgtDumpFilePath);
  //tgtDump->printDumpFile();
  //std::cout<< tgtDump->getAtomIDVector().size() << std::endl;


  std::vector<long> test = getCommonAtomIDs(refDump,tgtDump);
  //std::cout<< test.size() << std::endl;

  /*for(int i = 0; i < test.size(); ++i){
    std::cout<<test[i] << " ";
  }
  std::cout<<std::endl;*/

  calculateStressDifference(refDump, tgtDump);



/*-----------------------END OF THE MAIN PROGRAM ------------------------------------------------------*/


  // Delete all pointers for garbage collection
  delete []cmdLine;
  delete refDump;
  delete tgtDump;
  return 0;
    /* END OF THE MAIN PROGRAM !!*/
}
