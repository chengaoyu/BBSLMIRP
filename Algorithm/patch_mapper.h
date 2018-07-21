#ifndef PATCH_MAPPER_H
#define PATCH_MAPPER_H

/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/
#include"mapper.h"
namespace BBSLMIRP {
class PETScanner;
class Projector;
class Grid3D;
class PatchMapper:public Mapper{
public:
    PatchMapper(const PETScanner& pet, const Projector& projector);
    virtual ~PatchMapper();

    virtual void  ComputeMap( Grid3D& map);

    virtual const PETScanner& get_pet(){return *pet;}
    virtual const Projector& get_projector(){return *projector;}
    //virtual Grid3D& get_map(){return *map;}
    // enumerate all the block pairs to compute the Map.
    virtual void Enumerate( Grid3D& map);

private:
    const PETScanner*     pet;
    const Projector*     projector;
    //Grid3D*        map;
};

}


#endif // PATCH_MAPPER_H
