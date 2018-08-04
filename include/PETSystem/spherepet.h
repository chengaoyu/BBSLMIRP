#ifndef SpherePET_H
#define SpherePET_H
/*
  @brief:  
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include "petscanner.h"
#include <vector>
#include "obliqueblock.h"
#include "patch.h"
namespace BBSLMIRP{

class SpherePET:public PETScanner{
public:
    SpherePET(const std::string & scanner_file);
    virtual ~SpherePET();

    virtual void Initialize(const std::string &scanner_file ) override;
    virtual int LocateBlock(const Point3D& pt)const;
    virtual int get_num_blocks()const override{return block_list.size();}
    virtual const Block& get_block(int block_index)const override{return block_list[block_index];}
    virtual void DescribeSelf() const override;

    virtual const Patch& get_patch(int patch_index)const {return patch_list[patch_index];}

    // setter and getter
    virtual void set_inner_radius(float radius) { this->inner_radius = radius; }
    virtual void set_outer_radius(float radius) { this->outer_radius = radius; }
    virtual float get_inner_radius() const { return this->inner_radius; }
    virtual float get_outer_radius() const { return this->outer_radius; }

    virtual void set_mesh_size(const Point3D& mesh_size){this->mesh_size = mesh_size;}
    virtual const Point3D& get_mesh_size()const{
        return mesh_size;
    }

    virtual void make_block_list(const std::string& patch_list_file);

private:
    float inner_radius;
    float outer_radius;
    Point3D mesh_size;
    std::vector<ObliqueBlock> block_list;
    std::vector<Patch> patch_list;
};

}

#endif //SpherePET_H
