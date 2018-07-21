#ifndef RAYCAST_H
#define RAYCAST_H
/*
  @brief:  a raycast to help computing the ray tracing result.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/30
*/

namespace BBSLMIRP {
class Block;
class Ray;

class RayCast
{
public:
    RayCast();
    virtual ~RayCast();
    virtual void SetupRayCast(const Block &imgbox, const Ray& ray);
    virtual int  SetupRayCastComponent(const Block& imgbox, const Ray& ray, int comp);

    virtual int get_boffs()const{return boffs;}
    virtual float get_nextT(int index)const{return nextT[index];}
    virtual float get_deltaT(int index)const{return deltaT[index];}
    virtual float get_deltaBuf(int index)const{return deltaBuf[index];}
    virtual float get_inBuf(int index)const{return inBuf[index];}

    virtual void set_boffs(int value){this->boffs = value;}
    virtual void set_nextT(float value,int index){this->nextT[index] = value;}
    virtual void set_deltaT(float value,int index){this->deltaT[index] = value;}
    virtual void set_deltaBuf(int value,int index){this->deltaBuf[index] = value;}
    virtual void set_inBuf(int value, int index){this->inBuf[index] = value;}
private:
    int boffs;
    float nextT[3];
    float deltaT[3];
    int deltaBuf[3];
    int inBuf[3];
};
}

#endif // RAYCAST_H
