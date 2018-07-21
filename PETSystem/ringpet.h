#ifndef RINGPET_H
#define RINGPET_H

/*
  @brief:  concrete children class of 'PETScanner' to define a conventional ring PET geometry
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/
#include "petscanner.h"
#include <vector>
#include "../Geometry/obliqueblock.h"

namespace BBSLMIRP
{

class RingPET : public PETScanner
{
public:
    RingPET(const std::string& scanner_file);
    virtual ~RingPET();
    virtual void Initialize(const std::string& scanner_file ) override;
    virtual int  LocateBlock(const Point3D &pt) const;
    virtual const Block& get_block(int index) const override{return block_list[index];}
    virtual const Patch& get_patch(int patch_index)const{}
    virtual int get_num_blocks() const override {return block_list.size();}
    virtual void DescribeSelf() const override;
    //virtual void ComputeNormalizationMap(const std::string &efficiency_map_name, Grid3D &img) const;
    //virtual void TransformEvents(const std::string &event_file_input, const std::string &event_file_output) const;

    // setter and getter
    virtual void set_inner_radius(float radius) { this->inner_radius = radius; }
    virtual void set_outer_radius(float radius) { this->outer_radius = radius; }
    virtual void set_height(float height) { this->height = height; }
    virtual void set_ring_gap(float ring_gap) { this->ring_gap = ring_gap; }
    virtual void set_num_rings(int nRings) { this->num_rings = nRings; }
    virtual void set_num_blocks_per_ring(int nBlocks) { this->num_blocks_per_ring = nBlocks; }

    virtual float get_inner_radius() const { return this->inner_radius; }
    virtual float get_outer_radius() const { return this->outer_radius; }
    virtual float get_height() const { return this->height; }
    virtual float get_ring_gap() const { return this->ring_gap; }
    virtual int   get_num_rings() const { return this->num_rings; }
    virtual int   get_num_blocks_per_rings() const { return this->num_blocks_per_ring; }

    // generate the blocks in the block list.
    virtual void  make_block_list(ObliqueBlock& origin);

  private:
    float inner_radius;
    float outer_radius;
    float height;
    float ring_gap;
    int   num_rings;
    int   num_blocks_per_ring;
    std::vector<ObliqueBlock> block_list;

};
}
#endif //RINGPET_H
