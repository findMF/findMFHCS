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

///  used to visualize segementation.
struct MapVisMock : MapVis{
private:
    std::size_t x_;
    std::size_t y_;
public:
    MapVisMock(std::size_t x, std::size_t y):x_(x),y_(y){}

    std::size_t getMZsize() override
    {return x_;}

    std::size_t getRTsize() override
    {return y_;}

    virtual float get(std::size_t px, std::size_t py) override
    {
        return((float)px + (float)py);
    }
    virtual float getMaxelem(){return (float)(x_+y_);}
    virtual float getMinelem(){return 0.;}
};

}//end namespace mapvis
#endif // MAPVIS_H
