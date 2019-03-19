#include <string.h>
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
  if (argc != 3) { cerr << "useage:  mksh_exp <processes> <steps>\n"; return 1; }

  const int n = atoi(argv[1]);
  const int steps = atoi(argv[2]);

  if (n < 4 || n > steps || steps < 8 || steps%n != 0) { cerr << "error!\n"; return 1; }
  if ((steps & (~steps+1)) != steps || (n & (~n+1)) != n) { cerr << "error2!\n"; return 1; }

  char filename[100];
  int start = 0;
  int stop = steps/n;
  for (int i = 0; i < n; ++i) {
    sprintf(filename, "x%d.sh", i);
    ofstream fout(filename);

    fout << "#!/bin/sh\ndate\n";
    int j,step;
    for (j = start; j < stop; ++j) {
      fout << "mkdir " << j << "\ncd " << j << "; ../exp $1 " << j << ' ' << steps << "; cd ..\ndate\n";
    }
    for (step = 2; step < stop-start; step <<= 1) {
      for (j = start; j < stop; j += step) {
        fout << "mkdir " << j  << '-' << (j+step-1) << "\ncd " << j << '-' << (j+step-1) << '\n';
        if (step == 2) {
          fout << "../exp 1 ../" << j << "/ ../" << (j+1) << "/\ndate\n";
          fout << "../exp 3 ../" << j << "/ ../" << (j+1) << "/\ndate\n";
        } else {
          fout << "../exp 1 ../" << j << '-' << (j+step/2-1) << "/ ../" << (j+step/2) << '-' << (j+step-1) << "/\ndate\n";
          fout << "../exp 3 ../" << j << '-' << (j+step/2-1) << "/ ../" << (j+step/2) << '-' << (j+step-1) << "/\ndate\n";
        }
        fout << "cd ..\n";
      }
    }
    if (j >= step) {
      j -= step;
      fout << "mkdir " << j  << '-' << (j+step-1) << "\ncd " << j << '-' << (j+step-1) << '\n';
      if (step == 2) {
        fout << "../exp 1 ../" << j << "/ ../" << (j+1) << "/\ndate\n";
        fout << "../exp 3 ../" << j << "/ ../" << (j+1) << "/\ndate\n";
      } else {
        fout << "../exp 1 ../" << j << '-' << (j+step/2-1) << "/ ../" << (j+step/2) << '-' << (j+step-1) << "/\ndate\n";
        fout << "../exp 3 ../" << j << '-' << (j+step/2-1) << "/ ../" << (j+step/2) << '-' << (j+step-1) << "/\ndate\n";
      }
      fout << "../exp $1 " << steps << ' ' << steps << "\ndate\n";
      fout << "cd ..\n";
    }

    if (i == n-1) {
      for (int j = steps-step; j > 0; j -= step) {
        fout << "cd " << (j-step) << '-' << (j-1);
        fout << "\n../exp 4 ./ ../" << j << '-' << (j+step-1) << "/exp.tmp";
        if (j == steps-step) fout << '0';
        fout << "\ndate\n../exp 5 ./ ./\ndate\n";
        if (j == step) fout << "../exp $1\ndate\n";
        fout << "cd ..\n";
      }
    }
    start = stop; stop += steps/n;
  }

  return 0;
}
