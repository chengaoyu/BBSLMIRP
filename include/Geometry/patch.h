#ifndef PATCH_H
#define PATCH_H

/*
  @brief:  A patch is an irregular panel to construct complex PET scanner.
           A patch contain two parallel faces.
           A patch should be discretized and represent as a obliqueblock.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/

#include<vector>
#include "point.h"
#include "obliqueblock.h"
namespace BBSLMIRP {

typedef std::vector<Point3D> Face;
class ObliqueBlock;

struct angle_pair{
    int index;
    float angle;
};

class Patch{
public:
    Patch();
    virtual ~Patch();
    virtual void Reset();
    virtual bool Initialize(const Face& face1, const Face& face2);


    virtual const Face& get_inside_face()  const{return inside_face;}
    virtual const Face& get_outside_face() const{return outside_face;}
    virtual const Face& get_rotated_inside_face()  const{return rotated_inside_face;}
    virtual const Face& get_rotated_outside_face() const{return rotated_outside_face;}

    virtual const Rotation& get_rotate_z()const {return rotate_z;}
    virtual const Rotation& get_rotate_y()const {return rotate_y;}
    //virtual void set_normal(const Point3D& normal){ this->normal = normal;}
    virtual const Point3D& get_normal()const {return normal;}

    virtual void ComputeNormal();
    virtual ObliqueBlock CreateBlock(const Point3D& mesh_size) const;
    virtual void RotateToX();
    virtual void FindBlockSize(const Face& in_face, const Face& out_face,Point3D& bottom_bound, Point3D& top_bound) const;

    virtual bool IsInPatch(const Point3D& pt) const;


    static void BubbleSort(std::vector<angle_pair>& angle_list);
    static bool IsInPolygon(const Face& ordered_face, const Point3D& pt);
    static double cross(const Point3D& p0,const Point3D& p1,const Point3D& p2);
    static  Point3D SumVector(const Face &face);
    //check if the points of the face is on the same plane.
    static bool IsValidFace(const Face &face);
private:
    float theta;
    float phi;
    Point3D normal;
    //original faces
    Face inside_face;
    Face outside_face;

    // the rotation to rotate a patch to standard posture.
    Rotation rotate_z;
    Rotation rotate_y;
    //faces in standard posture.(perpendicular to x-axis)
    Face rotated_inside_face;
    Face rotated_outside_face;
};
}

#endif // PATCH_H
