#ifndef FILTER_HH
#define FILTER_HH

/*
 * @brief:  this file defines the image filters used in image processing
 * @auther: chengaoyu2013@gmail.com
 * @date:   2017/07/10
*/
#include "boost/math/special_functions/bessel.hpp"
#include "./Geometry/grid.h"
namespace BBSLMIRP {

class Filter{
public:
    Filter();
    virtual ~Filter();
    virtual Grid3D Kaiser(const Block& bk, float alpha,int order,float radius);
private:
};


}


#endif // IMGFILTERS_HH
