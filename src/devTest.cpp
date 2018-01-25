#include <iostream>

#include "../inc/Matrix.hpp"

using namespace aarz;

static const char* devlog = "[devTest]: ";

int main( ) {

   std::cout << devlog << "test started \n";

   Matrix<double> mx1(4, 5);

   mx1(2, 2) = 2;

   std::cout << mx1;

   Matrix<double> mx2(mx1);

   mx2(3, 3) = 3;

   std::cout << mx2;

   std::cout << devlog << "test ended \n";
}