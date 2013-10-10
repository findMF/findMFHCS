#ifndef SQLPARSE_H
#define SQLPARSE_H

#include <string>

///
/// misc functions for parsing sql files.
///
namespace ralab{
  namespace base{
    namespace misc{

        //find lines beginning with comment.
        struct beginMatch : std::unary_function<bool,std::string>{
          std::string comment_;
          beginMatch(const std::string & linebeg ):comment_(linebeg){}
          bool operator()(const std::string & bla)
          {
            if( bla.length() == 0 || bla.compare( 0,comment_.size(),comment_.c_str() ) == 0)
              return true;
            else
              return false;
          }
        };

        // remove comments from the end of line
        struct commentremover: std::unary_function<std::string, std::string>{
          std::string comment_;
          commentremover(const std::string & comment):comment_(comment){}

          std::string operator()(const std::string & str){
            return str.substr(0, str.find(comment_));
          }
        };

        //merge strings in first last til delimiter
        template<typename Tstrit, typename Toutstr>
        void strmerge( Tstrit first, Tstrit last,  Toutstr out, std::string delimiter)
        {
          size_t pos = 0;

          std::string statement;
          for(;first!= last; ++first){
              std::string str= *first;
              //delim not found
              if((pos = str.find(delimiter)) == std::string::npos){
                  statement += str;
                }
              else{
                  statement += str;
                  *out  = ( statement );
                  ++out;
                  statement.clear();
                }
            }
        }
      }//end utilities


  }
}
#endif // SQLPARSE_H

