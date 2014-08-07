#include "pp.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("You need to provide a file name!\n");
  }
  else
  {
    int i, j;
    FILE *f = fopen(argv[1], "r");
    PPFile *pp = readPP(f);
    fclose(f);

    printf("ntype: %d\n", pp->ntype);
    printf("nat: %d,\nngrid: ", pp->nat);

    for (i = 0; i < 3; ++i)
    {
      printf("%d ", pp->ngrid[i]);
    }
    printf("\ncelldm: ");

    for (i = 0; i < 3; ++i)
    {
      printf("%f ", pp->celldm[i]);
    }
    printf("\n");

    for (i = 0; i < pp->nat; ++i)
    {
      printf("%-2s %3d", pp->atoms[i].sym, pp->atoms[i].z);
      for (j = 0; j < 3; ++j)
      {
        printf(" %5.2f", pp->atoms[i].coor[j]);
      }
      printf("\n");
    }

    int ndata = 1, v = 1;
    for (size_t i = 0; i < 3; ++i)
    {
      ndata *= pp->ngrid[i];
      v *= pp->celldm[i];
    }
    double sum = 0;
    for (i = 0; i < ndata; ++i)
    {
      sum += pp->data[i];
    }

    printf("%.1f\n", sum * v / ndata);

    closePP(pp);

  }
  return 0;
}
