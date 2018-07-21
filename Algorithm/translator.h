#ifndef TRANSLATOR_H
#define TRANSLATOR_H

/*
  @brief:  This class translater the simulated Events into located events.
           (gamma position to crystal postition)
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/31
*/
#include<string>
namespace BBSLMIRP {
class Translator{
public:
    virtual void TranslateEvents(const std::string& input_file_name, const std::string& output_image_name) const= 0;
};
}

#endif // TRANSLATOR_H
