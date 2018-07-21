TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# the yaml-cpp dependency
INCLUDEPATH += /home/chengaoyu/tools/yaml-cpp-install/include
DEPENDPATH += $$PWD/../../../tools/yaml-cpp-install/include

LIBS += /home/chengaoyu/tools/yaml-cpp-install/lib/libyaml-cpp.a
unix:!macx: LIBS += -L$$PWD/../../../tools/yaml-cpp-install/lib/ -lyaml-cpp

# the hdf5 dependency
INCLUDEPATH += /home/chengaoyu/tools/hdf5-install/include
DEPENDPATH += $$PWD/../../../tools/hdf5-install/include
LIBS += /home/chengaoyu/tools/hdf5-install/lib/libhdf5.a
LIBS += /home/chengaoyu/tools/hdf5-install/lib/libhdf5_cpp.a
LIBS += /home/chengaoyu/tools/hdf5-install/lib/libhdf5_hl.a
LIBS += /home/chengaoyu/tools/hdf5-install/lib/libhdf5_hl_cpp.a
unix:!macx: LIBS += -L$$PWD/../../../tools/hdf5-install/lib/ -lhdf5
unix:!macx: LIBS += -L$$PWD/../../../tools/hdf5-install/lib/ -lhdf5_cpp
unix:!macx: LIBS += -L$$PWD/../../../tools/hdf5-install/lib/ -lhdf5_hl
unix:!macx: LIBS += -L$$PWD/../../../tools/hdf5-install/lib/ -lhdf5_hl_cpp
unix:!macx: LIBS += -lz

# cuda dependency
SYSTEM_NAME = linux         #
SYSTEM_TYPE = 64            #
CUDA_ARCH = sm_61           #
CUDA_SDK = "/usr/local/cuda-8.0/"   # cudaSDK path
CUDA_DIR = "/usr/local/cuda-8.0/"   # CUDA tookit path
NVCC_OPTIONS = --use_fast_math

INCLUDEPATH += $$CUDA_DIR/include  # include paths
QMAKE_LIBDIR += $$CUDA_DIR/lib64/  # library directories
CUDA_LIBS =  cudart cufft
# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')

#LIBS += $$join(CUDA_LIBS,'.so ', '', '.so')
# Configuration of the Cuda compiler
CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -O3 -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}

SUBDIRS += \
    BBSLMIRP_QT.pro

HEADERS += \
    Algorithm/mapper.h \
    Algorithm/mlem.h \
    Algorithm/patch_mapper.h \
    Algorithm/pet_translator.h \
    Algorithm/ring_mapper.h \
    Algorithm/translator.h \
    FileIO/datastruct.h \
    FileIO/filestruct.h \
    Geometry/block.h \
    Geometry/block_interface.h \
    Geometry/geometry.h \
    Geometry/grid.h \
    Geometry/gridsize.h \
    Geometry/obliqueblock.h \
    Geometry/patch.h \
    Geometry/point.h \
    Geometry/ray.h \
    Geometry/raycast.h \
    Geometry/rotation.h \
    Geometry/sphericalpoint.h \
    PETSystem/map_task.h \
    PETSystem/pet_factory.h \
    PETSystem/petapplication.h \
    PETSystem/petscanner.h \
    PETSystem/reconstruction_task.h \
    PETSystem/ringpet.h \
    PETSystem/ringpet_factory.h \
    PETSystem/spherepet.h \
    PETSystem/spherepet_factory.h \
    PETSystem/task.h \
    ImageProcessing/filter.h \
    Algorithm/projector.h \
    Algorithm/siddon_projector.h

SOURCES += \
    Algorithm/mlem.cc \
    Algorithm/patch_mapper.cc \
    Algorithm/pet_translator.cc \
    Algorithm/ring_mapper.cc \
    FileIO/datastruct.cc \
    FileIO/filestruct.cc \
    Geometry/grid.cc \
    Geometry/gridsize.cc \
    Geometry/obliqueblock.cc \
    Geometry/patch.cc \
    Geometry/point.cc \
    Geometry/ray.cc \
    Geometry/raycast.cc \
    Geometry/rotation.cc \
    Geometry/sphericalpoint.cc \
    PETSystem/map_task.cc \
    PETSystem/petapplication.cc \
    PETSystem/reconstruction_task.cc \
    PETSystem/ringpet.cc \
    PETSystem/ringpet_factory.cc \
    PETSystem/spherepet.cc \
    PETSystem/spherepet_factory.cc \
    testmain.cc \
    ImageProcessing/filter.cc \
    Algorithm/siddon_projector.cc

