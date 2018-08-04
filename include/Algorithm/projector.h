#ifndef PROJECTOR_H
#define PROJECTOR_H

namespace BBSLMIRP {
class Block;
class Grid3D;
class Event;
class Ray;
class LMEvent;
class TOF;
class Projector{
public:
    virtual void MakeListModeEvent (Event& evt, const Block& box, LMEvent& lmevt) const= 0;
    virtual void MappingLine(Ray& evt, Grid3D &image)const = 0;
    virtual const TOF& get_tof() = 0;
    virtual void set_tof(const TOF& tof) = 0;
};
}

#endif // PROJECTOR_H
