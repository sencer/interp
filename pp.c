#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pp.h"

#define L_LENGTH 110

PPFile *readPP(FILE *f)
{
  char line[L_LENGTH];
  int i, j, *z;
  PPFile *pp = (PPFile *) malloc(sizeof(PPFile));

  // Header data: # of grid points, atoms, species, as well as celldm
  fgets(line, L_LENGTH, f); // Dismiss Title Line
  fgets(line, L_LENGTH, f);
  sscanf(line,"%d %d %d %*d %*d %*d %d %d",
      &pp->ngrid[0], &pp->ngrid[1], &pp->ngrid[2], &pp->nat, &pp->ntype);
  fgets(line, L_LENGTH, f);
  sscanf(line,"%*d %lf %lf %lf",
      &pp->celldm[0], &pp->celldm[1], &pp->celldm[2]);
  // change units from alat to bohr
  pp->celldm[1] *= pp->celldm[0];
  pp->celldm[2] *= pp->celldm[0];
  fgets(line, L_LENGTH, f); //skip another line: gcut something?!

  //allocate an array for nuclei, another for 3D grid data
  pp->atoms = (Atom *) malloc(sizeof(Atom) * (pp->nat));
  pp->data = (double *) malloc((pp->ngrid[0]) * (pp->ngrid[1])
      * (pp->ngrid[2]) * sizeof(double));


  //temproraly store Z of all species in z array.
  char sym[pp->nat][3];
  z = (int *) malloc(sizeof(int) * pp->ntype);

  for (i = 0; i < pp->ntype; ++i)
  {
    fgets(line, L_LENGTH, f);
    sscanf(line, "%*d %s %d", sym[i], &z[i]);
  }

  //read nuclear positions of all atoms
  for (i = 0; i < pp->nat; ++i)
  {
    fgets(line, L_LENGTH, f);

    sscanf(line, "%*d %lf %lf %lf %d", &(pp->atoms[i].coor[0]),
      &(pp->atoms[i].coor[1]), &(pp->atoms[i].coor[2]), &(pp->atoms[i].z));

    strcpy(pp->atoms[i].sym, sym[pp->atoms[i].z - 1]);
    pp->atoms[i].z = z[pp->atoms[i].z - 1];

    for (j = 0; j < 3; ++j)
    {
      pp->atoms[i].coor[j] *= pp->celldm[0];
    }
  }

  free(z);

  //read grid data
  i = 0;
  while(fgets(line, L_LENGTH, f))
  {
    sscanf(line, "%lf %lf %lf %lf %lf", &pp->data[i],
        &pp->data[i+1], &pp->data[i+2], &pp->data[i+3], &pp->data[i+4]);
    i += 5;
  }

  return pp;
}

void closePP(PPFile *pp)
{
  free(pp->atoms);
  free(pp->data);
  free(pp);
}
