/*
  @brief:  A concrete class which conbine the pet interface and
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/27
*/

#include "ring_mapper.h"
#include <ctime>


#include "ringpet.h"
#include "block.h"
#include "projector.h"
#include "ray.h"
#include "grid.h"

namespace BBSLMIRP {
RingMapper::RingMapper(const PETScanner &pet, const  Projector &projector){
    this->pet = &pet;
    this->projector = &projector;
    //this->map = &map;
}
RingMapper::~RingMapper(){

}

void RingMapper::ComputeMap(Grid3D& map){
    this->Enumerate( map);
    this->CutMap(map);
}
void RingMapper::Enumerate(Grid3D& map){
    int num_blocks = pet->get_num_blocks();
    // debug
    // std::cout<<"num_blocks:" <<num_blocks<<std::endl;
    double total_block_pairs = (num_blocks-1)*(num_blocks)/2;
    Point3D pt1,pt2;
    Ray ray;

    // enumeration method to calculate the normalization map.
    std::cout << "the time is rough estimated, for reference only "<<std::endl;

    std::clock_t start, end;
    start = clock();
    double diff;

    for(int iBlock = 0; iBlock < num_blocks; iBlock++){
        // get the first block
        const Block& block1 = pet->get_block(iBlock);
        int num_mesh1 = block1.GetTotalMeshes();
        // debug
        // std::cout<<"loop start"<<std::endl;
        for(int jBlock = iBlock+1; jBlock < num_blocks; jBlock++){
            //get another block
            const Block& block2 = pet->get_block(jBlock);
            int num_mesh2 = block2.GetTotalMeshes();
            //loop all the LORs
            for(int imesh1 = 0; imesh1 < num_mesh1 ;imesh1++){
                block1.LocatePoint(imesh1,pt1);
                for(int imesh2 = 0;imesh2 < num_mesh2; imesh2++){
                    block2.LocatePoint(imesh2,pt2);
                    ray.SetRay(pt1,pt2);
                    projector->MappingLine(ray,map);
                }
            }
        }
        //timestamp
        double left_pairs = (num_blocks-1-iBlock)*(num_blocks-2-iBlock)/2;
        end = clock();
        diff = (double)(end - start) / CLOCKS_PER_SEC;
        std::cout << "finished/total: "<<(total_block_pairs - left_pairs)<<" / "<<total_block_pairs<<" ("<<(total_block_pairs - left_pairs)/total_block_pairs*100<<"% )"<<std::endl;
        std::cout << "cpu time: " << diff / 60 << " minutes,";
        std::cout << "time left: " << diff / 60 * left_pairs /(total_block_pairs - left_pairs) << "miuntes." << std::endl;
    }
}

void RingMapper::CutMap(Grid3D& map){
    // ToDo: add the code to change the voxel values to very large when it is out of FOV.
    float innerR = pet->get_inner_radius();
    int num_meshes = map.get_block().GetTotalMeshes();

    Point3D center = map.get_block().get_center();
    for(int iMesh = 0; iMesh < num_meshes; iMesh++){
        Point3D mpt;
        mpt.set_pz(center.get_pz());
        map.get_block().LocatePoint(iMesh,mpt);
        float dis = (mpt-center).GetNorm2();
        if (dis > pet->get_fov_ratio()*innerR){
            map.set_mesh(iMesh, 0.0);
        }
    }

}

}
