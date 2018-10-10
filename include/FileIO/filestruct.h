#ifndef FILESTRUCT_H
#define FILESTRUCT_H

/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/31
*/

#include <string>
#include <fstream>
#include "datastruct.h"
#include "point.h"

namespace BBSLMIRP {

//struct LMEvent;
//struct CoinEvent;
class Grid3D;
//class Event;
class Patch;
typedef std::vector<CoinEvent> CoinEventList;

class FileController{
public:
    FileController();
    virtual ~FileController();

    virtual Point3D ReadPoint(std::ifstream& inStream)const;
    virtual void SavePoint(std::ofstream& outStream,const Point3D& pt)const;
    // image file control
    virtual bool ReadImage(const std::string& image_name, Grid3D& image)const;
    virtual bool ReadImageBin(const std::string& image_name, Grid3D& image)const;
    virtual bool SaveImage(const std::string& image_name, const Grid3D& image)const;
    virtual bool SaveImageBin(const std::string image_name, const Grid3D& image)const;
    // data file control(original event file, translated event file)
    virtual bool ReadEvent(std::ifstream& inStream, Event& evt)const;
    virtual bool SaveEvent(std::ofstream& outStream, Event& evt)const;

    // read the events file (binary version)
    virtual bool LoadEvents(const std::string& file_name, int pos, int list_size, CoinEventList& evtlist);
    virtual bool SaveEvents(const std::string& file_name, CoinEventList& evtlist);

    // data file control(list-mode data file, projected)
    virtual bool ReadListModeEvent(std::ifstream &inStream, LMEvent& lmevent)const;
    virtual bool SaveListModeEvent(std::ofstream &outStream, LMEvent& lmevent)const;
    // data file header
    virtual bool SaveHeader(const std::string& file_name, int& num_events) const;
    virtual bool ReadHeader(const std::string& file_name, int& num_events) const;
    virtual bool ReadPatch(std::ifstream& inStream, Patch& patch) const;

    virtual void ReadPETConfigure(const std::string& file_name,std::string& pet_category) const;
    virtual void ReadTaskFile(const std::string& file_name) const;
};
}


#endif // FILESTRUCT_H
