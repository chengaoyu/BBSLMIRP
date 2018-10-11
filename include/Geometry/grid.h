#ifndef GRID_H
#define GRID_H

/*
  @brief:  this file gives the clas
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/


#include<vector>
#include"obliqueblock.h"
namespace BBSLMIRP{

const float kEps = 0.0000001;

class GridSize;
typedef GridSize MeshIndex;

class Grid3D{
public:
    Grid3D();
    virtual ~Grid3D();
    virtual bool Initialize(const ObliqueBlock &);

    virtual void SetBlockGrid(const MeshIndex& block_grid);

    virtual void set_mesh(const MeshIndex& mi, float value);
    virtual float get_mesh(const MeshIndex& mi) const;

    virtual void set_mesh(int mi, float value);
    virtual float get_mesh(int mi) const;

    //virtual void set_block(const VerticalBlock&);
    virtual const ObliqueBlock& get_block() const;
    //sum the GDvalue in the grid.
    virtual float Sum() const;
    // compare the  blockgrid of two Grid3D object.
    virtual bool  SizeCheck(const Grid3D& gd) const;

    //the all the meshes to one value.
    virtual void SetAllMeshes(float value);
    virtual void Add(float value);
    virtual void Minus(float value);
    virtual void Multiple(float value);
    virtual void Divide(float value);

    //calculate the exponent of the mesh values.
    virtual void Pow(float order);
    //compute the square root of the mesh values
    virtual void Sqrt();
    //get the absolution of the mesh values
    virtual void Absolute();

    //find the minimum and maxmium value of the 'GDValue'
    //virtual void FindMinAndMax(float& min, float& max) const;

    // 3D filtering of the grid.
    virtual void Conv3(const Grid3D & filter);

    // arithmetic operation of two grids.
    Grid3D  operator+(const Grid3D& gd);
    Grid3D  operator-(const Grid3D& gd);
    Grid3D  operator*(const Grid3D& gd);
    Grid3D  operator/(const Grid3D& gd);



private:
    ObliqueBlock block_origin;
    //the value in each mesh
    std::vector<std::vector<std::vector<float> > > mesh_value;
};

}

#endif  //GRID_H
