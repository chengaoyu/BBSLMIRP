#ifndef MAP_TASK_H
#define MAP_TASK_H
/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/
#include "task.h"

namespace BBSLMIRP {

class Projector;
class GridSize;
class Point3D;
class MapTask: public Task{
public:
    MapTask();
    virtual ~MapTask();
    virtual void Initialize(std::ifstream &task_stream);
    virtual void Run(const PETFactory&factory, const PETScanner&pet) const;
    virtual const GridSize& get_grid()const {return *grid;}
    virtual const Point3D& get_map_size()const{return *map_size;}
    virtual const std::string& get_map_name()const{return map_name;}
    
private:
    std::string projector_type;
    GridSize* grid;
    Point3D*  map_size;
    std::string map_name;
};
}
#endif // MAP_TASK_H
