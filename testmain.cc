/*
  @brief:  main() for debugging.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include<iostream>
#include "yaml-cpp/yaml.h"

#include "PETSystem/petapplication.h"

using namespace BBSLMIRP;

int main(int argc, char** argv){
    //check the input arguments
    if(argc!=3){
        std::cerr<<"Invalid arguments: "<< argc <<" !"<<std::endl;
        std::cerr<<"there should be two arguments: 1) the ring configure txt file;"<<std::endl;
        std::cerr<<"                               2) the task txt file."<<std::endl;
        std::exit(-1);
    }

    // the pet scanner configure file
    std::string pet_configure_file = argv[1];

    // the todo task file
    std::string task_file = argv[2];

    // create an application for the task and initialize it with two files.
    PETApplication app(pet_configure_file, task_file);

    // execute the task.
    app.Run();
    return 1;
}
