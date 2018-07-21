#ifndef SPHERICALPOINT_H
#define SPHERICALPOINT_H
/*
  @brief:  this file gives the class declaration of point in 3D sphercial cordinate space.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
 */

#include<iostream>
//class Point3D;

namespace BBSLMIRP {
class SphericalPoint3D {
public:
    SphericalPoint3D();
    SphericalPoint3D(float radius, float theta, float phi);
    virtual ~SphericalPoint3D();

    //set the three coordinate of the point.
    virtual void set_radius(float radius);
    virtual void set_theta(float theta);
    virtual void set_phi(float phi);
    //getter
    virtual float get_radius() const;
    virtual float get_theta() const;
    virtual float get_phi() const;
    // calculate the distance between two points
    //virtual float calculateDist(const SphericalPoint3D& spt1) const;
    //const SphericalPoint3D& getSPT();
    //transform a point in spherical coordinate system to a point in rectangular coordinate system.
    virtual void SetPoint(float radius, float theta, float phi);
    //virtual Point3D SPT2PT() const;

private:
    float radius;
    float theta;
    float phi;
};
}



#endif //SPHERICALPOINT_H
