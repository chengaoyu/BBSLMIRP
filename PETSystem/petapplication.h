#ifndef PETAPPLICATION_H
#define PETAPPLICATION_H
/*
  @brief:  This application class is the entry of a PET reconstruction,
           and also the controller of the whole PET reconstruction system.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include<string>
#include<vector>
namespace BBSLMIRP {
class PETScanner;
class Task;
class PETFactory;
class PETApplication
{
public:
    PETApplication(const std::string& pet_configure_file, const std::string& task_file);
    virtual ~PETApplication();

    virtual void BuildFactory(const std::string& pet_configure_file);
    virtual void BuildTaskList(const std::string& task_file);

    virtual const PETFactory& get_factory()const {return * factory;}
    virtual const Task& get_task(int index)const {return *task_list[index];}
    virtual const PETScanner& get_scanner()const {return *scanner;}

    virtual void Run() const;
private:
    PETFactory* factory;
    std::vector<Task*> task_list;
    PETScanner* scanner;
};
}



#endif // PETAPPLICATION_H
