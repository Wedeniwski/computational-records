// g++ -O3 -D_Old_STD_ -Xlinker -b -Xlinker D:0x80000000 decimal.cpp piologie.a -o decimal

#include "natural.h"
#include <fstream.h>


int main(int argc, char** argv)
{
  char ch;
  size_t m,n;

  if (argc != 5) {
    cerr << "USAGE: " << argv[0] << " <exp number> <input file> <output file> <output decimal file>\n";
    return 1;
  }

  cout << "read..." << endl;
  Natural a;
  ifstream fin(argv[2]);
  fin >> m;                       // precision = 45151965
  fin.get(ch);
  if (ch != ',') { cerr << "Error!\n"; return 1; }
  fin >> n;                       // decimals = 869894101
  fin.get(ch);
  if (ch != ',' || !a.scan(fin)) { cerr << "Error!\n"; return 1; }
  fin.close();

  cout << "pow..." << atoi(argv[1]) << endl;

  Natural b = pow(10, atoi(argv[1]));

  cout << "multiply..." << m << endl;

  if (m > 1000) {
    Natural c,d;
    cout << "split..." << endl;
    a.split(m/2, c, d);
    cout << "multiply2..." << endl;
    a = c * b;
    cout << "multiply3..." << endl;
    d *= b;
    cout << "move..." << endl;
    a.lmove(m/2);
    cout << "add..." << endl;
    a += d;
  } else a *= b;


  a.fast_rshift(m);

  ofstream fout(argv[4]);
  fout << atoi(argv[1]) << ',' << print(a);
  fout.close();

  a = 0; a.fast_append(m); a.normalize();
  if (a != 0) {
    Digit* pT;
    const size_t sz = a.trailing_zeros(pT);
    a.fast_rshift(sz); m -= sz;
  }

  fout.open(argv[3]);
  fout << m << ',' << n << ',';
  fout << print(a);
  fout.close();

  return 0;
}
