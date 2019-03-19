#include "natural.h"
#include <fstream>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv)
{
  if (argc != 3) return 1;
  const int N = atoi(argv[2]);
  ofstream fout("e.txt");
  char s[100];
  for (int i = atoi(argv[1]); i < N; ++i) {
    Natural a;
    sprintf(s, "e-d%d.txt", i);
    cout << i << "...input" << flush;
    ifstream fin(s);
    Digit d = 0;
    char c;
    fin >> d;
    fin.get(c);
    if (d == 0 || c != ',' || !a.scan(fin)) { cerr << "Error!\n"; return 1; }
    fin.close();
    cout << "...output..." << endl;
    fout.width(d);
    fout.fill('0');
    fout << a;
  }
  return 0;
}
