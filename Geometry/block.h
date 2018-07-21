#ifndef BLOCK_H
#define BLOCK_H

/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/
#include "block_interface.h"
#include "point.h"
#include "gridsize.h"

namespace BBSLMIRP {

typedef GridSize MeshIndex;
class Block : public Block_Interface
{
public:

    // locate the virtual crystal position in the block.
    virtual bool LocatePoint(Point3D& pt) const {} //when given the position of a point in space
    virtual bool LocatePoint(const MeshIndex& mi, Point3D& pt ) const {}//when given the mesh index in the block.
    virtual bool LocatePoint(int mi, Point3D &pt) const {}
    // compute the insect length of a Ray with a Block.(used in the model of detector attenuation)
    virtual bool ComputeInsection(const Ray& ray, float& insect_length) const {}
    // Describe the Block itself.
    virtual void DescribeSelf() const {}

    virtual void set_num_grid(const GridSize& num_grid){this->num_grid = num_grid;}
    virtual void set_geometry_size(const Point3D &geometry_size){this->geometry_size = geometry_size;}
    virtual void set_center(const Point3D &center){this->center = center;}
    virtual void set_top_bound(const Point3D & bound){this->top_bound = bound;}
    virtual void set_bottom_bound(const Point3D& bound){this->bottom_bound = bound;}
    virtual void set_offset(const Point3D& offset, int index){this->offset[index] = offset;}


    virtual const GridSize& get_num_grid() const {return num_grid;}
    virtual const Point3D&  get_geometry_size() const {return geometry_size;}
    virtual const Point3D&  get_center() const {return center;}
    virtual const Point3D&  get_interval() const{return interval;}
    virtual const Point3D&  get_top_bound() const{return top_bound;}
    virtual const Point3D&  get_bottom_bound() const{return bottom_bound;}
    virtual const Point3D&  get_offset(int index)const{return this->offset[index];}
    //calculate the number of meshes
    virtual int GetTotalMeshes() const {return num_grid.get_num_x()*num_grid.get_num_y()*num_grid.get_num_z();}

    virtual void Initialize(const GridSize& num_grid, const Point3D& geometry_size, const Point3D& center)
    {
        this->num_grid = num_grid;
        this->geometry_size = geometry_size;
        this->center=center;
        this->ComputeInterval();
        this->ComputeBounds();
        this->ComputeOffset();
    }
    virtual void ComputeInterval(){
        Point3D  num_grid_float(float(num_grid.get_num_x()),float(num_grid.get_num_y()),float(num_grid.get_num_z()));
        interval = geometry_size/num_grid_float;
    }
    virtual void ComputeBounds(){
        bottom_bound = center - (geometry_size/2);
        top_bound = bottom_bound + geometry_size;
    }
    virtual void ComputeOffset(){
        offset[0] = Point3D(interval.get_px(),0,0);
        offset[1] = Point3D(0,interval.get_py(),0);
        offset[2] = Point3D(0,0,interval.get_pz());

    }

private:
    GridSize num_grid;
    Point3D  geometry_size;
    Point3D  center;
    Point3D  interval;
    Point3D  bottom_bound;
    Point3D  top_bound; // the point of top vertex corresponding to the origin.
    Point3D  offset[3];
};
}


#endif // BLOCK_H
