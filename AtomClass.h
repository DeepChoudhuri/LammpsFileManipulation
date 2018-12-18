#ifndef GUARD_AtomClass_h
#define GUARD_AtomClass_h

class AtomClass{

// Values extracted from the data file
public:
  AtomClass();
  ~AtomClass();

  void readAtomValues(int, std::string*, std::string*);
  void writeAtomData();

  long int id; // atom #
  unsigned short int type; //atom type expressed as integers like 1, 2 etc.
  double xpos, ypos, zpos; // x, y and z coordinates

  // atomic volume, centro symetric parameter, potential energy per atom
  double vol, csym, pe;

  // Atomic stress tensors - virial stressi.e. sxx, syy, szz, sxy, syz, szx, sii and Svm
  double *sij;

   // difference between this and the reference state
  double *delSij;


private:
  void setZeros();
  double vonMisses(double, double, double, double, double, double);
  double meanStress(double, double, double);
};

#endif
