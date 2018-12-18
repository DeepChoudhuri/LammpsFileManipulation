#include<iostream>

#include "AtomClass.h"
#include <cmath>

AtomClass::AtomClass()
{
  this->sij = new double[8];
  this->delSij = new double[8];

  this->setZeros();
}

// Sets up all variables i.e. per-atom quantities to zero
void AtomClass::setZeros()
{
  this->id = 0;
  this->type = 0;

  this->xpos = 0.0;
  this->ypos = 0.0;
  this->zpos = 0.0;

  this->vol = 0.0;
  this->csym = 0.0;
  this->pe = 0.0;

  for(int i = 0; i < 8; ++i)
  {
    this->sij[i] = 0.0;
    this->delSij[i] = 0.0;
  }
}


// each row of per-atom data is saved in the variables defined in the atom class
void AtomClass::readAtomValues(int itemNumbers, std::string *itemList, std::string *atomInfo)
{
  for(int i = 2; i < itemNumbers + 2 ; i++)
  {
    try{
        //std::cout<< itemList[i] << " " << atomInfo[i-2] << std::endl;
        if(itemList[i].compare("id")==0)
          this->id = std::stoi(atomInfo[i-2]);

        if(itemList[i].compare("type")==0)
          this->type = std::stoi(atomInfo[i-2]);

        if(itemList[i].compare("x")==0)
          this->xpos = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("y")==0)
          this->ypos = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("z")==0)
          this->zpos = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_volatm")==0)
          this->vol = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("c_csym")==0)
          this->csym = std::stod(atomInfo[i-2]);

        //if(itemList[i].compare("c_peatm")==0){
        if(itemList[i].compare("c_peratom")==0){
          this->pe = std::stod(atomInfo[i-2]);
          //std::cout<< itemList[i] <<" pe debug"<< this->pe << " " << atomInfo[i-2] << std::endl;
        }

        if(itemList[i].compare("v_s11")==0)
          this->sij[0] = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_s22")==0)
          this->sij[1] = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_s33")==0)
          this->sij[2] = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_s12")==0)
          this->sij[3] = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_s23")==0)
          this->sij[4] = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_s31")==0)
          this->sij[5] = std::stod(atomInfo[i-2]);

        if(itemList[i].compare("v_sii")==0)
          this->sij[6] = this->meanStress(this->sij[0], this->sij[1], this->sij[2]);

        //if(itemList[i].compare("v_svm")==0){
          //std::cout<< itemList[i] <<" debug"<< std::endl;
          this->sij[7] = this->vonMisses(this->sij[0], this->sij[1], this->sij[2], this->sij[3], this->sij[4], this->sij[5]);
         //}
        }
     catch(std::exception e)
     {
       std::cout << "Bad string! Error in string to integer/double conversion" << std::endl;
       std::cout << "Error with " << itemList[i] << ", string-value  = " << atomInfo[i-2] <<std::endl;
     }
  }
}


// Write all atom data  on the screen for debugging
void AtomClass::writeAtomData()
{
  std::cout << "id " <<  this->id << std::endl;
  std::cout << "type " << this->type << std::endl;
  std::cout << "xs " << this->xpos << std::endl;
  std::cout << "ys " << this->ypos << std::endl;
  std::cout << "xs " << this->zpos << std::endl;
  std::cout << "v_volatm " << this->vol << std::endl;
  std::cout << "c_csym " << this->csym << std::endl;
  std::cout << "c_peatm " << this->pe << std::endl;
  std::cout << "v_s11 " << this->sij[0] << std::endl;
  std::cout << "v_s22 " << this->sij[1] << std::endl;
  std::cout << "v_s33 " << this->sij[2] << std::endl;
  std::cout << "v_s12 " << this->sij[3] << std::endl;
  std::cout << "v_s23 " << this->sij[4] << std::endl;
  std::cout << "v_s31 " << this->sij[5] << std::endl;
  std::cout << "v_sii " << this->sij[6] << std::endl;
  std::cout << "v_svm " << this->sij[7] << std::endl;
  std::cout << "v_dels11 " << this->delSij[0] << std::endl;
  std::cout << "v_dels22 " << this->delSij[1] << std::endl;
  std::cout << "v_dels33 " << this->delSij[2] << std::endl;
  std::cout << "v_dels12 " << this->delSij[3] << std::endl;
  std::cout << "v_dels23 " << this->delSij[4] << std::endl;
  std::cout << "v_dels31 " << this->delSij[5] << std::endl;
  std::cout << "v_delsii " << this->delSij[6] << std::endl;
  std::cout << "v_delsvm " << this->delSij[7] << std::endl;
}



double AtomClass::vonMisses(double s11, double s22, double s33, double s12, double s23, double s31)
{
  double val = 0.0;

  val = std::sqrt((1.0/2.0)*(std::pow((s11 - s22),2.0) + std::pow((s22 - s33),2.0) + \
        std::pow((s33 - s11),2.0) + 6.0*(std::pow(s12,2.0) + std::pow(s23,2.0) + std::pow(s31,2.0))));
  //std::cout << "Val = " << val << std::endl;

  return val;
}


double AtomClass::meanStress(double s11, double s22, double s33)
{
  return (1.0/3.0)*(s11 + s22 + s33);
}


AtomClass::~AtomClass()
{
  delete []this->sij;
  delete []this->delSij;
}
