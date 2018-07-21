/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/
#include "map_task.h"
#include "./Geometry/gridsize.h"
#include "./Geometry/point.h"
#include "ringpet_factory.h"
#include "spherepet_factory.h"
#include "./Algorithm/siddon_projector.h"
#include "./Geometry/block.h"
#include "./Geometry/grid.h"
#include "./FileIO/filestruct.h"
#include "./Algorithm/ring_mapper.h"
#include "./Algorithm/patch_mapper.h"

namespace BBSLMIRP {

MapTask::MapTask(){

}

MapTask::~MapTask(){
    if(grid!=NULL){
        delete grid;
        grid =NULL;
    }
    if(map_size != NULL){
        delete map_size;
        map_size = NULL;
    }

}

void MapTask::Initialize( std::ifstream& task_stream) {
    std::string key;
    while(key != "End"){
        task_stream>>key;
        if(key == "ProjectorType:")
            task_stream >> this->projector_type;
        else if(key == "MapGrid:"){
            int nx,ny,nz;
            task_stream >>nx>>ny>>nz;
            grid = new GridSize(nx,ny,nz);
        }
        else if(key== "MapSize:"){
            float gx,gy,gz;
            task_stream>>gx>>gy>>gz;
            map_size = new Point3D(gx,gy,gz);
        }
        else if(key == "MapName:")
            task_stream>>map_name;
        else{

        }
    }
}

void MapTask::Run(const PETFactory& factory, const PETScanner& pet)const {
    Projector *projector = NULL;
    if (this->projector_type == "siddon"){
            projector = new SiddonProjector();
    }
    Mapper* mapper = factory.MakeMapper(pet,*projector);

    Block bk;
    bk.Initialize(this->get_grid(),this->get_map_size(),Point3D(0,0,0));
    Grid3D map;
    map.Initialize(bk);
    FileController fc;
    //compute the normaliztion map
    mapper->ComputeMap(map);
    fc.SaveImage(this->get_map_name(),map);
}
}
