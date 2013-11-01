// Copyright : ETH Zurich
// License   : three-clause BSD license
// Authors   : Witold Wolski
// for full text refer to files: LICENSE, AUTHORS and COPYRIGHT

#include "map2test.h"
#include <iostream>
#include "findmf/datastruct/lcmsimage.h"




/** test an explain LCMSImage interface. */
Map2test::Map2test()
{
  ralab::findmf::datastruct::LCMSImagePtr ip = ralab::findmf::datastruct::LCMSImagePtr( new ralab::findmf::datastruct::LCMSImage() );
  try{
    ip->read("../data/napedro_L120420_009_SWfitered.tiff");
    std::cout << ip->getRTsize() << " " << ip->getMZsize() << " " << ip->getNrCols()
              << " " << ip->getNrRows() << std::endl;
  }
  catch(std::exception & e)
  {
    std::cout << e.what() << std::endl;
  }
}


struct A : private std::string{
  A(const char * bla):std::string(bla){}
  operator std::string() {
    return *this;
  }

  bool operator ==(const char * c)
  {
    return *this == c;
  }

  bool operator !=(const char * c){
      return !operator ==(c);
  }
};

struct B {
  std::string member_;
   B(const char * bla):member_(bla){}
  operator std::string  ()
  {
    return member_;
  }
};

//main method
int main(int argc, char *argv[])
{


  Map2test mp;
  std::cout << "bla bla" << std::endl;

  B x("testtest");
  if(static_cast<std::string>(x) != "")
  {
    std::cout << "hello world!" << std::endl;
  }

  A y("testtest");
  if(y != "")
  {
    std::cout << "hello world!" << std::endl;
  }
}
