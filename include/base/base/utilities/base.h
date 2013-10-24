#ifndef BASEUTILITIES_H
#define BASEUTILITIES_H


namespace ralab
{
  namespace base
  {
    namespace base
    {
      namespace utilities
      {
        // class generator:
        template<typename TReal>
        struct SeqPlus {
          TReal m_from;
          TReal m_by;

          SeqPlus(TReal from)
            : m_from(from), m_by(1)
          {}
          TReal operator()()
          {
            TReal current = m_from ;
            m_from += m_by;
            return current;
          }
        };

        template<typename TReal>
        struct SeqMinus {
          TReal m_from;
          TReal m_by;

          SeqMinus(TReal from)
            : m_from(from), m_by(1)
          {}
          TReal operator()()
          {
            TReal current = m_from ;
            m_from -= m_by;
            return current;
          }
        };
      }
    } //base
  } //base
}//ralab

#endif // BASE_H
