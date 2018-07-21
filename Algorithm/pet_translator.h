#ifndef PET_TRANSLATOR_H
#define PET_TRANSLATOR_H

/*
  @brief:  a concrete class of translator to locate the gamma postion in the crystal in ring PET.
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/29
*/

#include "translator.h"
namespace BBSLMIRP {
class PETScanner;
class PETTranslator:public Translator{
public:
    PETTranslator(const PETScanner&);
    virtual ~PETTranslator();
    virtual void TranslateEvents(const std::string &input_file_name, const std::string & output_image_name) const;
private:
    const PETScanner* pet;
};
}

#endif // PET_TRANSLATOR_H
