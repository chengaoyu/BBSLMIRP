/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/


#include <ctime>
#include "pet_translator.h"
#include "petscanner.h"
#include "datastruct.h"
#include "filestruct.h"
#include "ray.h"
#include "block.h"

namespace BBSLMIRP {

PETTranslator::PETTranslator(const PETScanner &pet){
    this->pet = &pet;
}
PETTranslator::~PETTranslator(){

}

/*this method locate a point int a block */
void PETTranslator::TranslateEvents(const std::string &input_file_name, const std::string & output_image_name) const{
    FileController fc;
    int num_events = 0;
    fc.ReadHeader(input_file_name,num_events);
    const std::string output_file_name = output_image_name+".trans";
    //std::ifstream inStream(input_file_name,std::ios_base::in);
    //cl
    std::ofstream outStream(output_file_name,std::ios_base::trunc);
    outStream.close();
    //Event evt(Point3D(),Point3D(),0);
    int num_valid_events = 0;

    std::cout << "Event Translation start.  "<<std::endl;
    std::clock_t start, end;
    start = clock();
    double diff;

    //divide the event file into parts and then squencely compute the event patches.
    int event_patch_size = 1e6; // maximum size of the patch.
    int num_of_patches = num_events/event_patch_size + 1;
    std::vector<CoinEvent> eventpatch;
    std::vector<CoinEvent> transeventpatch;

    // loop the patches
    for (int iPatch = 0; iPatch < num_of_patches; iPatch++){
        int pos = iPatch*event_patch_size;
        // compute the events in current patch.
        int num_events_in_patch = 0;
        if(iPatch < num_of_patches-1) num_events_in_patch = event_patch_size;
        else num_events_in_patch = num_events%event_patch_size;

        fc.LoadEvents(input_file_name,pos,num_events_in_patch,eventpatch);
        transeventpatch.clear();
        // loop the events
        for (int iEvent = 0; iEvent < (int)eventpatch.size(); iEvent++)
        {
            //fc.ReadEvent(inEventStream, evt);
            CoinEvent cevt = eventpatch[iEvent];
            //Event evt(Point3D(cevt.pos1_x,cevt.pos1_y,cevt.pos1_z),Point3D(cevt.pos2_x,cevt.pos2_y,cevt.pos2_z),cevt.tof_dis);
            //Ray& ray = evt.get_ray();
            //find the block which contain the event point.
            Point3D pt1(cevt.pos1_x,cevt.pos1_y, cevt.pos1_z); //= ray.get_startpoint();
            Point3D pt2(cevt.pos2_x,cevt.pos2_y, cevt.pos2_z);
            int index1 = pet->LocateBlock(pt1);
            int index2 = pet->LocateBlock(pt2);
            if(index1 < 0||index2 < 0 || index1 >= pet->get_num_blocks() ||index2 >= pet->get_num_blocks()){
                continue;
            }
            const Block& block1 = pet->get_block(index1);
            const Block& block2 = pet->get_block(index2);
            if(block1.LocatePoint(pt1)&&block2.LocatePoint(pt2)){
                num_valid_events++;
                CoinEvent transevt;
                transevt.pos1_x = pt1.get_px();
                transevt.pos1_y = pt1.get_py();
                transevt.pos1_z = pt1.get_pz();
                transevt.pos2_x = pt2.get_px();
                transevt.pos2_y = pt2.get_py();
                transevt.pos2_z = pt2.get_pz();
                transevt.tof_dis = cevt.tof_dis;
                transeventpatch.push_back(transevt);
            }
        }
        fc.SaveEvents(output_file_name,transeventpatch);
        end = clock();
        diff = (double)(end - start) / CLOCKS_PER_SEC;
        if(iPatch < num_of_patches -1){
            std::cout << "finished/total: "<<(iPatch+1)*event_patch_size<<" / "<<num_events<<std::endl;
            std::cout << "cpu time: " <<  diff  << " seconds,";
            std::cout << "time left: " << diff  * (num_events - (iPatch+1)*event_patch_size) /((iPatch+1)*event_patch_size) << "seconds." << std::endl;
       }
    }

    fc.SaveHeader(output_file_name,num_valid_events);

}
}
