#include "io.hpp"

#include <iostream>

int main()
{
  using namespace sadovnik;

  List< person_t > persons = readPersons(std::cin);
  writePersons(std::cout, persons);
  std::cerr << persons.size() << " 0\n";
  return 0;
}
