#ifndef PP_H_XTY81TBG
#define PP_H_XTY81TBG
#include <stdio.h>

typedef struct Atom {
  int z;
  char sym[3];
  double coor[3];
} Atom;

typedef struct PPFile {
  int ntype, nat, ngrid[3];
  double celldm[3];
  double *data;
  Atom *atoms;
} PPFile;

PPFile *readPP(FILE *p);

void closePP(PPFile *pp);

#endif /* end of include guard: PP_H_XTY81TBG */

