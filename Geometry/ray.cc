/*
  @brief:  the definition of a ray 
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "ray.h"

namespace BBSLMIRP
{
//class Point;

/* class---Ray
 * function definitions
*/
Ray::Ray() : startpoint(Point3D(0, 0, 0)), endpoint(Point3D(1, 1, 1))
{
    this->Update();
}

Ray::Ray(const Point3D &pt1, const Point3D &pt2)
{
    this->SetRay(pt1,pt2);
}

Ray::~Ray()
{
}

void Ray::SetRay(const Point3D &pt1, const Point3D pt2)
{
    startpoint = pt1;
    endpoint = pt2;
    Update();
}

void Ray::SetTs(float mint, float maxt)
{
    min_t = mint;
    max_t = maxt;
}

void Ray::Update()
{
    ComputeLength();
    Point3D temp = endpoint - startpoint;
    this->direction = temp / this->length;
}

void Ray::ComputeLength()
{
    Point3D diff = startpoint - endpoint;
    length = diff.GetNorm2();

}
}
