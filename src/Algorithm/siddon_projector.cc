/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/

#include "siddon_projector.h"
#include "datastruct.h"
#include "ray.h"
#include "obliqueblock.h"
#include "grid.h"
#include "raycast.h"
namespace BBSLMIRP {

SiddonProjector::SiddonProjector(){
    tof_information = NULL;
}
SiddonProjector::~SiddonProjector(){

}
void SiddonProjector::MakeListModeEvent(Event &evt, const Block& box,LMEvent& lmevt) const{
    Ray& ray = evt.get_ray();
    float LOR_length = ray.get_length();
    if(this->IsThroughImage(box,ray)){

        RayCast raycast;
        this->CastRay(box,ray,raycast);
        //the weight of this LOR line.
        //float wt = std::exp(-natal) / std::pow(LORLength, 2);
        float weight = 1/(LOR_length*LOR_length);
        LMF(evt.get_length_mint_to_center(), weight, raycast, lmevt);
    }
    else{
        //do nothing!
    }
}

void SiddonProjector::LMF(float t_TOF, float weight, RayCast &raycast, LMEvent &lmevt) const{
    RayCast &rc = raycast; /* local copy of raycast */
    float last_t = 0.0; /* initialize last distance */
    float l_len = 0.0;
    int stillin = rc.get_inBuf(0) * rc.get_inBuf(1) * rc.get_inBuf(2); /* make a non-zero test value unless we are already out */
    int i = -1;
    while (stillin)
    {
        /* look for next intersection: smallest component of nextT */
        i = (rc.get_nextT(0) <= rc.get_nextT(1)) ? ((rc.get_nextT(0) <= rc.get_nextT(2)) ? 0 : 2) : ((rc.get_nextT(1) <= rc.get_nextT(2)) ? 1 : 2);
        l_len = rc.get_nextT(i) - last_t;
        int mi = rc.get_boffs();
        LMElement lmele;
        lmele.mesh_index = mi;
        if (tof_information){// use TOF information.
            float bs = tof_information->binsize; // get the binsize of TOF.
            float t = t_TOF - 0.5 * (rc.get_nextT(i) + last_t);
            float sigma2 = tof_information->Sigma2 + (l_len * l_len + bs * bs) / 12.0;
            float t2_by_sigma2 = t * t / sigma2;
            if (tof_information->limit2 <= 0 || t2_by_sigma2 < tof_information->limit2)
            { // the voxel is valid in the TOF range.
                lmele.length = weight * l_len * tof_information->binsize * exp(-0.5 * t2_by_sigma2) / sqrt(2.0 * M_PI * sigma2);
                lmevt.array.push_back(lmele);
            }
        }
        else //no TOF information.
        {
            lmele.length = weight * l_len;
            lmevt.array.push_back(lmele);
        }
        /* update */

        last_t = rc.get_nextT(i);                         /* set last distance for next pass */
        rc.set_nextT(rc.get_deltaT(i)+rc.get_nextT(i),i); /* set next intersection distance */
        rc.set_boffs(rc.get_boffs()+rc.get_deltaBuf(i));  /* buffer offset for next voxel */
        stillin = rc.get_inBuf(i)-1;
        rc.set_inBuf(stillin,i);                /* if we go out this goes to zero */
    }
}

void SiddonProjector::MappingLine(Ray &ray, Grid3D& map) const{
    const Block& box = map.get_block();
    //Ray& ray = Ray.get_ray();
    float LOR_length = ray.get_length();
    // debug
    // std::cout<<"mapping lines"<<std::endl;
    if(this->IsThroughImage(box,ray)){
        RayCast raycast;
        this->CastRay(box,ray,raycast);
        //the weight of this LOR line.
        //float wt = std::exp(-natal) / std::pow(LORLength, 2);
        float weight = 1/(LOR_length*LOR_length);
        Map(weight, raycast, map);
    }
}

void SiddonProjector::Map(float weight, RayCast &raycast, Grid3D &image) const{
    RayCast &rc = raycast; /* local copy of raycast */
    float last_t = 0.0; /* initialize last distance */
    float l_len = 0.0;
    int stillin = rc.get_inBuf(0) * rc.get_inBuf(1) * rc.get_inBuf(2); /* make a non-zero test value unless we are already out */
    int i = -1;
    while (stillin)
    {
        /* look for next intersection: smallest component of nextT */
        i = (rc.get_nextT(0) <= rc.get_nextT(1)) ? ((rc.get_nextT(0) <= rc.get_nextT(2)) ? 0 : 2) : ((rc.get_nextT(1) <= rc.get_nextT(2)) ? 1 : 2);
        l_len = rc.get_nextT(i) - last_t;
        /* increment voxel */
        int mi = rc.get_boffs();
        float oldvalue = image.get_mesh(mi);
        image.set_mesh(mi,oldvalue + weight * l_len);
        /* update */

        last_t = rc.get_nextT(i);                         /* set last distance for next pass */
        rc.set_nextT(rc.get_deltaT(i)+rc.get_nextT(i),i); /* set next intersection distance */
        rc.set_boffs(rc.get_boffs()+rc.get_deltaBuf(i));  /* buffer offset for next voxel */
        stillin = rc.get_inBuf(i)-1;
        rc.set_inBuf(stillin,i);                /* if we go out this goes to zero */
    }
}

bool SiddonProjector::IsThroughImage(const Block &box, Ray &ray)const{
    ray.SetTs(0,ray.get_length());
    //get the bound od the box.

    /* Keep updating the new min and max t values for the line
     * clipping against each component one at a time */
    for (int i = 0; i < 3; i++)
    {
        if (!ClipRay(box.get_top_bound().GetPointByIndex(i),box.get_bottom_bound().GetPointByIndex(i) , i, ray))
        {
            return false;
        }
    }

    /* Check that all this clipping has not removed the interval,
        i.e. that the line intersects the bounding box. */
    return ray.get_min_t() < ray.get_max_t();
}

bool SiddonProjector::ClipRay(float toplim, float botlim, int index, Ray &ray) const
{
    float p0, delt_p, t, tmax, tmin;
    p0 = ray.get_startpoint().GetPointByIndex(index);
    delt_p = ray.get_direction().GetPointByIndex(index);

    tmax = ray.get_max_t();
    tmin = ray.get_min_t();

    if (delt_p > 0.0)
    {
        t = (toplim - p0) / delt_p;
        if (t < tmax)
            tmax = t;
        t = (botlim - p0) / delt_p;
        if (t > tmin)
            tmin = t;
    }
    else if (delt_p < 0.0)
    {
        t = (botlim - p0) / delt_p;
        if (t < tmax)
            tmax = t;
        t = (toplim - p0) / delt_p;
        if (t > tmin)
            tmin = t;
    }
    else
    {
        if ((p0 < botlim) || (p0 > toplim))
            return 0;
    }
    ray.SetTs(tmin, tmax);
    /* Check that all this clipping has not removed the interval. */

    return tmin < tmax;
}

void SiddonProjector::CastRay(const Block &box, Ray& ray, RayCast& RayCast) const{
    Point3D stp = ray.get_startpoint();      //get the start point of the ray.
    // get the bottom bound of the image field.
    Point3D p0 = stp - box.get_bottom_bound();
    // translate the origin to the corner of the image volume
    ray.set_startpoint(p0);
    RayCast.SetupRayCast(box, ray);
}

}
