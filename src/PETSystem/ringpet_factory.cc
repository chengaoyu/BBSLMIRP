/*
  @brief:  RingPETFactory is a concrete class of the PETFactory
  @auther: chengaoyu2013@gmail.com
  @date:   2017/10/19
*/

#include"ringpet_factory.h"
#include"ringpet.h"
#include"pet_translator.h"
#include"ring_mapper.h"
namespace BBSLMIRP {
RingPETFactory::RingPETFactory(){

}
RingPETFactory::~RingPETFactory(){

}
PETScanner* RingPETFactory::MakeScanner(const std::string &configure_file) const{
    return new RingPET(configure_file);
}

//Translator* RingPETFactory::MakeTranslator(const PETScanner & pet) const{
//    return new PETTranslator(pet);
//}

Mapper* RingPETFactory::MakeMapper(const PETScanner & pet, const Projector & projector) const{
    return new RingMapper(pet,projector);
}
}
