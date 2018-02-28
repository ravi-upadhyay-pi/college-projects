# ifndef _SIGNAL_H
# define _SIGNAL_H

# include <vector>
# include <iostream>

using namespace std;

template <class T>
class Signal
{
private:
  T *dta;
  int len;
public:
  Signal(std::vector<T> &);
  Signal(T *, int);
  Signal(const Signal &);
  void print();
  void resize(int);
  void add(Signal &);
  void mult(Signal &);
  int  length() { return len; }
};

// Constructor (from a vector) //
template <class T>
Signal<T>::Signal(std::vector<T> &v)
{
  len = v.size();
  dta = new T[len];
  for(int i = 0; i < len; i++){
    dta[i] = v[i];
  }
}


// Constructor (from an array) //
template <class T>
Signal<T>::Signal(T *a, int length)
{
  len = length;
  dta = new T[len];
  for(int i = 0; i < len; i++){
    dta[i] = a[i];
  }
}

// Constructor (copy) //
template <class T>
Signal<T>::Signal(const Signal<T> &b)
{
  len = b.length();
  dta = new T[len];
  for(int i = 0; i < len; i++)
    dta[i] = b[i];
}

// Resize the signal //
template <class T>
void Signal<T>::resize(int length)
{
  T *t = new T[length];
  for(int i = 0; i < min(length, len); i++) t[i] = dta[i];
  if(length > len)
    for(int i = len; i < length; i++) t[i] = 0;
  delete []dta;
  dta = t;
  len = length;
}

// function to print the signal //
template <class T>
void Signal<T>::print()
{
  int i = 0;
  for(i = 0; i < len - 1; i++)
    std::cout << dta[i] << " ";
  std::cout << dta[i];
}

// function to add a signal to itself //
template <class T>
void Signal<T>::add(Signal<T> &b)
{
  for(int i = 0; i < len; i++) dta[i] += b.dta[i];
}

// function to multiply signal to itself //
template <class T>
void Signal<T>::mult(Signal<T> &b)
{
  for(int i = 0; i < len; i++) dta[i] *= b.dta[i];
}

# endif
