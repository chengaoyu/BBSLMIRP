/*
  @brief:  the definition of methods in GridSize. 
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
 */

#include "gridsize.h"

namespace BBSLMIRP
{

GridSize::GridSize()
{
    //default initialization
    this->SetGridSize(10, 10, 10);
}

GridSize::GridSize(int nx, int ny, int nz)
{
    this->SetGridSize(nx, ny, nz);
}

GridSize::~GridSize()
{
    //do nothing
}

void GridSize::SetGridSize(int nx, int ny, int nz)
{
    this->set_num_x(nx);
    this->set_num_y(ny);
    this->set_num_z(nz);
}

void GridSize::SetAxis(int value, int axis_index)
{
    switch (axis_index)
    {
    case 0:
        this->set_num_x(value);
        break;
    case 1:
        this->set_num_y(value);
        break;
    case 2:
        this->set_num_z(value);
        break;
    default:
        break;
    }
}

int GridSize::GetAxis(int index) const
{
    switch (index)
    {
    case 0:
        return this->num_x;
    case 1:
        return this->num_y;
    case 2:
        return this->num_z;
    default:
        return -1;
    }
}

bool GridSize::IsEqualSize(const GridSize &gs) const
{
    if ((this->get_num_x() == gs.get_num_x()) 
       && (this->get_num_y() == gs.get_num_y()) 
       && (this->get_num_z() == gs.get_num_z()))
        return true;
    else
        return false;
}

GridSize GridSize::operator+(const GridSize& gs)const{
    GridSize add_gs(this->get_num_x()+gs.get_num_x(),
                    this->get_num_y()+gs.get_num_y(),
                    this->get_num_z()+gs.get_num_z());
    return add_gs;
}

bool GridSize::IsInside(const GridSize &gs) const{
    if (this->num_x >= gs.get_num_x()||this->num_x<0){
        return false;
    }
    if (this->num_y >= gs.get_num_y()||this->num_y<0){
        return false;
    }
    if (this->num_z >+ gs.get_num_z()||this->num_z<0){
        return false;
    }
    return true;
}


}
