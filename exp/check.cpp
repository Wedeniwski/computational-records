#include <fstream>
#include <ctype.h>

using namespace std;

void check(const char* a, const char* b)
{
  ifstream fin(a);
  ifstream fin2(b);
  char c1,c2;
  bool first = true;
  int digit = 0;
  while (!fin.eof() && !fin2.eof()) {
    do {
      while (fin.get(c1) && !isdigit(c1));
    } while (first && c1 != '2');
    do {
      while (fin2.get(c2) && !isdigit(c2));
    } while (first && c2 != '2');
    ++digit;
    first = false;
    if (c2 != c1) break;
  }
  cout << "Error at " << digit << ',' << c1 << '=' << c2 << endl;
}


int main()
{
  check("e_100000000.txt", "exp.tmp");
  return 0;
}
