#include "pi.h"
#include "natural.h"

#ifdef _Old_STD_
# include <fstream.h>
#else
# include <fstream>
#endif

#include <string.h>
#include <time.h>
#include <math.h>



#define LOG(a)  { cout << a << flush; { ofstream lout("../exp.log", ios::app); lout << a; } }
#define LOGLN(a)  { cout << a << endl; { ofstream lout("../exp.log", ios::app); lout << a << endl; } }


class Exp {
private:
  Fixed exp;

  void series(Digit, const Digit, Natural&, Natural&) const;

public:
  Exp(const size_t);
  Exp(const size_t, const size_t, const size_t, const Digit);

  const Fixed& value() const;

  friend ostream& operator<<(ostream&, const Exp&);
};


inline ostream& operator<<(ostream& out, const Exp& a)
// Algorithm:  o := operator<<(o, a)
// Input:      o in ostream, a in Exp.
// Output:     o in ostream ||
//
// Note:       puts Exp a on output stream.
{
  return out << a.exp;
}

inline const Fixed& Exp::value() const
{
  return exp;
}


void Exp::series(Digit n, const Digit m, Natural& q, Natural& t) const
// n < 2^BETA/72-24
{
  // CONDITION(n < m);
  switch (m-n) {
    case 1: {
      q = (n == 0)? 1 : n;
      t = 1;
      break;
    }
    case 2: {
      const Digit l = (n+m)/2;
      if (n == 0) q = l;
      else if (l < GAMMA_LOW) q = n*l;
      else { q = n; q *= l; }
      t = l+1;
      break;
    }
    case 3: {
      const Digit l = (n+m)/2;
      const Digit l2 = (l+m)/2;
      if (l2 < GAMMA_LOW) {
        const Digit x = l*l2;
        t = x; q = x;
      } else {
        t = l; t *= l2; q = t;
      }
      if (n != 0) q *= n;
      t += l2+1;
      break;
    }
    default: {
      Natural q1,t1;
      Natural q2,t2;

      const Digit l = (n+m)/2;
      series(n, l, q1, t1);
      series(l, m, q2, t2);
      t = t1*q2; t += t2;
      q = q1*q2;
      break;
    }
  }
/*
  if (n+1 == m) {
    if (n == 0) { q = 1; t = 1; }
    else {
      q = n;
      t = 1;
    }
  } else {
    Natural q1,t1;
    Natural q2,t2;

    const Digit l = (n+m)/2;
    series(n, l, q1, t1);
    series(l, m, q2, t2);
    t = t1*q2; t += t2;
    q = q1*q2;

    const Digit k = m-n;
    if (k > 8 && k <= 16) {
      q1 = gcd(t, q);
      q /= q1; t /= q1;
    }
  }*/
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
 : exp(n)
{
  ifstream fin("exp.tmp");
  size_t x;
  fin >> x;
  if (x != exp.precision()) { LOG("Note (different BETA)!\n"); }
  char c;
  fin.get(c);
  fin >> x;
  if (x != exp.decimals()) { LOG("Error!\n"); return; }
  fin.get(c);
  if (!exp.value().scan(fin)) { LOG("Error!\n"); return; }
}

Exp::Exp(const size_t n, const size_t idx, const size_t max, const Digit st)
 : exp(n)
{
  time_t start,stop;
  const size_t sz = exp.precision();

  Natural q,t;
  if (idx < max) {
    // n < (m + 1/2)*log10(m) - 11/25*m
    Digit m = Digit(n/4.239174208864);
    Digit m2 = m/2;
    Digit d = Digit(log10(m) * (m+0.5) - (m/25.0)*11);
    while (d <= n) {
      m *= 2;
      d = Digit(log10(m) * (m+0.5) - (m/25.0)*11);
    }
    Digit d2 = Digit(log10(m2) * (m2+0.5) - (m2/25.0)*11);
    while (d2 > n) {
      m2 /= 2;
      d2 = Digit(log10(m2) * (m2+0.5) - (m2/25.0)*11);
    }
    while (m-m2 > 1) {
      Digit m3 = (m+m2)/2;
      Digit d3 = Digit(log10(m3) * (m3+0.5) - (m3/25.0)*11);
      if (d3 > n) m = m3;
      else m2 = m3;
    }
    const Digit k = m/max;
    const Digit i = k*idx;

    start = time(0);
    if (i+2*k > m) {
      LOGLN("process:" << idx << ", from " << ((st)? st : i) << " to " << m);
      series((st)? st : i, m, q, t);
    } else {
      LOGLN("process:" << idx << ", from " << ((st)? st : i) << " to " << i+k);
      series((st)? st : i, i+k, q, t);
    }
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutq("exp-q.tmp");
    foutq << print(q);
    foutq.close();

    ofstream foutt("exp-t.tmp");
    foutt << print(t);
    foutt.close();
  } else {
    ifstream finq("exp-q.tmp");
    ifstream fint("exp-t.tmp");
    q.scan(finq);
    t.scan(fint);

    LOGLN("process division");
    start = time(0);
    size_t i = 0;
    if (q != 0)
      while (!q.testbit(i)) ++i;
    q >>= i; t <<= BETA*sz-i;
    exp.value() = t / q;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));
    ofstream fout("exp.tmp0");
    fout << exp.precision() << ',' << exp.decimals() << ',';
    fout << print(exp.value());
  }
}

static size_t read(const char* path, const char* name, Natural& q, const bool prec, size_t& decimals)
{
  char str[100];
  ifstream fin(strcat(strcpy(str, path), name));
  LOG(name << " read...");

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
    Natural q,t,t1;
    size_t x;
    read(path1, "exp-t.tmp", t1, false, x);
    read(path2, "exp-q.tmp", q, false, x);
    read(path2, "exp-t.tmp", t, false, x);

    LOGLN("combine1");
    start = time(0);
    t += t1*q;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream fout("exp-t.tmp");
    fout << print(t);
    break;
  }
  case 2: cout << "ERROR!" << endl; break;
  case 3: {
    Natural q,q1;
    size_t x;
    read(path1, "exp-q.tmp", q1, false, x);
    read(path2, "exp-q.tmp", q, false, x);

    start = time(0);
    LOGLN("combine" << idx);
    q *= q1;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutq("exp-q.tmp");
    foutq << print(q);
    break;
  }
  case 4: {
    Natural tq,q;
    size_t decimals,x;
    size_t precision = read(path2, "", tq, true, decimals);
    read(path1, "exp-q.tmp", q, false, x);

    LOGLN("combine" << idx);
    start = time(0);
    tq /= q;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutq("exp.tmp2");
    foutq << precision << ',' << decimals << ',' << print(tq);
    break;
  }
  case 5: {
    Natural s1,s2;
    size_t decimals;
    size_t precision = read(path1, "exp.tmp0", s1, true, decimals);
    if (precision != read(path2, "exp.tmp2", s2, true, decimals)) {
      LOGLN("precision error!");
      return;
    }

    LOGLN("combine" << idx);
    start = time(0);
    s1 += s2;
    stop = time(0);
    LOGLN("exp time [s] = " << difftime(stop, start));

    ofstream foutq("exp.tmp");
    foutq << precision << ',' << decimals << ',' << print(s1);
    break;
  }
  default: LOG("ERROR!\n");
  }
}

int main(int argc, char** argv)
{
  size_t i,j,n;
  Digit  m = 0;

  for (i = 0; i < argc; ++i) LOG(argv[i] << ' ');

  if (argc == 4) {
    n = atoi(argv[1]);
    if (n >= 1 && n <= 5) {
      combine(argv[2], argv[3], n);
      return 0;
    }
    i = atoi(argv[2]);
    j = atoi(argv[3]);
  } else if (argc == 5) {
    n = atoi(argv[1]);
    i = atoi(argv[2]);
    j = atoi(argv[3]);
    m = atoi(argv[4]);
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
    cout << "usage:  exp <decimals> <step> <parts> [start]\n";
    cout << "        exp <decimals> <step> <parts>\n";
    cout << "        exp <decimals>\n";
    cout << "     or exp <idx> <path1> <path2>\n";
    return 1;
  }

  Exp z(n, i, j, m);

  return 0;
}
