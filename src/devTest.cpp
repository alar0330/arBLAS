#include <iostream>
#include <typeinfo>
#include <valarray>

#include "../inc/Matrix.hpp"

using namespace aarz;

using std::cout;
using std::endl;

using std::valarray;

static const char* devlog = "[devTest]: ";

int main( ) {

   cout << devlog << "test started \n";

   Matrix<double> mx1(4, 5);

   mx1(2, 2) = 2.99;
   cout << mx1;

   Matrix<double> mx2(mx1);

   mx2(0, 3) = 3.99;
   cout << mx2;

   Matrix<int> mx3(3, 3);

   cout << typeid(mx3).name();
   cout << mx3;

   mx3 = (Matrix<int>) (mx2);

   cout << typeid(mx3).name();
   cout << mx3;

   valarray<double> darr(1, 5);
   for (auto i : darr)
      cout << i << " ";

   valarray<int> iarr(5);



   cout << devlog << "test ended \n";
}