/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/
#include "obliqueblock.h"
#include "ray.h"
#include "rotation.h"

namespace BBSLMIRP {

ObliqueBlock::ObliqueBlock(){

}
ObliqueBlock::~ObliqueBlock(){

}
void ObliqueBlock::Initialize(const GridSize& num_grid,
                               const Point3D& geometry_size,
                               const Point3D& center,
                               float phi,
                               float theta){

    // a block only rotate along z-axis (phi (0~360)) and y-axis(theta (0~180))
    // when the normal of a block is fixed, the two rotations are decided.

    // the block is generated with 4 steps (rotate along y-axis first, then z-axis)

    // step1: generate the block in original point.
    Block::Initialize(num_grid,geometry_size,center);

    // step2: rotate along y-axis if theta is not equal to 0;
    if(theta != 0){
        Rotation rotation_y;
        float rad_y = theta;
        rotation_y.set_rotation_matrix(rad_y-kPi/2,Axis::y);
        //rotate the block.
        this->set_bottom_bound(rotation_y.Rotate(this->get_bottom_bound()));
        this->set_top_bound(rotation_y.Rotate(this->get_top_bound()));
        this->set_center(rotation_y.Rotate(this->get_center()));
        for(int index  = 0; index < 3;index++){
            this->set_offset(rotation_y.Rotate(this->get_offset(index)),index);
        }
    }

    // step3: rotate along z-axis if phi is not equal to 0;
    if(phi!=0){
        Rotation rotation_z;
        float rad_z = phi;
        rotation_z.set_rotation_matrix(rad_z,Axis::z);
        //rotate the block.
        this->set_bottom_bound(rotation_z.Rotate(this->get_bottom_bound()));
        this->set_top_bound(rotation_z.Rotate(this->get_top_bound()));
        this->set_center(rotation_z.Rotate(this->get_center()));
        for(int index  = 0; index < 3;index++){
            this->set_offset(rotation_z.Rotate(this->get_offset(index)),index);
        }
    }

    // step4: shift the block to the given center.
//    this->set_bottom_bound(Point3D(center)+this->get_bottom_bound());
//    this->set_top_bound(Point3D(center)+this->get_top_bound());
//    this->set_center(center);

}


bool ObliqueBlock::LocatePoint(Point3D &pt) const{

    Point3D diff = pt - this->get_bottom_bound();
    int mesh_index[3];
    for(int index = 0 ; index < 3; index++){
        float unit = this->get_interval().GetPointByIndex(index);
        mesh_index[index] = Point3D::ComputeScalarProduct(diff,this->get_offset(index))/unit/unit;
    }
    return this->LocatePoint(GridSize(mesh_index[0],mesh_index[1],mesh_index[2]),pt);
}

bool ObliqueBlock::LocatePoint(const GridSize &mesh_index, Point3D &pt) const{
    //if the
    if(mesh_index.IsInside(this->get_num_grid())){
        Point3D temp = Point3D::GridSize_to_Point3D(mesh_index)+Point3D(0.5,0.5,0.5);
        pt = Point3D(this->get_bottom_bound())
                + Point3D(this->get_offset(0))*temp.get_px()
                + Point3D(this->get_offset(1))*temp.get_py()
                + Point3D(this->get_offset(2))*temp.get_pz();
        return true;
    }
    return false;
}

bool ObliqueBlock::LocatePoint(int mesh_index, Point3D &pt) const{
    int nx = this->get_num_grid().get_num_x();
    int ny = this->get_num_grid().get_num_y();
    int plane = nx*ny;
    int line = nx;
    int iz =(mesh_index/plane);
    int iy =((mesh_index-iz*plane)/line);
    int ix =((mesh_index%plane)%line);
    return this->LocatePoint(GridSize(ix,iy,iz),pt);
}
//to be finished
bool ObliqueBlock::ComputeInsection(const Ray &ray, float &insect_length) const{

}
//to be finished
void ObliqueBlock::DescribeSelf()const {

}

}
