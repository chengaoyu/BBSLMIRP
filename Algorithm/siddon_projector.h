#ifndef SIDDON_PROJECTOR_H
#define SIDDON_PROJECTOR_H
/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/

#include "projector.h"
namespace BBSLMIRP{
class Ray;
class RayCast;
class LMEvent;
class TOF;
class SiddonProjector:public Projector{
public:
    SiddonProjector();
    virtual ~SiddonProjector();
    //compute the list-mode event.
    virtual void MakeListModeEvent (Event& evt, const Block &box, LMEvent& lmevt) const;
    virtual void LMF(float t_TOF, float weight, RayCast& raycast, LMEvent &lmevt)const;

    //compute normalization map.
    virtual void MappingLine(Ray& ray, Grid3D &image)const;
    virtual void Map(float weight, RayCast& raycast, Grid3D& image) const;

    //judge if the ray pass the image.
    virtual bool IsThroughImage(const Block& box, Ray& ray)const;
    virtual bool ClipRay(float toplim, float botlim, int index, Ray &ray) const;

    //set up the ray cast.
    virtual void CastRay( const Block &box, Ray& ray, RayCast& RayCast)const;

    virtual const TOF& get_tof(){return *tof_information;}
    virtual void set_tof(const TOF& tof){tof_information = &tof;}

private:
    const TOF * tof_information;
};

}
#endif // SIDDON_PROJECTOR_H
