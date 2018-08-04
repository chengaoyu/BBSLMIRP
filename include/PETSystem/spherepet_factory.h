#ifndef SPHEREPET_FACTORY_H
#define SPHEREPET_FACTORY_H
/*
  @brief:
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/

#include "pet_factory.h"

namespace BBSLMIRP{
class PETScanner;
class SpherePETFactory : public PETFactory{
public:
    SpherePETFactory();
    virtual ~SpherePETFactory();
    virtual PETScanner* MakeScanner(const std::string& pet_configure_file) const;
    //virtual Translator* MakeTranslator(const PETScanner &) const;
    virtual Mapper* MakeMapper(const PETScanner &, const Projector &) const;
};

}
#endif // SPHEREPET_FACTORY_H
