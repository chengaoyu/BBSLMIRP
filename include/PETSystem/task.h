#ifndef TASK_H
#define TASK_H

/*
  @brief:  the
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/

#include<string>
#include <fstream>
namespace BBSLMIRP {
class PETFactory;
class PETScanner;
class Task{
public:
    //Task();
    virtual ~Task(){}
    virtual void Initialize( std::ifstream& task_stream) = 0;
    virtual void Run(const PETFactory&, const PETScanner&) const = 0;
private:
};



}
#endif // TASK_H
