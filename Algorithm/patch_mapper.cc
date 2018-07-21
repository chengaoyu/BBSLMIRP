/*
  @brief:  A concrete class which conbine the pet interface and
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/27
*/

#include "patch_mapper.h"
#include <ctime>


#include "./PETSystem/spherepet.h"
#include "./Geometry/block.h"
#include "./Projector/projector.h"
#include "./Geometry/ray.h"
#include "./Geometry/patch.h"

namespace BBSLMIRP {
PatchMapper::PatchMapper(const PETScanner &pet,  const Projector &projector){
    this->pet = &pet;
    this->projector = &projector;
    //this->map = &map;
}
PatchMapper::~PatchMapper(){

}

void PatchMapper::ComputeMap(Grid3D &map){
    this->Enumerate( map);
}
void PatchMapper::Enumerate( Grid3D& map){
    int num_blocks = pet->get_num_blocks();
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
        const Patch& patch1 = pet->get_patch(iBlock);
        int num_mesh1 = block1.GetTotalMeshes();
        for(int jBlock = iBlock+1; jBlock < num_blocks; jBlock++){
            //get another block
            const Block& block2 = pet->get_block(jBlock);
            const Patch& patch2 = pet->get_patch(jBlock);
            int num_mesh2 = block2.GetTotalMeshes();
            //loop all the LORs
            for(int imesh1 = 0; imesh1 < num_mesh1 ;imesh1++){
                block1.LocatePoint(imesh1,pt1);
                if(!patch1.IsInPatch(pt1))
                    continue;
                for(int imesh2 = 0;imesh2 < num_mesh2; imesh2++){
                    block2.LocatePoint(imesh2,pt2);
                    if(!patch2.IsInPatch(pt2))
                        continue;
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

}
