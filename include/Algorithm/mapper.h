#ifndef MAPPER_H
#define MAPPER_H

/*
  @brief:  An abstrct class which is used to compute the normaliztion map of an Reconstruction.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/
namespace BBSLMIRP {

class Grid3D;

class Mapper{
public:
    virtual void ComputeMap(Grid3D& map) = 0;
    //virtual Grid3D& get_map()= 0;
};
}


#endif // MAPPER_H
