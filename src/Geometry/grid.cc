/*
  @brief:  a grid is a 3D matrix which can be used to represent a PET image.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "grid.h"
#include "gridsize.h"
#include "block.h"
namespace BBSLMIRP {


Grid3D::Grid3D(){

}
Grid3D::~Grid3D(){

}

bool Grid3D::Initialize(const ObliqueBlock& block){
    this->block_origin = block;
    //this->block_origin.Initialize(block.get_num_grid(),block.get_geometry_size(),block.get_center());
    GridSize grid_size = block_origin.get_num_grid();
    int num_x = grid_size.get_num_x();
    int num_y = grid_size.get_num_y();
    int num_z = grid_size.get_num_z();
    mesh_value.clear();
    mesh_value.resize(num_x);
    for(auto& column : mesh_value){
        column.resize(num_y);
        for(auto& layer : column){
            layer.resize(num_z);
        }
    }
    return true;
}

void Grid3D::SetBlockGrid(const GridSize &block_grid){
    this->block_origin.set_num_grid(block_grid);
}

void Grid3D::set_mesh(const MeshIndex &mi, float value){
    int ix = mi.get_num_x();
    int iy = mi.get_num_y();
    int iz = mi.get_num_z();
    mesh_value[ix][iy][iz] = value;
}

void Grid3D::set_mesh(int mesh_index, float value ){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int plane = nx*ny;
    int line = nx;
    int iz =(mesh_index/plane);
    int iy =((mesh_index-iz*plane)/line);
    int ix =((mesh_index%plane)%line);
    mesh_value[ix][iy][iz] =value;
}

float Grid3D::get_mesh(const MeshIndex &mi) const{
    int ix = mi.get_num_x();
    int iy = mi.get_num_y();
    int iz = mi.get_num_z();
    return mesh_value[ix][iy][iz];
}

float Grid3D::get_mesh(int mesh_index)const{
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int plane = nx*ny;
    int line = nx;
    int iz =(mesh_index/plane);
    int iy =((mesh_index-iz*plane)/line);
    int ix =((mesh_index%plane)%line);
    return mesh_value[ix][iy][iz];
}



bool Grid3D::SizeCheck(const Grid3D& gd) const{
    if(this->block_origin.get_num_grid().IsEqualSize(gd.get_block().get_num_grid() ))
        return true;
    else
        return false;
}

const ObliqueBlock &Grid3D::get_block() const{
    return block_origin;
}

float Grid3D::Sum() const{
    float sum = 0;
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                sum += mesh_value[ix][iy][iz];
            }
        }
    }
    return sum;
}

void Grid3D::SetAllMeshes(float value){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                mesh_value[ix][iy][iz] = value;
            }
        }
    }
}

void Grid3D::Add(float value){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                mesh_value[ix][iy][iz] += value;
            }
        }
    }
}

void Grid3D::Minus(float value){
    this->Add(-value);
}

void Grid3D::Multiple(float value){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                mesh_value[ix][iy][iz] *= value;
            }
        }
    }
}

void Grid3D::Divide(float value){
    if (value == 0){
        std::cout<<"devideAllGridValue():zero dividor! "<<std::endl;
        std::exit(-1);
    }
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                mesh_value[ix][iy][iz] /= value;
            }
        }
    }
}

void Grid3D::Pow(float order){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix < nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                mesh_value[ix][iy][iz] = std::pow(mesh_value[ix][iy][iz],order);
            }
        }
    }
}

void Grid3D::Absolute(){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                mesh_value[ix][iy][iz] = std::abs(mesh_value[ix][iy][iz]);
            }
        }
    }
}

void Grid3D::Sqrt(){
    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();
    for(int ix = 0; ix<nx; ix++){
        for(int iy = 0; iy < ny; iy++){
            for(int iz = 0; iz < nz; iz++){
                if(mesh_value[ix][iy][iz]<0)
                    mesh_value[ix][iy][iz] = 0;
                else
                    mesh_value[ix][iy][iz] = std::sqrt(mesh_value[ix][iy][iz]);
            }
        }
    }
}

void Grid3D::Conv3(const Grid3D &filter){
    GridSize filter_grid = filter.get_block().get_num_grid();
    GridSize image_grid = this->get_block().get_num_grid();
    GridSize filtered_grid = filter_grid+image_grid;
    ObliqueBlock bok;
    bok.Initialize(filtered_grid,Point3D(),Point3D(), 0, 0);
    Grid3D filtered;
    filtered.Initialize(bok);
    //filtered.SetBlockGrid(filtered_grid);

    int nx = this->block_origin.get_num_grid().get_num_x();
    int ny = this->block_origin.get_num_grid().get_num_y();
    int nz = this->block_origin.get_num_grid().get_num_z();

    int fx = filter.get_block().get_num_grid().get_num_x();
    int fy = filter.get_block().get_num_grid().get_num_y();
    int fz = filter.get_block().get_num_grid().get_num_z();
    //padding the original image cube.
    for(int ix = 0;ix<nx;ix++){
        for(int iy = 0;iy<ny;iy++){
            for(int iz = 0; iz<nz;iz++){
                GridSize loc1(ix,iy,iz);
                GridSize loc2(ix+fx,iy+fy,iz+fz);
                float value = this->get_mesh(loc1);
                filtered.set_mesh(loc2,value);
            }
        }
    }

    //convolution
    for(int ix = 0;ix<nx;ix++){
        for(int iy = 0;iy<ny;iy++){
            for(int iz = 0;iz<nz;iz++){
                //Grid3D batch = gd;
                GridSize loc(ix,iy,iz);
                float value=0;
                for(int ibx = 0;ibx < fx;ibx++){
                    for(int iby = 0;iby < fy;iby++){
                        for(int ibz = 0;ibz < fz;ibz++){
                            GridSize floc(ibx,iby,ibz);
                            GridSize b(ix+ibx,iy+iby,iz+ibz);
                            value += filtered.get_mesh(b)*filter.get_mesh(floc);
                        }
                    }
                }
                this->set_mesh(loc,value);
            }
        }
    }
}

Grid3D Grid3D::operator+(const Grid3D& gd){
    Grid3D addGD = gd;
    if(this->SizeCheck(gd)){// same size.
        int nX = this->get_block().get_num_grid().get_num_x();
        int nY = this->get_block().get_num_grid().get_num_y();
        int nZ = this->get_block().get_num_grid().get_num_z();
        for(int iX = 0;iX<nX;iX++){
            for(int iY = 0;iY<nY;iY++){
                for(int iZ = 0; iZ<nZ;iZ++){
                    GridSize loc(iX,iY,iZ);
                    float value = this->get_mesh(loc)+gd.get_mesh(loc);
                    addGD.set_mesh(loc,value);
                }
            }
        }
        return addGD;
    }
    else{
        std::cout<<"Grid3D addtion: two grids have different grid size! "<<std::endl;
        std::exit(-1);
    }
}

Grid3D Grid3D::operator-(const Grid3D& gd){
    Grid3D minusGD = gd;
    if(this->SizeCheck(gd)){// same size.
        int nX = this->get_block().get_num_grid().get_num_x();
        int nY = this->get_block().get_num_grid().get_num_y();
        int nZ = this->get_block().get_num_grid().get_num_z();
        for(int iX = 0;iX<nX;iX++){
            for(int iY = 0;iY<nY;iY++){
                for(int iZ = 0; iZ<nZ;iZ++){
                    GridSize loc(iX,iY,iZ);
                    float value = this->get_mesh(loc)-gd.get_mesh(loc);
                    minusGD.set_mesh(loc,value);
                }
            }
        }
        return minusGD;
    }
    else{
        std::cout<<"Grid3D substraction: two grids have different grid size! "<<std::endl;
        std::exit(-1);
    }
}

Grid3D Grid3D::operator/(const Grid3D& gd){
    Grid3D divideGD = gd;
    //float k = kEps;
    if(this->SizeCheck(gd)){// same size.
        int nX = this->get_block().get_num_grid().get_num_x();
        int nY = this->get_block().get_num_grid().get_num_y();
        int nZ = this->get_block().get_num_grid().get_num_z();
        for(int iX = 0;iX<nX;iX++){
            for(int iY = 0;iY<nY;iY++){
                for(int iZ = 0; iZ < nZ;iZ++){
                    GridSize loc(iX,iY,iZ);
                    if(std::abs(gd.get_mesh(loc)) <= kEps){ // if the dividor is zero, set the result zero.
                        divideGD.set_mesh(loc,this->get_mesh(loc));
                    }
                    else{
                        float value = this->get_mesh(loc)/gd.get_mesh(loc);
                        divideGD.set_mesh(loc,value);
                    }
                }
            }
        }
        return divideGD;
    }
    else{
        std::cout<<"Grid3D division: two grids have different grid size! "<<std::endl;
        std::exit(-1);
    }
}

Grid3D Grid3D::operator*(const Grid3D& gd){
    Grid3D multiGD = gd;
    if(this->SizeCheck(gd)){// same size.
        int nX = this->get_block().get_num_grid().get_num_x();
        int nY = this->get_block().get_num_grid().get_num_y();
        int nZ = this->get_block().get_num_grid().get_num_z();
        for(int iX = 0;iX<nX;iX++){
            for(int iY = 0;iY<nY;iY++){
                for(int iZ = 0; iZ<nZ;iZ++){
                    GridSize loc(iX,iY,iZ);
                    float value = this->get_mesh(loc)*gd.get_mesh(loc);
                    multiGD.set_mesh(loc,value);
                }
            }
        }
        return multiGD;
    }
    else{
        std::cout<<"Grid3D multiplication: two grids have different grid size! "<<std::endl;
        std::exit(-1);
    }
}
}


