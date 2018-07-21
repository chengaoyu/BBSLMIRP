#ifndef GRIDSIZE_H
#define GRIDSIZE_H

/*
  @brief:  this file gives the class declaration of 3 dimensional grid size.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
 */

namespace BBSLMIRP{

class GridSize
{
public:
    GridSize();
    GridSize(int nx,int ny, int nz);
    virtual ~GridSize();

    // set the size in 3 coordinates of the grid.
    virtual void set_num_x(int nx){ this->num_x = nx; }
    virtual void set_num_y(int ny){ this->num_y = ny; }
    virtual void set_num_z(int nz){ this->num_z = nz; }
    // get the grid size of 3 coordinates.
    virtual int get_num_x() const{ return num_x;}
    virtual int get_num_y() const{ return num_y;}
    virtual int get_num_z() const{ return num_z;}


    virtual long int GetTotalNumOfMeshes()const{ return num_x*num_y*num_z ;}

    // set the whole three axis
    virtual void SetGridSize(int nx, int ny, int nz);
    
    // seperately set and get the number in three axis
    virtual void SetAxis(int num, int axis_index);
    virtual int GetAxis(int axis_index) const;
    
    // return true if the two grid have equal size. 
    virtual bool IsEqualSize(const GridSize& gs) const;

    virtual GridSize operator+(const GridSize& gs) const;
    virtual bool IsInside(const GridSize& gs) const;
private:
    int num_x;
    int num_y;
    int num_z;
};

}

#endif //GRIDSIZE_H
