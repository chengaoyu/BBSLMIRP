#ifndef MLEM_H
#define MLEM_H
/*
  @brief:  MLEM is the standard iterative reconstruction method for PET system.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/31
*/
#include<string>
namespace BBSLMIRP {

class PETScanner;
class Grid3D;
class Projector;
class LMEvent;

class MLEM{
public:
    MLEM();
    virtual ~MLEM();
    virtual void Reconstruct();
    virtual void Initialize(const PETScanner&, const Projector& projector, Grid3D& map, int iteration_start_index, int num_iteration,
                    std::string start_image_name, std::string in_file_name, std::string out_image_name, int output_interval, bool abf_flag);

    virtual void EM(Grid3D& image_now, Grid3D& image_next, float comfac, const std::string& event_file_name) const;
    virtual float EM_Projection(Grid3D& image_now, Grid3D& image_next, LMEvent& lmevent) const;
    virtual void ABF(Grid3D& image) const;
    virtual Grid3D& get_map(){return *map;}
private:
    const PETScanner* pet; // pet scanner
    const Projector*   projector;
    Grid3D* map;     // normalization map

    int   iteration_start_index;  //start iteration from a exist result
    int   num_iteration;         // number of iterations
    std::string start_image_name;// the image used as the start point of iteration.
    std::string in_file_name;    // the event file to be reconstucted
    std::string out_image_name;  // reconstruction result.
    int   output_interval;       // how many iterations per output image.
    bool  abf_flag;              // if use the after backprojection filtering.
};
}


#endif // MLEM_H
