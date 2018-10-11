/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11/01
*/
#include "mlem.h"
#include <ctime>

#include "grid.h"
#include "filestruct.h"
#include "projector.h"
#include "datastruct.h"
#include "filter.h"
#include "./Geometry/obliqueblock.h"
namespace BBSLMIRP {

MLEM::MLEM(){

}
MLEM::~MLEM(){

}

void MLEM::Initialize(const PETScanner& pet, const Projector& projector, Grid3D& map, int iteration_start_index, int num_iteration,
                 std::string start_image_name, std::string in_file_name, std::string out_image_name, int output_interval, bool abf_flag){
    this->pet = &pet;
    this->projector = &projector;
    this->map = &map;
    this->iteration_start_index = iteration_start_index;
    this->start_image_name = start_image_name;
    this->num_iteration = num_iteration;
    this->in_file_name = in_file_name;
    this->out_image_name = out_image_name;
    this->output_interval = output_interval;
    this->abf_flag = abf_flag;
}
void MLEM::Reconstruct(){
    Grid3D image_next, image_now;
    image_next.Initialize(this->map->get_block());
    image_now.Initialize(this->map->get_block());
    Grid3D& map = this->get_map();
    FileController fc;
    int num_events = 0;
    fc.ReadHeader(this->in_file_name,num_events);
    image_now.SetAllMeshes((float)num_events/map.Sum());
    //image_now.SetAllMeshes(((float)num_events)/(*map).Sum());


    if(iteration_start_index>1 && start_image_name != ""){
        fc.ReadImage(this->start_image_name,image_now);
    }
    image_next = image_now;
    if(this->abf_flag){
        this->ABF(map);
    }

    //timestamp
    std::clock_t start, end;
    start = clock();
    double diff;
    std::cout << "MLEM reconstruction start..." << std::endl;
    for (int iter = iteration_start_index; iter < num_iteration + iteration_start_index; iter++)
    {
        image_now = image_next;
        //'image_next' will aaccumulate the backprojection of the reciprocal line integrals of 'this->img'.
        image_next.SetAllMeshes(0.0);
        this->EM(image_now,image_next, 0.0, this->in_file_name);

//        float gavg = image_now.Sum()/ image_now.get_block().get_num_grid().GetTotalNumOfMeshes();
//        Grid3D temp1 = image_now * image_now;
//        float gsig = std::sqrt(temp1.Sum() / temp1.get_block().get_num_grid().GetTotalNumOfMeshes() - gavg * gavg);
        //image_now.(gmin, gmax);
        //
        if (abf_flag)
        { //insert the ABF (after backprojection filtering) here!
            this->ABF(image_next);
        }

        image_next = (image_next * image_now);
        image_next = image_next / map;
        image_now =  image_next - image_now;
        //image_next.Absolute();

        end = clock();
        diff = (double)(end - start) / CLOCKS_PER_SEC;
        std::cout << "iteration " << iter << " / " <<num_iteration + iteration_start_index - 1 << std::endl;
        std::cout << "cpu time: " << diff << " s,";
        std::cout << "the time remains:" << diff / (iter - iteration_start_index + 1) * (num_iteration + iteration_start_index - iter- 1) << " s." << std::endl;

        if ((iter ) % this->output_interval == 0)
        {
            fc.SaveImageBin(out_image_name +"_"+ std::to_string(iter) + ".img", image_next);
        }
    }

}

void MLEM::EM(Grid3D& image_now, Grid3D& image_next, float comfac, const std::string& event_file_name) const{
    int num_events = 0;
    FileController fc;
    fc.ReadHeader(event_file_name, num_events);
    //std::ifstream inEventStream(event_file_name, std::ios_base::in);
    //this->setLMEFlag(true);
    const Block block = image_now.get_block();
    //Event evt(Point3D(0,0,0),Point3D(1,1,1),0);

    //divide the event file into parts and then squencely compute the event patches.
    int event_patch_size = 1e6; // maximum size of the patch.
    int num_of_patches = num_events/event_patch_size + 1;
    std::vector<CoinEvent> eventpatch;

    // loop the patches
    for (int iPatch = 0; iPatch < num_of_patches; iPatch++){
        int pos = iPatch*event_patch_size;
        // compute the events in current patch.
        int num_events_in_patch = 0;
        if(iPatch < num_of_patches-1) num_events_in_patch = event_patch_size;
        else num_events_in_patch = num_events%event_patch_size;
        fc.LoadEvents(event_file_name,pos,num_events_in_patch,eventpatch);
        // loop the events
        for (int iEvent = 0; iEvent < num_events_in_patch; iEvent++)
        {
            //fc.ReadEvent(inEventStream, evt);
            CoinEvent cevt = eventpatch[iEvent];
            Event evt(Point3D(cevt.pos1_x,cevt.pos1_y,cevt.pos1_z),Point3D(cevt.pos2_x,cevt.pos2_y,cevt.pos2_z),cevt.tof_dis);
            LMEvent lmevt;
            lmevt.num_events = 1;
            lmevt.num_voxels = 0;
            projector->MakeListModeEvent(evt,block,lmevt);
            if (lmevt.array.size() > 0)
            {
                std::log(comfac * this->EM_Projection(image_now, image_next, lmevt));
            }
        }
    }




}

float MLEM::EM_Projection(Grid3D &ImgNow, Grid3D &ImgNext, LMEvent &lmevt) const
{
    float val = 0;
    float lineItgrl = 0;
    for (int i = 0; i < lmevt.array.size(); i++)
    {
        LMElement lmele = lmevt.array[i]; ;
        //forward projection
        lineItgrl += ImgNow.get_mesh(lmele.mesh_index) * lmele.length;
    }
    if (lineItgrl > 0)
    {
        //back projection
        val = (float)lmevt.num_events / lineItgrl;
        for (int i = 0; i < lmevt.array.size(); i++)
        {
            LMElement lmele = lmevt.array[i];
            ImgNext.set_mesh(lmele.mesh_index, val * lmele.length + ImgNext.get_mesh(lmele.mesh_index));
        }
    }
    return lineItgrl;
}

void MLEM::ABF(Grid3D &image) const{
    Grid3D abf;
    Filter ft;
    ObliqueBlock bok;
    GridSize gs1(4, 4, 4);
    bok.set_num_grid(gs1);
    abf = ft.Kaiser(bok, 10.4, 2, 2);
    image.Conv3(abf);
}

}
