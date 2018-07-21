/*
  @brief:  the definition of
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11
*/
#include"spherepet_factory.h"
#include"spherepet.h"
#include"Algorithm/pet_translator.h"
#include"Algorithm/patch_mapper.h"

namespace BBSLMIRP {
SpherePETFactory::SpherePETFactory(){

}
SpherePETFactory::~SpherePETFactory(){

}
PETScanner* SpherePETFactory::MakeScanner(const std::string &configure_file) const{
    return new SpherePET(configure_file);
}

//Translator* SpherePETFactory::MakeTranslator(const PETScanner &pet) const{
//    return new PETTranslator(pet);
//}

Mapper* SpherePETFactory::MakeMapper(const PETScanner & pet, const Projector & projector) const{
    return new PatchMapper(pet,projector);
}

}
