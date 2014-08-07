#include "pp.h"
#include <stdio.h>
#include <stdlib.h>


double linear(double a, double b, double weight)
{
  return (1 - weight) * a + weight * b;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Usage:\n\n");
    printf("interp num_images file1 file2");
  }
  else
  {
    int nimage = atoi(argv[1]),k, j = 0, i;
    FILE *f1 = fopen(argv[2], "r");
    char line[256];
    for (i = 0; i < 4; ++i)
    {
      fgets(line, sizeof(line), f1);
    }
    rewind(f1);
    PPFile *fil1 = readPP(f1);
    fclose(f1);
    FILE *f2 = fopen(argv[3], "r");
    PPFile *fil2 = readPP(f2);
    fclose(f2);
    char fname[20];
    for (k = 1; k <= nimage; ++k)
    {
      sprintf(fname, "%s_%d.dat", argv[2], k);
      FILE *f = fopen(fname, "a");
      double w = 1.0 * k / (nimage + 1);

      fprintf(f, "\n %7d %7d %7d %7d %7d %7d %7d %7d\n", fil1->ngrid[0],
        fil1->ngrid[1], fil1->ngrid[2], fil1->ngrid[0], fil1->ngrid[1],
        fil1->ngrid[2], fil1->nat, fil1->ntype);

      fprintf(f, "%6d   %15.8f %15.8f %15.8f %15.8f %15.8f %15.8f\n", 8,
        fil1->celldm[0], fil1->celldm[1]/fil1->celldm[0],
        fil1->celldm[2]/fil1->celldm[0], 0.0, 0.0, 0.0);

      fprintf(f, "%s", line);

      int types[fil1->ntype];
      int tmp = 0;
      for (i = 0; i < fil1->nat; ++i)
      {
        if (fil1->atoms[i].z != tmp)
        {
          tmp = fil1->atoms[i].z;
          types[j] = tmp;
          fprintf(f, "%4d %4s %7.2f\n", ++j, fil1->atoms[i].sym, (double) tmp);
          if (j == fil1->ntype)
          {
            break;
          }
        }
      }
      for (i = 0; i < fil1->nat; ++i)
      {
        fprintf(f, "%4d    ", i+1);
        for (j = 0; j < 3; ++j)
        {
          fprintf(f, "%14.9f ", linear(fil1->atoms[i].coor[j], fil2->atoms[i].coor[j], w)/fil1->celldm[0]);
        }
        for(j = 0; j < fil1->ntype; ++j)
        {
          if (types[j] == fil1->atoms[i].z)
          {
            fprintf(f, "%4d\n", j + 1);
            break;
          }
        }
      }
      int ndata = fil1->ngrid[0] * fil1->ngrid[1] * fil1->ngrid[2];
      for (i = 0; i < ndata; ++i)
      {
        if (i % 5 == 0 && i > 0)
        {
          fprintf(f, "\n");
        }
        fprintf(f, "%17.9E ", linear(fil1->data[i], fil2->data[i], w));
      }
    }
  }
  return 0;
}
