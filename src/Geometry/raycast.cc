/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/30
*/

#include "raycast.h"

#include "obliqueblock.h"
#include "ray.h"

#include <limits>
namespace BBSLMIRP {

/* class---RayCast
 * function definitions
*/
RayCast::RayCast(){

}
RayCast::~RayCast(){

}


/****************************************************************************
 * SetupRayCast()
 * setup for fast processing of line length calculations
 * IMPORTANT: assumes that the ray has already been intersected with
 * the volume bounding box
****************************************************************************/
void RayCast::SetupRayCast(const Block &imgbox, const Ray &ray){
    int pos_x, pos_y, pos_z;

    pos_x = SetupRayCastComponent(imgbox, ray, 0);
    pos_y = SetupRayCastComponent(imgbox, ray, 1);
    pos_z = SetupRayCastComponent(imgbox, ray, 2);

    int num_x = imgbox.get_num_grid().get_num_x();
    int num_y = imgbox.get_num_grid().get_num_y();
    /* return buffer offset for first voxel along ray */
    this->boffs = pos_x + num_x*(pos_y + num_y*pos_z);
}

/****************************************************************************
 * SetupRayCastComponent()
 * compute distance to next crossing, nextT,
 * distance along ray between successive crossings, deltaT,
 * the increment of the buffer offset for a crossing, deltaBuf,
 * the number of crossings which take us out of the imaging volume, inBuf,
 * and return the index of the voxel component
****************************************************************************/

int RayCast::SetupRayCastComponent(const Block& imgbox, const Ray& ray, int comp)
{
    int pos, i, v_count;
    float pt;
    float v_res;

    float r_pdir =ray.get_direction().GetPointByIndex(comp);
    float p0 = ray.get_startpoint().GetPointByIndex(comp);
    float mint = ray.get_min_t();
    /* compute component of point at intersection of volume bounding box */

    pt = mint * r_pdir + p0;

    /* get local copies of resolution, and dimension */

    v_res = imgbox.get_interval().GetPointByIndex(comp);

    v_count = imgbox.get_num_grid().GetAxis(comp);

    if (r_pdir > 0.0) {

        /* going to the right, so round down */

        pos = pt / v_res;
//        nextT.setp(((pos+1) * v_res - pt) / r_pdir, comp);
//        deltaT.setp(v_res / r_pdir,comp);
//        deltaBuf.setn(1,comp);
//        inBuf.setn(v_count - pos,comp);
        nextT[comp] = ((pos+1) * v_res - pt) / r_pdir;
        deltaT[comp] = v_res / r_pdir;
        deltaBuf[comp] = 1;
        inBuf[comp] = v_count - pos;
        
        //debug
        // std::cout<<"r_pdir > 0"<<std::endl;

    } else if (r_pdir < 0.0) {

        /* going to the left, so round up and subtract 1 */

        pos = v_count - 1 - (int)(v_count - pt / v_res);
//        nextT.setp((pos * v_res - pt) / r_pdir,comp);
//        deltaT.setp(-v_res / r_pdir,comp);
//        deltaBuf.setn(-1,comp);
//        inBuf.setn(pos+1,comp);
        nextT[comp] = (pos * v_res - pt) / r_pdir;
        deltaT[comp] = -v_res / r_pdir;
        deltaBuf[comp] = -1;
        inBuf[comp] = pos + 1;
        // debug
        // std::cout<<"r_pdir < 0"<<std::endl;

    } else {

        pos = pt / v_res;
//        nextT.setp(HUGE,comp);
//        deltaT.setp(HUGE,comp);
//        deltaBuf.setn(0,comp);
//        inBuf.setn(1,comp);
        // debug
        // std::cout<<"r_pdir = 0"<<std::endl;

        nextT[comp] = std::numeric_limits<float>::max();
        deltaT[comp] = std::numeric_limits<float>::max();
        deltaBuf[comp] = 0;
        inBuf[comp] = 1;
    }

    /* get the correct spacing for the buffer pointer changes */

    for (i=comp-1; i>=0; i--)
        deltaBuf[comp] *= imgbox.get_num_grid().GetAxis(i);
//        deltaBuf.setn(deltaBuf.getn(comp)*imgbox.getBlockGrid().getn(i),comp);

    return pos;
}


}
