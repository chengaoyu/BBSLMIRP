#ifndef PETSCANNER_H
#define PETSCANNER_H

/*
  @brief:  abstract class 'PETScanner' to give the interface of a PET detector.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/23
*/

#include <string>
//#include "FileIO/datastruct.h"

namespace BBSLMIRP
{
class PETConfigureFile;
class VerticalBlock;
class Block;
class Point3D;
class Patch;
class PETScanner
{
public:
  virtual ~PETScanner() {}
  // Initialize the scanner with a configure file.
  virtual void Initialize(const std::string &scanner_file) = 0;
  // Get the number of blocks in the PET scanner.
  virtual int get_num_blocks() const = 0;
  virtual float get_inner_radius() const = 0;
  virtual float get_outer_radius() const = 0;
  // Get the block in the block list by index.
  virtual int LocateBlock(const Point3D &pt) const = 0;
  virtual const Block &get_block(int block_index) const = 0;
  virtual const Patch &get_patch(int patch_index) const = 0;
  virtual void DescribeSelf() const = 0;
};
} // namespace BBSLMIRP
#endif //PET_H
