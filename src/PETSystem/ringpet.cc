/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/
#include "ringpet.h"
#include <fstream>
#include "obliqueblock.h"
#include "block.h"

namespace BBSLMIRP{

RingPET::RingPET(const std::string& scanner_file){

    this->Initialize(scanner_file);
}

RingPET::~RingPET(){
    //delete this->block_origin;
}

void RingPET::Initialize(const std::string& scanner_file){

    std::ifstream configure(scanner_file, std::ios_base::in);
    float in_radius, out_radius;
    float height, gap;
    int   nBlocks, nRings;
    int   nx, ny ,nz;
    float gx, gy, gz;
    float fov_ratio = 0.95;
    std::string key;
    while(!configure.eof()){
       configure >> key;
       if(key == "PETScanner:")
           configure>>key;
       else if(key =="InnerRadius:")
           configure>>in_radius;
       else if(key =="OuterRadius:")
           configure>>out_radius;
       else if(key =="Height:")
           configure>>height;
       else if(key =="RingGap:")
           configure>>gap;
       else if(key == "NumberOfBlocksPerRing:")
           configure>>nBlocks;
       else if(key == "NumberOfRings:")
           configure>>nRings;
       else if(key =="Grid:")
           configure>>nx>>ny>>nz;
       else if(key =="BlockSize:")
           configure>>gx>>gy>>gz;
       else if(key =="FovRatio:")
           configure>>fov_ratio;
       else{

       }
    }
    configure.close();

    this->set_inner_radius(in_radius);
    this->set_outer_radius(out_radius);
    this->set_height(height); //not important.
    this->set_ring_gap(gap); //important.
    this->set_num_blocks_per_ring(nBlocks);
    this->set_num_rings(nRings);
    this->set_fov_ratio(fov_ratio);

    ObliqueBlock vb;
    vb.Initialize(GridSize(nx,ny,nz),Point3D(gx,gy,gz),Point3D(0,0,0),0,0);
    this->make_block_list(vb);
}

int RingPET::LocateBlock(const Point3D &pt) const{
    float gap = this->get_ring_gap();
    //all the blocks are of the same size, so get the size from the first block.
    const Point3D bgs =  this->get_block(0).get_geometry_size();
    //the bottom bound of Ring PET.
    float bottom_z = -(gap + bgs.get_pz())*(num_rings)/2;

    // decide which ring (0 ~ num_rings-1) the point belongs to.
    int   iRing = (pt.get_pz()-bottom_z)/(gap + bgs.get_pz());

    // decide which block(0 ~ num_blocks_per_ring - 1) the point belongs to.
    float phi = atan2(pt.get_py(), pt.get_px());
    //add kPi to shift the range of phi to 0~2kPi.
    phi = (phi >= 0) ? phi : phi + 2 * kPi;
    int iBlock = 0;
    float phi_offset = 2*kPi/num_blocks_per_ring;

    //compute the block index in the block_list.
    if(phi>phi_offset/2 && phi < (2*kPi-phi_offset/2))
        iBlock = phi/phi_offset + 0.5;
    int block_index = iRing*num_blocks_per_ring+iBlock;
    //if index is out of range, return -1;
    return block_index < this->get_num_blocks()? block_index:-1;
}

void RingPET::DescribeSelf() const{

}
//add the blocks from bottom to top.
void RingPET::make_block_list(ObliqueBlock &origin){
    int nRings = this->get_num_rings();
    int nBlocks = this->get_num_blocks_per_rings();
    //the z_axis  of bottom ring
    float bottom_z = -(origin.get_geometry_size().get_pz()+this->get_ring_gap())*(nRings-1)/2;
    //x_offset of block prototype
    float block_x_offset = (this->get_inner_radius()+this->get_outer_radius())/2;

    //generate the blocks from the bottom ring to the top ring
    for(int iRing = 0; iRing < nRings; iRing++){
        float block_z_offset = bottom_z + iRing*(origin.get_geometry_size().get_pz()+this->get_ring_gap());
        Point3D center(block_x_offset,0,block_z_offset);
        //generate the blocks in a ring. the first block lay on the x_axis;
        for(int iBlock = 0; iBlock < nBlocks; iBlock++){
            float phi = 360.0/nBlocks*iBlock;
            //compute the center position.
//            Rotation rotation_z;
            float rad_z = phi/180*kPi;
//            rotation_z.set_rotation_matrix(rad_z,Axis::z);
//            Point3D rotated_center = rotation_z.Rotate(center);

            //copy the prorotype block.
            ObliqueBlock vb = origin;
            vb.Initialize(origin.get_num_grid(),origin.get_geometry_size(),center,rad_z,0);
            this->block_list.push_back(vb);
        }
    }
}
}
