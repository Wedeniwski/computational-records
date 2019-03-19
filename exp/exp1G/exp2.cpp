/*
g++ -O3 -I ../arithmetic/piologie exp2.cpp ../arithmetic/piologie/piologie.a -o exp2
g++ -O3 -I ../arithmetic/piologie exp2.cpp -D NO_DIRECT_DIV piologieX.a -o exp2X
*/

#include "pi.h"
#include "natural.h"

#ifdef _Old_STD_
# include <fstream.h>
#else
# include <fstream>
using namespace std;
#endif

#include <time.h>
#include <string.h>

#define LOG(a)  { cout << a << flush; { ofstream lout("../exp.log", ios::app); lout << a; } }
#define LOGLN(a)  { cout << a << endl; { ofstream lout("../exp.log", ios::app); lout << a << endl; } }



template <class T>
struct matrix {
  T p[4];

  matrix() { p[0] = p[1] = p[2] = p[3] = 0; }
  matrix(const matrix& a) { *this = a; }

  matrix& operator=(const matrix& a) {
    p[0] = a.p[0]; p[1] = a.p[1]; p[2] = a.p[2]; p[3] = a.p[3];
    return *this;
  }

  matrix& operator=(const T& a) {
    p[0] = a; p[1] = 0; p[2] = 0; p[3] = a;
    return *this;
  }

  matrix& operator+=(const matrix& a) {
    p[0] += a.p[0];
    p[1] += a.p[1];
    p[2] += a.p[2];
    p[3] += a.p[3];
    return *this;
  }

  matrix& operator-=(const matrix& a) {
    p[0] -= a.p[0];
    p[1] -= a.p[1];
    p[2] -= a.p[2];
    p[3] -= a.p[3];
    return *this;
  }

  matrix& operator*=(const matrix& a);

  matrix& operator*=(const T& a) { p[0] *= a; p[1] *= a; p[2] *= a; p[3] *= a; return *this; }

  void transpose() { T t = p[1]; p[1] = p[2]; p[2] = t; }
};

template <class T>
inline OSTREAM& operator<<(OSTREAM& out, const matrix<T>& a)
{
  return out << '(' << a.p[0] << ',' << a.p[1] << ',' << a.p[2] << ',' << a.p[3] << ')' << endl;
}

template <class T>
inline bool operator==(const matrix<T>& a, const matrix<T>& b)
{
  return (a.p[0] == b.p[0] && a.p[1] == b.p[1] && a.p[2] == b.p[2] && a.p[3] == b.p[3]);
}

template <class T>
inline bool operator==(const matrix<T>& a, const T& b)
{
  return (a.p[0] == b && a.p[1] == 0 && a.p[2] == 0 && a.p[3] == b);
}

template <class T>
inline bool operator!=(const matrix<T>& a, const T& b)
{
  return (a.p[0] != b || a.p[1] != 0 || a.p[2] != 0 || a.p[3] != b);
}

template <class T>
matrix<T>& matrix<T>::operator*=(const matrix<T>& a)
{
  if (this == &a) {
    // T commutative:
    const T t = p[0]+p[3];
    p[0] *= p[0];
    const T s = p[1]*p[2];
    p[0] += s;
    p[1] *= t; p[2] *= t;
    p[3] *= p[3];
    p[3] += s;
  } else {
/*
    T u = p[0];
    T s = p[0]*a.p[0];
    T t = p[1]*a.p[2];
    p[0] = s+t;
    s = u*a.p[1];
    t = p[1]*a.p[3];
    p[1] = s+t;
    u = p[2];
    s = p[2]*a.p[0];
    t = p[3]*a.p[2];
    p[2] = s+t;
    s = u*a.p[1];
    t = p[3]*a.p[3];
    p[3] = s+t;
*/
    p[2] = p[0] - p[2];
    T h4 = a.p[3] - a.p[1];
    T h5 = p[2] * h4;
    p[2] = p[3] - p[2];
    h4 += a.p[0];
    T h = p[2] + p[0];
    T h2 = a.p[3] - h4;
    T h3 = h * h2;
    h2 = p[2] * h4;
    h4 -= a.p[2];
    h = p[3] * h4;
    p[2] = p[1] - p[2];
    p[3] = p[2] * a.p[3];
    p[2] = p[0] * a.p[0];
    p[0] = p[1] * a.p[2];
    p[0] += p[2];
    p[2] += h2;
    h3 += p[2];
    p[2] += h5;
    p[1] = p[3] + h3;
    p[3] = h5 + h3;
    p[2] -= h;
  }
  return *this;
}


class Exp {
private:
  Fixed exp;

  void split(Digit k, const Digit n, matrix<Integer>& z) const;
  void split(Digit k, const Digit n, Natural& p, Natural& q) const;

public:
  Exp(const size_t);
  Exp(const bool, const size_t, const size_t, const size_t, const Digit);

  const Fixed& value() const;
  size_t decimals() const;
};


inline OSTREAM& operator<<(OSTREAM& out, const Exp& a)
// Algorithm:  o := operator<<(o, a)
// Input:      o in ostream, a in Exp.
// Output:     o in ostream ||
//
// Note:       puts Exp a on output stream.
{
  return out << a.value();
}

inline const Fixed& Exp::value() const
{
  return exp;
}

inline size_t Exp::decimals() const
{
  return exp.decimals();
}




void Exp::split(Digit k, const Digit n, matrix<Integer>& z) const
{
//  CONDITION(n > k+2);

  if (n-k < 50) {
    if (k == 0) {
      z.p[0] = 3; z.p[1] = 1; z.p[2] = 2; z.p[3] = 1;
      ++k;
    } else if (k&1) {
      z.p[0] = 8*k-4; z.p[1] = 1; z.p[2] = 1; z.p[3] = 0;
    } else {
      z.p[0] = 2*k-1; z.p[1] = 1; z.p[2] = 1; z.p[3] = 0;
    }
    Integer t;
    while (++k <= n) {
      if (k&1) {
        t = z.p[1];
        z.p[1] = z.p[0];
        z.p[0] *= 8*k-4; z.p[0] += t;
        t = z.p[3];
        z.p[3] = z.p[2];
        z.p[2] *= 8*k-4; z.p[2] += t;
      } else {
        t = z.p[1];
        z.p[1] = z.p[0];
        z.p[0] *= 2*k-1; z.p[0] += t;
        t = z.p[3];
        z.p[3] = z.p[2];
        z.p[2] *= 2*k-1; z.p[2] += t;
      }
    }
  } else {
    const Digit m = (k+n)/2;
    matrix<Integer> a;
    split(k, m, z);
    split(m+1, n, a);
    z *= a;
  }
}

void Exp::split(Digit k, const Digit n, Natural& p, Natural& q) const
{
//  CONDITION(n > k+3);

  if (n-k < 50) {
    matrix<Natural> z;
    if (k == 0) {
      z.p[0] = 3; z.p[1] = 1; z.p[2] = 2; z.p[3] = 1;
      ++k;
    } else if (k&1) {
      z.p[0] = 8*k-4; z.p[1] = 1; z.p[2] = 1; z.p[3] = 0;
    } else {
      z.p[0] = 2*k-1; z.p[1] = 1; z.p[2] = 1; z.p[3] = 0;
    }
    Natural t;
    while (++k < n) {
      if (k&1) {
        t = z.p[1];
        z.p[1] = z.p[0];
        z.p[0] *= 8*k-4; z.p[0] += t;
        t = z.p[3];
        z.p[3] = z.p[2];
        z.p[2] *= 8*k-4; z.p[2] += t;
      } else {
        t = z.p[1];
        z.p[1] = z.p[0];
        z.p[0] *= 2*k-1; z.p[0] += t;
        t = z.p[3];
        z.p[3] = z.p[2];
        z.p[2] *= 2*k-1; z.p[2] += t;
      }
    }
    if (k&1) {
      p = z.p[0] * (8*k-4); p += z.p[1];
      q = z.p[2] * (8*k-4); q += z.p[3];
    } else {
      p = z.p[0] * (2*k-1); p += z.p[1];
      q = z.p[2] * (2*k-1); q += z.p[3];
    }
  } else {
    const Digit m = (k+n)/2;
    matrix<Integer> z;
    split(k, m, z);
    split(m+1, n, p, q);

    Natural t = abs(z.p[0])*p;
    Natural s = abs(z.p[2])*p;
    p = t; p += t = abs(z.p[1])*q;
    t = abs(z.p[3])*q;
    q = s+t;
  }
}


static void format(bool point)
{
  ifstream fin("exp.tmp");
  ofstream fout("exp-f.tmp");
  int i = 0;
  char c;
  if (point)
    while (!fin.eof()) {
      if (++i == 2) { fout << '.'; continue; }
      if (!fin.get(c)) break;
      fout.put(c);
      if (i == 78) { fout << "\\\n"; i = 0; break; }
    }
  while (!fin.eof()) {
    if (!fin.get(c)) break;
    fout.put(c);
    if (++i == 78) { fout << "\\\n"; i = 0; }
  }
}


Exp::Exp(const size_t n)
// Algorithm:  a := Exp(n)
// Input:      n in size_t.
// Output:     a in Exp such that |a-exp(1)*10^n| < 1 ||
 : exp(n)
{
  ifstream fszi("sz.tmp");
  char c;
  size_t d,sz;
  fszi >> d;
  fszi.get(c);
  fszi >> sz;
  fszi.close();
  if (c != ',' || sz != 0) {
    LOGLN("ERROR!");
    return;
  }
  exp.set_decimals(d);

  ifstream fin("exp-h.tmp");
  if (!fin) {
    fin.close();
    fin.open("exp1.tmp");
    exp.value().scan(fin);
    Natural q = 3;
    q.lmove(exp.precision());
    exp.value() -= q;

    ofstream fout("exp-h.tmp");
    fout << exp.precision() << ',' << exp.decimals() << ',';
    fout << print(exp.value());
    return;
  }
  size_t x;
  fin >> x;
  if (x != exp.precision()) { LOG("Note (different BETA)!\n"); }
  fin.get(c);
  fin >> x;
  if (x != exp.decimals()) { LOG("Error!\n"); return; }
  fin.get(c);
  if (!exp.value().scan(fin)) { LOG("Error!\n"); return; }
  fin.close();
}

Exp::Exp(const bool mat, const size_t n, const size_t idx, const size_t max, const Digit st)
 : exp(n)
{
  const size_t SPLIT_QUOTIENT = 2;

  time_t start,stop;
  if (idx < max) {
    const size_t m = n;
    const Digit k = m/max;
    const Digit i = k*idx;

    if (mat) {
      matrix<Integer> z;
      start = time(0);
      if (i+2*k > m) {
        LOGLN("process_m:" << idx << ", from " << ((st)? st : ((i)? i+1 : 0)) << " to " << m);
        split((st)? st : ((i)? i+1 : 0), m, z);
      } else {
        LOGLN("process_m:" << idx << ", from " << ((st)? st : ((i)? i+1 : 0)) << " to " << i+k);
        split((st)? st : ((i)? i+1 : 0), i+k, z);
      }
      stop = time(0);
      LOGLN("exp time [s] = " << difftime(stop, start));

      ofstream foutp0("exp-p0.tmp");
      foutp0 << print(z.p[0]);
      foutp0.close();

      ofstream foutp1("exp-p1.tmp");
      foutp1 << print(z.p[1]);
      foutp1.close();

      ofstream foutp2("exp-p2.tmp");
      foutp2 << print(z.p[2]);
      foutp2.close();

      ofstream foutp3("exp-p3.tmp");
      foutp3 << print(z.p[3]);
      foutp3.close();
    } else {
      Natural p,q;
      start = time(0);
      if (i+2*k > m) {
        LOGLN("process:" << idx << ", from " << ((st)? st : ((i)? i+1 : 0)) << " to " << m);
        split((st)? st : ((i)? i+1 : 0), m, p, q);
      } else {
        LOGLN("process:" << idx << ", from " << ((st)? st : ((i)? i+1 : 0)) << " to " << i+k);
        split((st)? st : ((i)? i+1 : 0), i+k, p, q);
      }
      stop = time(0);
      LOGLN("exp time [s] = " << difftime(stop, start));

      ofstream foutp("exp-p.tmp");
      foutp << print(p);
      foutp.close();

      ofstream foutq("exp-q.tmp");
      foutq << print(q);
      foutq.close();
    }
  } else if (idx == max) {
    Natural p,q;
    ifstream fin("exp-p.tmp");
    p.scan(fin);
    fin.close();
    fin.open("exp-q.tmp");
    q.scan(fin);
    fin.close();

    exp.set_decimals(size_t(log2(p)*0.602059991328));

    LOGLN("decimals=" << decimals());

    p <<= 2;
    size_t sz = BETA*exp.precision();
    const size_t s = log2(q)/SPLIT_QUOTIENT;
    exp.value() = 0;

    LOGLN("process division: " << sz << ',' << s);
    start = time(0);

#ifdef DIV_DIRECT
    p.lmove(exp.precision());
    exp.value() = p/q;
    sz = 0;
#elif DIV_SPLIT
    while (sz > s) {
      p <<= s;
      x = p/q;           // div(p, q, x, p);
      const Natural r = x*q;
      p -= r;
      exp.value() <<= s; exp.value() += x;
      sz -= s;
    }
#else
    if (sz > s) {
      p <<= s;
      exp.value() = p/q;
    } else if (sz) {
      p <<= sz;
      Natural x = p/q;
      exp.value() <<= sz; exp.value() += x;
      sz = 0;
    }
#endif

    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream fsz("sz.tmp");
    fsz << decimals() << ',' << sz << '.';
    fsz.close();

    if (sz) {
      ofstream fout("exp-p1.tmp");
      fout << print(p);
      fout.close();

#ifndef NO_DIRECT_DIV
      fout.open("x.tmp");
      fout << print(exp.value());
      fout.close();
#endif
      Natural x;
      fout.open("exp1.tmp");
      fout << x;
      fout.close();
    } else {
      ofstream fout("exp1.tmp");
      fout << print(exp.value());
      fout.close();

      q = 3; q.lmove(exp.precision());
      exp.value() -= q;

      fout.open("exp-h.tmp");
      fout << exp.precision() << ',' << exp.decimals() << ',';
      fout << print(exp.value());
      fout.close();
    }
  } else {
    char c;
    size_t d,sz = 0;
    Natural p,q,x;
    ifstream fin("sz.tmp");
    fin >> d;
    fin.get(c);
    fin >> sz;
    if (c != ',' || d == 0) {
      LOGLN("ERROR!");
      return;
    }
    if (sz == 0) {
      LOGLN("READY!");
      return;
    }
    fin.get(c);
    fin.close();

    fin.open("exp-p1.tmp");
    p.scan(fin);
    fin.close();
    fin.open("exp-q.tmp");
    q.scan(fin);
    fin.close();

    const size_t s = log2(q)/SPLIT_QUOTIENT;

    exp.set_decimals(d);

    if (c == '.') {  // combine
      fin.open("exp1.tmp");
      exp.value().scan(fin);
      fin.close();
      fin.open("x.tmp");
      x.scan(fin);
      fin.close();

      LOGLN("combine: " << sz << ',' << s);
      start = time(0);

      const Natural r = x*q;
      p -= r;
      exp.value() <<= s; exp.value() += x;
      sz -= s;

      stop = time(0);
      LOGLN("exp time [s] = " << difftime(stop, start));

      if (sz) {
        ofstream fout("exp1.tmp");
        fout << print(exp.value());
      }
    } else {
      if (sz <= s) {
        fin.open("exp1.tmp");
        exp.value().scan(fin);
        fin.close();
      }

      LOGLN("process division: " << sz << ',' << s);
      start = time(0);

      if (sz > s) {
        p <<= s;
        x = p/q;
      } else if (sz) {
        p <<= sz;
        x = p/q;
        exp.value() <<= sz; exp.value() += x;
        sz = 0;
      }

      stop = time(0);
      LOGLN("exp time [s] = " << difftime(stop, start));

#ifndef NO_DIRECT_DIV
      if (sz) {
        ofstream foutx("x.tmp");
        foutx << print(x);
      }
#endif
    }

    if (sz) {
      ofstream fout("exp-p1.tmp");
      fout << print(p);
      fout.close();
    } else {
//      q = 3; q <<= BETA*exp.precision();
//      exp.value() -= q;

      ofstream fout("exp-h.tmp");
      fout << exp.precision() << ',' << exp.decimals() << ',';
      fout << print(exp.value());
      fout.close();
    }

    ofstream fout("sz.tmp");
    fout << decimals() << ',' << sz;
    if (c != '.') fout << '.';
    fout.close();
  }
}

template <class T>
static size_t read(const char* path, const char* name, T& q, const bool prec, size_t& decimals)
{
  char str[100];
  ifstream fin(strcat(strcpy(str, path), name));
  LOG(str << " read...");

  size_t precision = 0;
  if (prec) {
    char c;
    fin >> precision;
    fin.get(c);
    fin >> decimals;
    fin.get(c);
  }
  if (q.scan(fin)) { LOGLN("...ok!"); }
  else { LOG("...ERROR!\n"); }
  return precision;
}

static void combine(const char* path1, const char* path2, int idx)
{
  time_t start,stop;

  switch (idx) {
  case 1: {
    matrix<Integer> a,z;
    size_t x;
    read(path1, "exp-p0.tmp", z.p[0], false, x);
    read(path1, "exp-p1.tmp", z.p[1], false, x);
    read(path1, "exp-p2.tmp", z.p[2], false, x);
    read(path1, "exp-p3.tmp", z.p[3], false, x);
    read(path2, "exp-p0.tmp", a.p[0], false, x);
    read(path2, "exp-p1.tmp", a.p[1], false, x);
    read(path2, "exp-p2.tmp", a.p[2], false, x);
    read(path2, "exp-p3.tmp", a.p[3], false, x);

    LOGLN("combine1");
    start = time(0);
    z *= a;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutp0("exp-p0.tmp");
    foutp0 << print(z.p[0]);
    foutp0.close();

    ofstream foutp1("exp-p1.tmp");
    foutp1 << print(z.p[1]);
    foutp1.close();

    ofstream foutp2("exp-p2.tmp");
    foutp2 << print(z.p[2]);
    foutp2.close();

    ofstream foutp3("exp-p3.tmp");
    foutp3 << print(z.p[3]);
    foutp3.close();
    break;
  }
  case 2: {
    matrix<Integer> z;
    Natural p,q;

    size_t x;
    read(path1, "exp-p0.tmp", z.p[0], false, x);
    read(path1, "exp-p1.tmp", z.p[1], false, x);
    read(path1, "exp-p2.tmp", z.p[2], false, x);
    read(path1, "exp-p3.tmp", z.p[3], false, x);
    read(path2, "exp-p.tmp", p, false, x);
    read(path2, "exp-q.tmp", q, false, x);

    LOGLN("combine2");
    start = time(0);

    Natural t = abs(z.p[0])*p;
    Natural s = abs(z.p[2])*p;
    p = t; p += t = abs(z.p[1])*q;
    t = abs(z.p[3])*q;
    q = s+t;

    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutp("exp-p.tmp");
    foutp << print(p);
    foutp.close();

    ofstream foutq("exp-q.tmp");
    foutq << print(q);
    foutq.close();
    break;
  }
  case 3: {
    Natural z0,p,q;

    size_t x;
    read(path1, "exp-p0.tmp", z0, false, x);
    read(path2, "exp-p.tmp", p, false, x);

    LOGLN("combine3");
    start = time(0);

    q = z0*p;

    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutp("exp-p.tmp");
    foutp << print(q);
    foutp.close();
    break;
  }
  case 4: {
    Natural z1,p,q;

    size_t x;
    read(path1, "exp-p1.tmp", z1, false, x);
    read("", "exp-p.tmp", p, false, x);
    read(path2, "exp-q.tmp", q, false, x);

    LOGLN("combine4");
    start = time(0);

    Natural t = z1*q;
    p += t;

    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutp("exp-p.tmp");
    foutp << print(p);
    foutp.close();
    break;
  }
  case 5: {
    Natural p,q,z2;

    size_t x;
    read(path1, "exp-p2.tmp", z2, false, x);
    read(path2, "exp-p.tmp", p, false, x);

    LOGLN("combine5");
    start = time(0);

    q = z2*p;

    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutq("exp-q.tmp");
    foutq << print(q);
    foutq.close();
    break;
  }
  case 6: {
    Natural q,q2,z3;

    size_t x;
    read(path1, "exp-p3.tmp", z3, false, x);
    read(path2, "exp-q.tmp", q, false, x);
    read("", "exp-q.tmp", q2, false, x);

    LOGLN("combine6");
    start = time(0);

    Natural t = z3*q;
    q2 += t;

    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutq("exp-q.tmp");
    foutq << print(q2);
    foutq.close();
    break;
  }
  default: LOG("ERROR!\n");
  }
}

/*
n = 10000:
decimals=83339      (8,3339)

n = 50000:
decimals=486654     (9,73)

n = 100000: (5 MB)
decimals=1033532    (10,33532)

n = 1000000: (42 MB)
decimals=12335507   (12,335507)
*/

int main(int argc, char** argv)
{
  size_t i,j,n;
  Digit  m = 0;
  bool   matrix = false;

  for (i = 0; i < argc; ++i) LOG(argv[i] << ' ');

  if (argc == 4) {
    n = atoi(argv[1]);
    combine(argv[2], argv[3], n);
    return 0;
  } else if (argc == 5) {
    matrix = bool(atoi(argv[1]));
    n = atoi(argv[2]);
    i = atoi(argv[3]);
    j = atoi(argv[4]);
  } else if (argc == 6) {
    matrix = bool(atoi(argv[1]));
    n = atoi(argv[2]);
    i = atoi(argv[3]);
    j = atoi(argv[4]);
    m = atoi(argv[5]);
  } else if (argc == 2) {
    Exp z(atoi(argv[1]));
    time_t start,stop;
    ofstream fout("exp.tmp");
    start = time(0);
    fout << z << endl;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));
    fout.close();
    format(false);
    return 0;
  } else {
    cout << "useage:  exp <bool> <iterations> <step> <parts> [start]\n";
    cout << "         exp <bool> <iterations> <step> <parts>\n";
    cout << "         exp <iterations>\n";
    cout << "      or exp <idx> <path1> <path2>\n";
    return 1;
  }

  Exp z(matrix, n, i, j, m);

  return 0;
}
