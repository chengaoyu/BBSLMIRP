#ifndef ROTATION_H
#define ROTATION_H
/*
  @brief:  this file gives the class declaration of the rotation along the cordinates(x,y,z).
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "point.h"
namespace BBSLMIRP{

const float kPi = std::acos(-1);
enum class Axis{
    x = 1,
    y = 2,
    z = 3
};

class Rotation{
public:
    Rotation();
    virtual ~Rotation();
    virtual void set_rotation_matrix(float radian, Axis);
//    virtual void RotateByX(float radian);
//    virtual void RotateByY(float radian);
//    virtual void RotateByZ(float radian);
    virtual void Reset();
    virtual Point3D Rotate(const Point3D& pt) const;
    //virtual Rotation operator=(const Rotation& r) const;
private:
    float rotation_matrix[3][3];
    // rotation_matrix
    // [0,0][0,1][0,2]
    // [1,0][1,1][1,2]
    // [2,0][2,1][2,2]
};


}



#endif  //ROTATION_H
