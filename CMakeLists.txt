# August 3rd
# Chen Gaoyu, chengaoyu2013@gmail.com
# CMAKE compiling the BBSLMIRP reconstruction software


# --------
# Checking the CMAKE version 

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# --------
# Set the build type
set( CMAKE_BUILD_TYPE "Debug -O0 -Wall -g -ggdb" )
set( CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug Release"   
)
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "ENV{CXXFLAGS} -O3 -Wall")



#---------
# Defining the project
project( bbslmirp )
set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-modules
  ${CMAKE_MODULE_PATH}
)


# add c++11 surpport
set(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(LIBS ${Boost_LIBRARIES} ${LIBS})
  link_directories(${Boost_LIBRARY_DIRS})
  message("Boost is Found!")
endif()

find_package(Yaml-cpp)
if(Yaml_Found)
  include_directories(${YAMLCPP_INCLUDE_DIRS})
  set(LIBS ${YAMlCPP_LIBRARIES} ${LIBS})
  # link_directories(${YAMLCPP_LIBRARY_DIRS})
  message("Yaml-cpp is Found !")
endif()


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/include/FileIO
  ${PROJECT_SOURCE_DIR}/include/Geometry
  ${PROJECT_SOURCE_DIR}/include/ImageProcessing
  ${PROJECT_SOURCE_DIR}/include/PETSystem
  ${PROJECT_SOURCE_DIR}/include/Algorithm
)

#---------
# Locate sources and headers for LMRec
file( GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc )
file( GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h )

add_executable( bbs-recon main.cc ${sources} ${headers}) 
target_link_libraries(bbs-recon ${LIBS})

# -------
# Move the executables to the install path
install( TARGETS
  bbs-recon
  DESTINATION
  bin
)


# #========Build and link:=========================#
# add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc ${SOURCEFILES} ${HEADERFILES})
# target_link_libraries(${PROJECT_NAME} ${LIBS})



