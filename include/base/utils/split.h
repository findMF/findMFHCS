

#ifndef SPLIT_H
#define SPLIT_H

#include <vector>
#include <string>

namespace ralab{
  namespace base{
    namespace utils{
      template<typename Toutit>
      void split(const std::string &s, const std::string & delimiter, Toutit elems) {
        size_t pos = 0;
        std::string token;
        std::string str(s);
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            *elems = token;
            ++elems;
            str.erase(0, pos + delimiter.length());
          }
        *elems =str;
      }




    }
  }
}
#endif // SPLIT_H
