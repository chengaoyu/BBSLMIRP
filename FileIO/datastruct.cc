/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "datastruct.h"
#include "../Geometry/ray.h"

namespace BBSLMIRP {

//const double kLightSpeed = 3e11;

//TOF event
Event::Event(const Point3D& p1, const Point3D& p2, float tof_distance){
    ray = new Ray(p1,p2);
    this->set_tof_distance(tof_distance);
}

Event::~Event(){
    delete ray;
    ray = NULL;
}

float Event::get_length_mint_to_center()const {
    return (ray->get_length()*0.5 - get_tof_distance() -ray->get_min_t());
}



}

