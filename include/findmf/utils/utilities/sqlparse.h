#ifndef SQLPARSE_H
#define SQLPARSE_H

namespace ralab{
  namespace findmf{
    namespace utils{
      namespace utilities{

        //match begin of line or empty line
        struct beginMatch : std::unary_function<bool,std::string>{
          std::string begstatement_;
          beginMatch(const std::string & linebeg ):begstatement_(linebeg){}
          bool operator()(const std::string & bla)
          {
            if( bla.length() == 0 || bla.compare( 0,3,begstatement_.c_str() ) == 0)
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

        //merge str til delimiter
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
}
#endif // SQLPARSE_H

