#ifndef POINT_H
#define POINT_H

/*
  @brief:  this file gives the class declaration of Point in three dimensional space
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
 */

//to be deleted when the debug is passed
#include <iostream>
#include "sphericalpoint.h"
#include <cmath>


namespace BBSLMIRP{
class GridSize;
class Point3D{
public:
	// constructor
	Point3D();
	Point3D(const Point3D &pt);
	Point3D(float px, float py, float pz);
	//transform a GridSize object and create a Point3D object;
	//Point3D(const GridSize &gs);
	virtual ~Point3D();

	// setter
	virtual void set_px(float px) { this->px = px; }
	virtual void set_py(float py) { this->py = py; }
	virtual void set_pz(float pz) { this->pz = pz; }
	// getter
	virtual float get_px() const { return px; }
	virtual float get_py() const { return py; }
	virtual float get_pz() const { return pz; }

	// set the three coordinates of the point.
	virtual void SetPoint(float px, float py, float pz);
	// set and get the components by index.(for loops)
	virtual float GetPointByIndex(int index) const;
	virtual void SetPointByIndex(float value, int index);

	// calculate the distance between two points.
	// virtual float CalculateDistance(const Point3D &pt1) const;

    Point3D operator+(const Point3D &pt1) { return Point3D(px + pt1.get_px(), py + pt1.get_py(), pz + pt1.get_pz()); }
    Point3D operator-(const Point3D &pt1) { return Point3D(px - pt1.get_px(), py - pt1.get_py(), pz - pt1.get_pz()); }
    Point3D operator*(const Point3D &pt1) {	return Point3D(px * pt1.get_px(), py * pt1.get_py(), pz * pt1.get_pz()); }
    Point3D operator*(float ref){return Point3D(px*ref,py*ref,pz*ref);}
    Point3D operator/(const Point3D &pt);
	Point3D operator/(float ref);

    // compute the scalar product and cross product of two vectors (Point3D).
    static float ComputeScalarProduct(const Point3D& pt1, const Point3D& pt2);
    static Point3D ComputeCrossProduct(const Point3D& pt1, const Point3D& pt2);


    static Point3D GridSize_to_Point3D(const GridSize& gs);
	bool operator==(const Point3D &pt) const;
	
	// Point3D operator=(const Point3D& pt1);
	// transform a point in Cartisian coordinate system to a point in spherical coordinate system.
	virtual SphericalPoint3D  CartesianToSpherical() const;
	// transform a point in spherical coordinate system to a point in Cartesian coordinate system.
	virtual void SphericalToCartesian(const SphericalPoint3D& spt);
	// compute the L2 norm of the three axis value.
	virtual float GetNorm2() const;

private:
	float px;
	float py;
	float pz;
};
}
#endif // !POINT_H
