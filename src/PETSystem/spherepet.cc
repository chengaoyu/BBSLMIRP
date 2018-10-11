/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11/03
*/
#include "spherepet.h"
#include "filestruct.h"
#include <float.h>
namespace BBSLMIRP
{

SpherePET::SpherePET(const std::string &scanner_file)
{
    this->Initialize(scanner_file);
}
SpherePET::~SpherePET()
{
}

void SpherePET::DescribeSelf() const
{
}
void SpherePET::Initialize(const std::string &scanner_file)
{
    std::ifstream configure(scanner_file, std::ios_base::in);
    float in_radius, out_radius;
    float mx, my, mz;
    std::string patch_list_file;
    float fov_ratio = 0.9;
    std::string key;
    while (!configure.eof())
    {
        configure >> key;
        if (key == "PETScanner:")
            configure >> key;
        else if (key == "InnerRadius:")
            configure >> in_radius;
        else if (key == "OuterRadius:")
            configure >> out_radius;
        else if (key == "MeshSize:")
            configure >> mx >> my >> mz;
        else if (key == "PatchListName:")
            configure >> patch_list_file;
        else if (key == "FovRatio")
            configure >> fov_ratio;
        else
        {
        }
    }
    configure.close();

    this->set_inner_radius(in_radius);
    this->set_outer_radius(out_radius);
    this->set_mesh_size(Point3D(mx, my, mz));
    this->set_fov_ratio(fov_ratio);
    this->patch_list.clear();
    this->block_list.clear();
    this->make_block_list(patch_list_file);
}

//to be enhanced to make it faster.
int SpherePET::LocateBlock(const Point3D &pt) const
{
    // find the most posible 4 blocks that contains the point.
    // where the angle between the block center and the point is smallest.
    // cosine is the largest.
    /*    float cosine_list[4] = {FLT_MAX};
    int index_list[4] = {block_list.size()};
    for(int iter = 0; iter < block_list.size();iter++){
        //compute the dot product of the block center with the point
        const ObliqueBlock& block =  block_list[iter];
        const Point3D& block_center = block.get_center();
        float angle = Point3D::ComputeScalarProduct(pt, block_center)/pt.GetNorm2()/block_center.GetNorm2();

        //when angle > 0, the point is possible to be in the block.
        if (angle >= 0 ){
            int index = 0;
            //compare with the block.
            while(index < 4){
                if(angle < cosine_list[index]){
                    //shift the index and cosine
                    for(int i = 3; i > index; i--){
                        cosine_list[i] = cosine_list[i-1];
                        index_list[i] = index_list[i-1];
                    }
                    //insert the new block.
                    cosine_list[index] = angle;
                    index_list[index] = iter;
                    break;
                }
                index++;
            }
        }
    }
*/

    // locate the
    // loop the 4 patches.
    int block_index = -1;
    //std::cout<<patch_list.size()<<std::endl;
    //std::cout<<block_list.size()<<std::endl;
    for (int iter = 0; iter < patch_list.size(); iter++)
    {
        const Patch &patch = patch_list[iter];
        if (patch.IsInPatch(pt))
        {
            block_index = iter;
            break;
        }
    }
    //if block_index is out of range, return  -1;
    return block_index < this->get_num_blocks() ? block_index : -1;
}

void SpherePET::make_block_list(const std::string &patch_list_file)
{
    std::ifstream infile(patch_list_file, std::ios_base::in);
    if (!infile.good())
    {
        std::cerr << "make_block_list(): no file " << patch_list_file << "!" << std::endl;
        std::exit(-1);
    }
    FileController fc;
    Patch patch;
    // reset the patch_list and block_list.
    this->patch_list.clear();
    this->block_list.clear();
    int num_patches = 0;
    infile >> num_patches;
    for (int iPatch = 0; iPatch < num_patches; iPatch++)
    {
        fc.ReadPatch(infile, patch);
        int num_inner_edges = patch.get_inside_face().size();
        int num_outer_edges = patch.get_inside_face().size();
        //debug
        //std::cout<<num_inner_edges <<"  "<<num_outer_edges<<std::endl;
        if (num_inner_edges == num_outer_edges && num_inner_edges > 0)
        {
            this->patch_list.push_back(patch);
            this->block_list.push_back(patch.CreateBlock(this->get_mesh_size()));
        }
    }
    infile.close();
    //debug
    //std::cout<<patch_list.size();
}

} // namespace BBSLMIRP
