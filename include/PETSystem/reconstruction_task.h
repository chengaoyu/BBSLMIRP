#ifndef RECONSTRUCTION_TASK_H
#define RECONSTRUCTION_TASK_H
/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/

#include"task.h"

namespace BBSLMIRP {

class GridSize;
class Point3D;
struct TOF;
class ReconstructionTask: public Task{
public:
    ReconstructionTask();
    virtual ~ReconstructionTask();
    virtual void Initialize( std::ifstream& task_stream);
    virtual void Run(const PETFactory& factory, const PETScanner& pet) const;

    virtual const GridSize& get_grid()const {return *grid;}
    virtual const Point3D& get_image_size()const{return *image_size;}
    virtual const TOF& get_tof()const {return *tof;}
private:
    std::string method;
    std::string map_name;
    std::string projector_type;
    GridSize *grid;
    Point3D  *image_size;
    int iteration_start_index;
    std::string start_image;
    int num_iteration;
    std::string input_file;
    std::string output_image_name;
    int output_image_interval; //how many iterations per image.
    TOF* tof;
    bool abf_flag;
};
}

#endif // RECONSTRUCTION_TASK_H
