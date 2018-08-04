/*
  @brief:  PETApplication define an entry to a PET reconstruction.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/26
*/

#include "petapplication.h"
#include "filestruct.h"
#include "petscanner.h"
#include "pet_factory.h"
#include "ringpet_factory.h"
#include "spherepet_factory.h"

#include "map_task.h"
#include "reconstruction_task.h"

namespace BBSLMIRP {
PETApplication::PETApplication(const std::string &pet_configure_file, const std::string &task_file){

    // build the Factory.
    BuildFactory(pet_configure_file);
    //construct the pet scanner.
    scanner = factory->MakeScanner(pet_configure_file);
    //construct the task list.
    BuildTaskList(task_file);


}
PETApplication::~PETApplication(){
    if (factory!=NULL){
        delete factory;
        factory = NULL;
    }
    if (scanner != NULL){
        delete scanner;
        scanner = NULL;
    }
    // release the task list
    std::vector<Task*>::iterator iter;
    for(iter = task_list.begin();iter != task_list.end();++iter){
        delete *iter;
    }
    task_list.clear();

}

void PETApplication::BuildFactory(const std::string &pet_configure_file){
    std::string pet_category;
    FileController fc;
    fc.ReadPETConfigure(pet_configure_file, pet_category);
    if(pet_category == "ring"){
//        RingPETFactory factory;
        factory = new RingPETFactory();
    }
    else if(pet_category == "sphere"){
        factory = new SpherePETFactory();
    }
    else{

    }
}

void PETApplication::BuildTaskList(const std::string &task_file){
    FileController fc;
    std::ifstream task_stream(task_file,std::ios_base::in);
    std::string key;
    while(!task_stream.eof()){
        task_stream>>key;
        if(key == "Name:"){
            std::string name;
            task_stream>> name;
            if(name == "NormalizationMap"){
                Task* task = new MapTask();
                task->Initialize(task_stream);
                task_list.push_back(task);
            }
            else if (name == "Reconstruction"){
                Task* task = new ReconstructionTask();
                task->Initialize(task_stream);
                task_list.push_back(task);
            }
        }
    }
    task_stream.close();
}

void PETApplication::Run()const{
    for (int index =0 ; index< task_list.size();index++){
        const Task& task = this->get_task(index);
        task.Run(this->get_factory(),this->get_scanner());
    }
}

}


