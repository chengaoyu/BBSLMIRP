#ifndef BLOCK_INTERFACE_H
#define BLOCK_INTERFACE_H

/*
  @brief:  the interface of a Block.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/



namespace BBSLMIRP {

class Point3D;
class Ray;
class GridSize;
typedef GridSize MeshIndex;

class Block_Interface{
public:
    // interface methods.

    // locate the virtual crystal position in the block.
    virtual bool LocatePoint(Point3D& pt) const = 0; //when given the position of a point in space
    virtual bool LocatePoint(const MeshIndex& mi, Point3D& pt ) const = 0;//when given the mesh index in the block.
    virtual bool LocatePoint(int mi, Point3D &pt) const = 0;
    // compute the insect length of a Ray with a Block.(used in the model of detector attenuation)
    virtual bool ComputeInsection(const Ray& ray, float& insect_length) const = 0;
    // Describe the Block itself.
    virtual void DescribeSelf() const = 0;

//    virtual Point3D CalculateInterval() const = 0;
//    //calculate the center of the first mesh in the block.
//    virtual Point3D CalculateOffset() const = 0;

//    //get the bound of this block.
//    virtual Point3D GetTopBound() const = 0;
//    virtual Point3D GetBottomBound() const = 0;

//    virtual int GetTotalMeshes() const = 0;

};

}


#endif // BLOCK_INTERFACE_H
