#ifndef PET_FACTORY_H
#define PET_FACTORY_H

#include"pet_translator.h"

namespace BBSLMIRP{

class PETScanner;
class Translator;
class Mapper;
class Projector;
class Grid3D;
class PETFactory{

public:
    //PETFactory();
    virtual ~PETFactory(){}
    virtual PETScanner* MakeScanner(const std::string& ) const = 0 ;
    virtual Translator* MakeTranslator(const PETScanner& pet)const {return new PETTranslator(pet);}
    virtual Mapper* MakeMapper(const PETScanner&, const Projector&)const = 0;
};


}
#endif //PETFACTORY_H
