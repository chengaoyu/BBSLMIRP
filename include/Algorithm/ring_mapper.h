#ifndef RING_MAPPER_H
#define RING_MAPPER_H

/*
  @brief:  a concrete class of normalizetion map which can compute the
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/27
*/

#include "mapper.h"

namespace BBSLMIRP {
class PETScanner;
class Projector;
class Grid3D;
class RingMapper:public Mapper{
public:
    RingMapper(const PETScanner& pet, const Projector& projector);
    virtual ~RingMapper();

    virtual void ComputeMap(Grid3D& map);
    virtual void CutMap(Grid3D& map);
    virtual const PETScanner& get_pet(){return *pet;}
    virtual const Projector& get_projector(){return *projector;}
    //virtual Grid3D& get_map(){return *map;}
    // enumerate all the block pairs to compute the Map.
    virtual void Enumerate(Grid3D &map);


private:
    const PETScanner*     pet;
    const Projector*   projector;
    //Grid3D*      map;
};
}

#endif // PET_MAPPER_H
