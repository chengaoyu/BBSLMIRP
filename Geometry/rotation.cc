/*
 * @brief:  this file defines the rotation of a 3D point
 * @auther: chengaoyu2013@gmail.com
 * @date:   2017/05/25
*/

#include "rotation.h"

// the form of rotation_matrix
// [0,0][0,1][0,2]
// [1,0][1,1][1,2]
// [2,0][2,1][2,2]
namespace BBSLMIRP{

Rotation::Rotation(){
    Reset();
}
Rotation::~Rotation(){

}
void Rotation::set_rotation_matrix(float radian , Axis dimension){
    Reset();
    if(dimension == Axis::x){
        rotation_matrix[0][0] = 1;
        rotation_matrix[1][1] = std::cos(radian);
        rotation_matrix[2][2] = std::cos(radian);
        rotation_matrix[1][2] = std::sin(radian);
        rotation_matrix[2][1] = -std::sin(radian);

        // [ 1 ][ 0 ][ 0 ]
        // [ 0 ][cos][sin]
        // [ 0 ][-sin][cos]
    }
    else if(dimension ==Axis::y){
        rotation_matrix[1][1] = 1;
        rotation_matrix[0][0] = std::cos(radian);
        rotation_matrix[2][2] = std::cos(radian);
        rotation_matrix[0][2] = -std::sin(radian);
        rotation_matrix[2][0] = std::sin(radian);

        // [cos][ 0 ][-sin]
        // [ 0 ][ 1 ][ 0 ]
        // [sin][ 0 ][cos]
    }
    else if(dimension == Axis::z){
        rotation_matrix[2][2] = 1;
        rotation_matrix[0][0] = std::cos(radian);
        rotation_matrix[1][1] = std::cos(radian);
        rotation_matrix[0][1] = std::sin(radian);
        rotation_matrix[1][0] = -std::sin(radian);

        // [cos][sin][ 0 ]
        // [-sin][cos][ 0 ]
        // [ 0 ][ 0 ][ 1 ]
    }
    else{
        // TO BE DELETED WHEN THE DEBUGGING IS PASSED!
        std::cout<<"Rotation::setMatrix: invalid axis! "<<std::endl;
        Reset();
    }
}

Point3D Rotation::Rotate(const Point3D &pt) const{
    Point3D rpt;
    rpt.set_px(pt.get_px()*rotation_matrix[0][0]+pt.get_py()*rotation_matrix[1][0]+pt.get_pz()*rotation_matrix[2][0]);
    rpt.set_py(pt.get_px()*rotation_matrix[0][1]+pt.get_py()*rotation_matrix[1][1]+pt.get_pz()*rotation_matrix[2][1]);
    rpt.set_pz(pt.get_px()*rotation_matrix[0][2]+pt.get_py()*rotation_matrix[1][2]+pt.get_pz()*rotation_matrix[2][2]);
    return rpt;
}

void Rotation::Reset(){
    for(size_t i = 0; i < 3;i++){
        for(size_t j = 0; j < 3;j++){
            rotation_matrix[i][j] = 0;
        }
    }
}

}

