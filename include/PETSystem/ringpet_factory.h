#ifndef RINGPET_FACTORY_H
#define RINGPET_FACTORY_H

#include "pet_factory.h"
namespace BBSLMIRP{
class PETScanner;
class Translator;
class RingPETFactory : public PETFactory{
public:
    RingPETFactory();
    virtual ~RingPETFactory();
    virtual PETScanner* MakeScanner(const std::string& pet_configure_file) const;
    //virtual Translator* MakeTranslator(const PETScanner&) const;
    virtual Mapper* MakeMapper(const PETScanner &, const Projector &) const;
};

}

#endif //RINGPET_FACTORY_H
