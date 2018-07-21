#ifndef RAY_H
#define RAY_H
/*
  @brief:  this file gives the class declaration of a ray connect two points.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "point.h"

namespace BBSLMIRP
{

class Ray
{
  public:
    Ray();
    Ray(const Point3D &pt1, const Point3D &pt2);
    virtual ~Ray();
    //
    virtual void set_startpoint(const Point3D &pt){this->startpoint = pt;}
    virtual void set_endpoint(const Point3D &pt){this->endpoint = pt;}

    // get the two end points.
    virtual const Point3D &get_startpoint() const { return startpoint; }
    virtual const Point3D &get_endpoint() const { return endpoint; }
    // get the direction of this ray.
    virtual const Point3D &get_direction() const { return direction; }
    // get the length of this Ray.
    virtual float get_length() const { return length; }
    // get the min_t and max_t
    virtual float get_min_t() const { return min_t; }
    virtual float get_max_t() const { return max_t; }

    virtual void SetTs(float mint, float maxt);
    // update the dir and distance when the ends are changed.
    virtual void SetRay(const Point3D &pt1, const Point3D pt2);
    virtual void Update();
    virtual void ComputeLength();

  private:
    Point3D startpoint;
    Point3D endpoint;
    Point3D direction; // the unit vector from startPt to endPt.(direction cosines)
    float length;      // length between the two points.
    float min_t;       // the 1st initial point(t=0), reset by Intersect... routines.
    float max_t;       // the 2nd initial point(t = len), reset by Intersect... routines.
};
}

#endif //RAY_H
