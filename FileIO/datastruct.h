#ifndef DATASTRUCT_H
#define DATASTRUCT_H
/*
  @brief:   
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include<string>
#include<vector>
namespace BBSLMIRP
{

// contains 7 float to read the tof coincidence float from input list-mode data.
struct CoinEvent{
    float pos1_x;
    float pos1_y;
    float pos1_z;
    float pos2_x;
    float pos2_y;
    float pos2_z;
    float tof_dis;
};
typedef std::vector<CoinEvent> CoinEventList;
class Point3D;
class Ray;

class Event{
public:
    Event(const Point3D& p1, const Point3D& p2, float time_diff);
    virtual ~Event();
    virtual Ray& get_ray(){return *ray;}
    virtual float get_tof_distance()const {return tof_distance;}
    virtual void set_tof_distance(float tof_distance){this->tof_distance = tof_distance;}
    // get the length between the mint point to the annihilation center.
    virtual float get_length_mint_to_center() const;
private:
    Ray* ray;
    // the distance (0.5*lightspeed*(time2-time1)) between the Ray center to the annihilation point,
    // always be positive which means the annihilation is taken place closer to pt1 (time2>time1).
    float tof_distance;
};

struct LMElement{
    float length;
    int mesh_index;
};
struct LMEvent{
    std::vector<LMElement> array;
    int num_voxels;
    int num_events;
};
struct TOF{
    float FWHM;
    float Sigma2;
    float binsize;
    float limit2;
};

}

#endif //FILESTRUCT_H
