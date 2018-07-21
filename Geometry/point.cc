/*
  @brief:  the definition of point 
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "point.h"
#include "sphericalpoint.h"
#include "gridsize.h"
/*
  @brief:  the definition of Point3D
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
 */

namespace BBSLMIRP
{
const float kPi = std::acos(-1);
//constructor
Point3D::Point3D()
{
    //if no parameter, set the point in origin.
    SetPoint(0.0, 0.0, 0.0);
}
Point3D::~Point3D()
{
}

Point3D::Point3D(const Point3D &pt)
{
    SetPoint(pt.get_px(), pt.get_py(), pt.get_pz());
}

Point3D::Point3D(float px, float py, float pz)
{
    SetPoint(px, py, pz);
}

// Point3D::Point3D(const GridSize& gs){
//     SetPoint((float)gs.getnX(), (float)gs.getnY(),(float)gs.getnZ());
// }

void Point3D::SetPoint(float px, float py, float pz)
{
    this->px = px;
    this->py = py;
    this->pz = pz;
}

float Point3D::GetPointByIndex(int index) const
{
    switch (index)
    {
    case 0:
        return px;
    case 1:
        return py;
    case 2:
        return pz;
    default:
        // TO BE DELETED WHEN THE DEBUGGING IS PASSED!
        std::cout << "Point3D::getp(): " << index << " is invalid index! " << std::endl;
        std::exit(-1);
    }
}

void Point3D::SetPointByIndex(float value, int index)
{
    switch (index)
    {
    case 0:
        this->set_px(value);
        break;
    case 1:
        this->set_py(value);
        break;
    case 2:
        this->set_pz(value);
        break;
    default:
        // TO BE DELETED WHEN THE DEBUGGING IS PASSED!
        std::cout << "Point3D::getp(): " << index << " is invalid index! " << std::endl;
        std::exit(-1);
        break;
    }
}

Point3D Point3D::operator/(const Point3D &pt)
{
    Point3D thePt(0, 0, 0);
    //when the divisor has 0 component, Set the result to be zero.
    if (pt.get_px() == 0)
        thePt.set_px(0);
    else
        thePt.set_px(this->get_px() / pt.get_px());
    if (pt.get_py() == 0)
        thePt.set_py(0);
    else
        thePt.set_py(this->get_py() / pt.get_py());
    if (pt.get_pz() == 0)
        thePt.set_pz(0);
    else
        thePt.set_pz(this->get_pz() / pt.get_pz());
    //input point is taken as the divisor, and 'this point' is the dividend.
    return thePt;
}

Point3D Point3D::operator/(float ref)
{
    if (ref == 0)
    { // divisor can not be 0.
        //std::cout<<"invalid value! point3D divided by zero!"<<std::endl;
        Point3D pt(0, 0, 0);
        return pt;
    }
    Point3D pt(this->get_px() / ref, this->get_py() / ref, this->get_pz() / ref);
    return pt;
}

bool Point3D::operator==(const Point3D &pt1) const
{
    if (this->get_px() == pt1.get_px() && this->get_py() == pt1.get_py() && this->get_pz() == pt1.get_pz())
        return true;
    else
        return false;
}

// float Point3D::CalculateDista(const Point3D &pt1) const{
//     Point3D dir(float(px-pt1.get_px()),float(py-pt1.get_py()),float(pz-pt1.get_pz()));
//     float dist =std::sqrt(std::pow(dir.get_px(),2)+std::pow(dir.get_py(),2)+std::pow(dir.get_pz(),2));
//     return dist;
// }

SphericalPoint3D Point3D::CartesianToSpherical() const
{
    SphericalPoint3D spt(0, 0, 0);
    spt.set_radius(sqrt(pow(this->get_px(), 2) + pow(this->get_py(), 2) + pow(this->get_pz(), 2)));
    if (spt.get_radius() < 0)
    { // the radius shoule not be negative.
        // TO BE DELETED WHEN THE DEBUGGING IS PASSED!
        std::cout << "transfrom error from rectangle coordinate system point to spherical coordinate system !" << std::endl;
    }
    else
    {
        spt.set_theta(std::acos(this->get_pz() / spt.get_radius()));
    }
    float phi = atan2(this->get_py(), this->get_px());
    //add M_PI to shift the range of phi to 0~2M_PI.
    spt.set_phi((phi >= 0) ? phi : phi + 2 * kPi);
    return spt;
}

void Point3D::SphericalToCartesian(const SphericalPoint3D& spt)
{
    this->set_px(spt.get_radius()*sin(spt.get_theta())*cos(spt.get_phi()));
    this->set_py(spt.get_radius()*sin(spt.get_theta())*sin(spt.get_phi()));
    this->set_pz(spt.get_radius()*cos(spt.get_theta()));
}

// calculate the  L2 norm
float Point3D::GetNorm2() const
{
    return std::sqrt(std::pow(this->get_px(), 2) + std::pow(this->get_py(), 2) + std::pow(this->get_pz(), 2));
}

Point3D Point3D::GridSize_to_Point3D(const GridSize &gs){
    return Point3D(gs.get_num_x(),gs.get_num_y(),gs.get_num_z());
}


float Point3D::ComputeScalarProduct(const Point3D &pt1, const Point3D &pt2){
    return     (pt1.get_px() * pt2.get_px()+
                pt1.get_py() * pt2.get_py()+
                pt1.get_pz() * pt2.get_pz());
}
Point3D Point3D::ComputeCrossProduct(const Point3D& pt1, const Point3D& pt2)
{return Point3D(pt2.get_py()*pt1.get_pz()-pt1.get_py()*pt2.get_pz(),
                pt2.get_pz()*pt1.get_px()-pt1.get_pz()*pt2.get_px(),
                pt2.get_px()*pt1.get_py()-pt1.get_px()*pt2.get_py());}




/**********************************************/
/****** class function definition above *******/
}
