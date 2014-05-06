#ifndef MAPVIS_H
#define MAPVIS_H

namespace ralab
{
  /// map access interface for visualization
  struct MapVis
  {
    virtual ~MapVis(){}
    virtual std::size_t getMZsize() = 0;
    virtual std::size_t getRTsize() = 0;
    virtual float get(std::size_t px, std::size_t py) = 0;
    virtual float getMaxelem() = 0;
    virtual float getMinelem() = 0;
  };
}//end namespace mapvis
#endif // MAPVIS_H
