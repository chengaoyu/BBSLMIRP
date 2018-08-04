#ifndef OBLIQUEBLOCK_H
#define OBLIQUEBLOCK_H

/*
  @brief:  A vertical block is a concrete block class.
           And it is vertical in right hand coordinate system(its y-z plane is parallel to the z-axis).
           This block is usually used to construct a ring PET system.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/26
*/

#include "block.h"
#include "rotation.h"
namespace BBSLMIRP{

class ObliqueBlock : public Block{
public:
    ObliqueBlock();
    virtual ~ObliqueBlock();
    // locate a mesh in the block and return its position(a 3D point)
    virtual bool LocatePoint(const GridSize& mesh_index,Point3D& pt ) const override;   //when given the index of a mesh
    virtual bool LocatePoint( Point3D& pt) const override;       //when given the position of a point in space
    virtual bool LocatePoint(int mesh_index,Point3D& pt) const override;
    virtual bool ComputeInsection(const Ray &ray, float &insect_length) const override;
    virtual void DescribeSelf() const override;
    virtual void Initialize(const GridSize& num_grid, const Point3D& geometry_size, const Point3D& center, float phi, float theta);
};


}



#endif //OBLIQUEBLOCK_H
