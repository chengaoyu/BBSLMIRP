/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/
#include "reconstruction_task.h"
#include "gridsize.h"
#include "point.h"
#include "block.h"
#include "grid.h"

#include "ringpet_factory.h"
#include "spherepet_factory.h"
#include "siddon_projector.h"
#include "filestruct.h"
#include "datastruct.h"
#include "pet_translator.h"
#include "mlem.h"


namespace BBSLMIRP {

ReconstructionTask::ReconstructionTask(){
    tof =NULL;
    grid = NULL;
    image_size = NULL;
}
ReconstructionTask::~ReconstructionTask(){
    if(grid!=NULL){
        delete grid;
        grid =NULL;
    }
    if(image_size != NULL){
        delete image_size;
        image_size = NULL;
    }
    if(tof != NULL){
        delete tof;
        tof = NULL;
    }
}
void ReconstructionTask::Initialize(std::ifstream &task_stream){
    std::string key;
    while(key != "End"){
        task_stream>>key;
        if(key == "Method:")
            task_stream >> this->method;
        if(key == "ProjectorType:")
            task_stream >> this->projector_type;
        else if(key == "Grid:"){
            int nx,ny,nz;
            task_stream >>nx>>ny>>nz;
            grid = new GridSize(nx,ny,nz);
        }
        else if(key== "Size:"){
            float gx,gy,gz;
            task_stream>>gx>>gy>>gz;
            image_size = new Point3D(gx,gy,gz);
        }
        else if(key == "MapName:")
            task_stream>>map_name;
        else if(key == "IterationStartIndex:")
            task_stream>>iteration_start_index;
        else if (key == "NumberOfIteration:")
            task_stream>>num_iteration;
        else if (key == "StartImage:")
            task_stream>>start_image;
        else if (key == "InputEventFile:")
            task_stream>>input_file;
        else if(key == "OutputImage:")
            task_stream>>output_image_name;
        else if(key == "OutputInterval:")
            task_stream>>output_image_interval;
        else if (key == "TOF_Flag:"){
            int flag;
            task_stream>>flag;
            if(flag == 1){
                tof = new TOF();
                task_stream>>key>>tof->FWHM; // the full half
                task_stream>>key>>tof->binsize;
                float limit;
                task_stream>>key>>limit;
                tof->limit2 = limit*limit;
                tof->Sigma2 = tof->FWHM*tof->FWHM/(8.0*M_LN2);
            }

        }
        else if (key == "ABF_Flag:"){
            task_stream>>abf_flag;
        }
        else{

        }
    }
}
void ReconstructionTask::Run(const PETFactory&factory, const PETScanner& pet) const{

    //Event translation
    Translator* pet_translator = factory.MakeTranslator(pet);
    pet_translator->TranslateEvents(this->input_file,this->output_image_name);

    ObliqueBlock bk;
    bk.Initialize(this->get_grid(),this->get_image_size(),Point3D(0,0,0),0,0);

    //read the normalization map
    Grid3D map;
    map.Initialize(bk);
    FileController fc;
    fc.ReadImageBin(this->map_name,map);

    Projector *projector = NULL;
    if (this->projector_type == "siddon"){
            projector = new SiddonProjector();
            projector->set_tof(this->get_tof());
    }

    MLEM mlem;
    mlem.Initialize(pet,*projector,map,this->iteration_start_index,
                    this->num_iteration,this->start_image,this->output_image_name+".trans",
                    this->output_image_name,this->output_image_interval,this->abf_flag);
    mlem.Reconstruct();
}


}
