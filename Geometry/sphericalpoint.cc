/*
  @brief:  the definition of point in spherical coordinate.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
 */

#include "sphericalpoint.h"
#include <cmath>
 /* class---SphericalPoint3D
 * function definitions
*/

namespace BBSLMIRP{
const float kPi = std::acos(-1);
SphericalPoint3D::SphericalPoint3D(){
    SetPoint(0.0,0.0,0.0);
}

SphericalPoint3D::SphericalPoint3D(float radius, float theta, float phi){
    SetPoint(radius,theta,phi);
}

SphericalPoint3D::~SphericalPoint3D(){

}

void SphericalPoint3D::SetPoint(float radius, float theta, float phi){

    this->set_radius(radius);
    this->set_theta(theta);
    this->set_phi(phi);
}

void SphericalPoint3D::set_radius(float radius){
    if(radius<0){
        std::cout<<"radius can not be negative!"<<std::endl;
        this->radius = 0;
    }
    else{
        this->radius = radius;
    }
}

void SphericalPoint3D::set_theta(float theta){
    //the polar angle ranges from 0~M_PI.
    if(theta>kPi || theta<0){
        std::cout<<"polar angle out of range!"<<std::endl;

    }
    else{
        this->theta = theta;
    }
}

void SphericalPoint3D::set_phi(float phi){
    // the azimuth angle ranges from 0~2M_PI
    if(phi>2*kPi || phi<0){
        std::cout<<"azimuth angle out of range!"<<std::endl;
    }
    else {
        this->phi =phi;
    }
}

float SphericalPoint3D::get_radius() const{
    return radius;
}

float SphericalPoint3D::get_theta() const{
    return theta;
}

float SphericalPoint3D::get_phi() const{
    return phi;
}

//calaulate the distance between two points in spherical coordinate system.
// float SphericalPoint3D::calculateDist(const SphericalPoint3D &spt1) const{

//     Point3D pt = this->SPT2PT();
//     Point3D pt1 = spt1.SPT2PT();
//     return pt.calculateDist(pt1);
// }

//transform a point in spherical coordinate system to a point in rectangular coordinate system.
// Point3D SphericalPoint3D::SPT2PT() const{
//     Point3D pt(0,0,0);
//     pt.setpx(this->get_radius()*sin(this->get_theta())*cos(this->get_phi()));
//     pt.setpy(this->get_radius()*sin(this->get_theta())*sin(this->get_phi()));
//     pt.setpz(this->get_radius()*cos(this->get_theta()));
//     return pt;
// }

//transform a point in rectangle coordinate system to a point in spherical coordinate system.
// SphericalPoint3D Point3D::PT2SPT() const{
//     SphericalPoint3D spt(0,0,0);
//     spt.setRadius(sqrt(pow(this->getpx(),2)+pow(this->getpy(),2)+pow(this->getpz(),2)));
//     if(spt.get_radius()<0){// the radius shoule not be negative.
//         std::cout<<"transfrom error from rectangle coordinate system point to spherical coordinate system !"<<std::endl;
//     }
//     else{
//         spt.set_theta(acos(this->getpz()/spt.get_radius()));
//     }
//     float phi = atan2(this->getpy(),this->getpx());
//     //spt.setPhi(phi);
//     //add M_PI to shift the range of phi to 0~2M_PI.
//     spt.setPhi(phi= (phi>=0)? phi:phi+2*M_PI);
//     return spt;
// }

}
