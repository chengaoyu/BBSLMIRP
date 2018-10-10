/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/

#include"filestruct.h"
#include"datastruct.h"
#include<iostream>
#include<iomanip>
#include"grid.h"
#include"ray.h"
#include"patch.h"

namespace BBSLMIRP {



FileController::FileController(){

}

FileController::~FileController(){

}

bool FileController::ReadImage(const std::string &image_name, Grid3D &image) const{
    std::ifstream infile(image_name, std::ios_base::in);
    if(! infile.good()){
        std::cerr<<"ReadImg(): no file "<<image_name <<"!"<<std::endl;
        std::exit(-1);
    }
    size_t nX = image.get_block().get_num_grid().get_num_x();
    size_t nY = image.get_block().get_num_grid().get_num_y();
    size_t nZ = image.get_block().get_num_grid().get_num_z();
    for (size_t k=0; k<nZ;k++){
        for(size_t j=0; j<nY; j++){
            for(size_t i=0; i<nX; i++){
                //define a mesh index.
                MeshIndex mi(i,j,k);
                float value;
                //read the value of the image voxels.
                infile>>value;
                image.set_mesh(mi,value);
            }
        }
    }
    return true;
}

bool FileController::ReadImageBin(const std::string &image_name, Grid3D &image) const{
    std::ifstream infile(image_name, std::ios_base::in);
    if(! infile.good()){
        std::cerr<<"ReadImg(): no file "<<image_name <<"!"<<std::endl;
        std::exit(-1);
    }
    size_t nX = image.get_block().get_num_grid().get_num_x();
    size_t nY = image.get_block().get_num_grid().get_num_y();
    size_t nZ = image.get_block().get_num_grid().get_num_z();
    for (size_t k=0; k<nZ;k++){
        for(size_t j=0; j<nY; j++){
            for(size_t i=0; i<nX; i++){
                //define a mesh index.
                MeshIndex mi(i,j,k);
                float value;
                //read the value of the image voxels.
                infile.read((char*)&value,sizeof(float));
                image.set_mesh(mi,value);
            }
        }
    }
    return true;
}

bool FileController::SaveImage(const std::string &image_name, const Grid3D &image) const{
    std::ofstream outfile(image_name, std::ios_base::out);
    if(! outfile.good()){
        std::cerr<<"SaveImg(): Error while open file!"<<std::endl;
        return false;
    }
    size_t nX = image.get_block().get_num_grid().get_num_x();
    size_t nY = image.get_block().get_num_grid().get_num_y();
    size_t nZ = image.get_block().get_num_grid().get_num_z();
    for (size_t k=0; k<nZ;k++){
        for(size_t j=0; j<nY; j++){
            for(size_t i=0; i<nX; i++){
                MeshIndex mi(i,j,k);
                outfile<< image.get_mesh(mi)<<std::endl;
            }
        }
    }
    return true;
}
bool FileController::SaveImageBin(const std::string image_name, const Grid3D &image) const{
    std::ofstream outfile(image_name, std::ios_base::out|std::ios_base::binary);
    if(! outfile.good()){
        std::cerr<<"SaveImg(): Error while open file!"<<std::endl;
        return false;
    }
    size_t nX = image.get_block().get_num_grid().get_num_x();
    size_t nY = image.get_block().get_num_grid().get_num_y();
    size_t nZ = image.get_block().get_num_grid().get_num_z();
    for (size_t k=0; k<nZ;k++){
        for(size_t j=0; j<nY; j++){
            for(size_t i=0; i<nX; i++){
                MeshIndex mi(i,j,k);
                float value = image.get_mesh(mi);
                outfile.write((char*)&value, sizeof(float));
            }
        }
    }
    return true;
}

bool FileController::ReadEvent(std::ifstream &inStream, Event &evt) const{
//    if(!inStream.good()){
//        std::cerr<<"ReadEvt(): Error while open file!"<<std::endl;
//        return false;
//    }

    Point3D pt1 = ReadPoint(inStream);
    Point3D pt2 = ReadPoint(inStream);
    evt.get_ray().SetRay(pt1,pt2);

    float tof_distance;
    inStream >> tof_distance;
    evt.set_tof_distance(tof_distance);
    return true;
}
bool FileController::SaveEvent(std::ofstream &outStream, Event &evt) const{
//    if(!outStream.good()){
//        std::cerr<<"SaveEvent(): Error while open file!"<<std::endl;
//        return false;
//    }
    SavePoint(outStream,evt.get_ray().get_startpoint());
    SavePoint(outStream,evt.get_ray().get_endpoint());
    outStream << std::setw(20) << evt.get_tof_distance()<<std::endl;
    return true;
}

bool FileController::LoadEvents(const std::string &file_name, int pos, int list_size, CoinEventList &evtlist){
    std::ifstream infile(file_name,std::ios_base::in|std::ios_base::binary);
    if(!infile.good()){
        std::cerr<<"LoadEvents(): error when open file "<<file_name<<"!"<<std::endl;
        std::exit(-1);
    }
    // move to the position to be loaded.
    infile.seekg(7*pos*sizeof(float),std::ios_base::beg);
    int data_size = list_size*7;
    float* buffer = new float[data_size];
    infile.read((char*) buffer,sizeof(float)*data_size);
    evtlist.clear();
    for (int i = 0; i < list_size; i++){
        CoinEvent evt;
        int epos = i*7;
        evt.pos1_x = buffer[epos];
        evt.pos1_y = buffer[epos+1];
        evt.pos1_z = buffer[epos+2];
        evt.pos2_x = buffer[epos+3];
        evt.pos2_y = buffer[epos+4];
        evt.pos2_z = buffer[epos+5];
        evt.tof_dis = buffer[epos+6];
        evtlist.push_back(evt);
    }
    delete [] buffer;
    return true;
}
bool FileController::SaveEvents(const std::string &file_name, CoinEventList &evtlist){
    std::ofstream outfile(file_name,std::ios_base::out|std::ios_base::binary|std::ios_base::app);
    if(!outfile.good()){
        std::cerr<<"SaveEvents(): error when open file "<<file_name<<"!"<<std::endl;
        std::exit(-1);
    }
    // move to the position to be loaded.
    //outfile.seekp(pos,std::ios_base::beg);
    int list_size = evtlist.size();
    int data_size = list_size*7;
    float* buffer = new float[data_size];
    for (int i = 0; i < list_size; i++){
        CoinEvent evt = evtlist[i];
        int epos = i*7;
        buffer[epos]   = evt.pos1_x;
        buffer[epos+1] = evt.pos1_y;
        buffer[epos+2] = evt.pos1_z;
        buffer[epos+3] = evt.pos2_x;
        buffer[epos+4] = evt.pos2_y;
        buffer[epos+5] = evt.pos2_z;
        buffer[epos+6] = evt.tof_dis;
    }
    outfile.write((char*) buffer,data_size*sizeof(float));
    delete [] buffer;
    return true;
}

bool FileController::ReadListModeEvent(std::ifstream &inStream, LMEvent &lmevent) const{

}
bool FileController::SaveListModeEvent(std::ofstream &outStream, LMEvent &lmevent) const{

}
bool FileController::ReadHeader(const std::string& file_name, int &num_events) const{
    std::ifstream header_file(file_name+".hdr",std::ios_base::in);
    if(!header_file.good()){
        std::cerr<<"ReadHeader(): Error while open file!"<<std::endl;
        std::exit(-1);
    }
    header_file>>num_events;
    return true;
}
bool FileController::SaveHeader(const std::string& file_name, int &num_events) const{
    std::ofstream header_file(file_name+".hdr",std::ios_base::out);
    if(!header_file.good()){
        std::cerr<<"SaveHeader(): Error while open file!"<<std::endl;
        return false;
    }
    header_file<<num_events<<std::endl;
    return true;
}

bool FileController::ReadPatch(std::ifstream & inStream, Patch& patch )const{
    if(!inStream.good()){
        std::cerr<<"ReadPatch(): Error while open file!"<<std::endl;
        std::exit(-1);
    }
    int num_vertice;
    inStream>> num_vertice;
    Face in_face;
    Face out_face;
    for (int iVertex = 0; iVertex < num_vertice; iVertex++){
        in_face.push_back(ReadPoint(inStream));
    }
    for (int iVertex = 0; iVertex < num_vertice; iVertex++){
        out_face.push_back(ReadPoint(inStream));
    }
    return patch.Initialize(in_face, out_face);
}

Point3D FileController::ReadPoint(std::ifstream & inStream)const{
    float px,py,pz;
    inStream >> px >> py >> pz;
    return Point3D(px,py,pz);
}
void  FileController::SavePoint(std::ofstream &outStream,const Point3D &pt) const{
    outStream << std::setw(20) << pt.get_px();
    outStream << std::setw(20) << pt.get_py();
    outStream << std::setw(20) << pt.get_pz();
}

void FileController::ReadPETConfigure(const std::string &file_name, std::string &pet_category) const{
    std::ifstream pet_configure(file_name,std::ios_base::in);
    if(!pet_configure.good()){
        std::cerr<<"ReadPETConfigure(): Error while open file!"<<std::endl;
        std::exit(-1);
    }
    else{
        std::string pet_name;
        pet_configure>>pet_name;
        if(pet_name == "PETScanner:"){
            pet_configure>>pet_category;
        }
        else{
            std::cerr<<"ReadPETConfigure(): configure file is bad!"<<std::endl;
            std::exit(-1);
        }
    }
    pet_configure.close();
}

void FileController::ReadTaskFile(const std::string &file_name) const{

}
}
