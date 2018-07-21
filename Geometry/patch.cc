/*
  @brief:  Patch definition
  @auther: chengaoyu2013@gmail.com
  @date:   2017/11/03
*/

#include "patch.h"
#include<float.h>
#include"point.h"
#include "obliqueblock.h"
namespace BBSLMIRP {
Patch::Patch(){

}
Patch::~Patch(){

}

void Patch::Reset(){
    theta = 0;
    phi = 0;
    normal = Point3D(0,0,0);
    this->inside_face.clear();
    this->outside_face.clear();
    this->rotated_inside_face.clear();
    this->rotated_outside_face.clear();
}

// if valid faces, compute the normal.
bool Patch::Initialize(const Face &face1, const Face &face2){
    this->Reset();
    if(face1.size() >=3&&face1.size() == face2.size()){ //at least 3 points to form a face.
        for (int iter = 0; iter < (int)face1.size();++iter){
            this->inside_face.push_back(face1[iter]);
            this->outside_face.push_back(face2[iter]);
        }
        ComputeNormal();
        this->RotateToX();
        return true;
    }
    return false;
}

// the normal is defined from inside face to  outside face.
// Assume the face is valid.
void Patch::ComputeNormal(){
    Point3D inside_center = SumVector(inside_face)/inside_face.size();
    Point3D outside_center = SumVector(outside_face)/outside_face.size();
    Point3D center_diff = outside_center- inside_center;

    Point3D pt0  = outside_face[0];
    Point3D pt1  = outside_face[1];
    Point3D pt2  = outside_face[2];
    Point3D v0 = pt1-pt0;
    Point3D v1 = pt2-pt0;
    Point3D face_normal =Point3D::ComputeCrossProduct(v0,v1);
    Point3D face_ = face_normal/(face_normal.GetNorm2());

    if (Point3D::ComputeScalarProduct(face_,center_diff)>0)
        this->normal = face_;
    else
        this->normal = Point3D(0,0,0)- face_;
    SphericalPoint3D spt =  this->normal.CartesianToSpherical();
    this->theta = spt.get_theta();
    this->phi = spt.get_phi();

}

//create a oblique block to
ObliqueBlock Patch::CreateBlock(const Point3D &mesh_size) const{
    // find the minimum block;
    Point3D bottom_bound;
    Point3D top_bound;
    this->FindBlockSize(rotated_inside_face,rotated_outside_face,bottom_bound,top_bound);

    Point3D  geometry_size = (top_bound-bottom_bound)/mesh_size;
    GridSize num_grid(geometry_size.get_px()+1,geometry_size.get_py()+1,geometry_size.get_pz()+1);
    geometry_size = Point3D::GridSize_to_Point3D(num_grid)*mesh_size;
    Point3D  center = bottom_bound + geometry_size/2;

    ObliqueBlock block;
    block.Initialize(num_grid, geometry_size,center,phi,theta);
    return block;
}


Point3D Patch::SumVector(const Face &face){
    Point3D sum(0,0,0);
    for (int iter = 0; iter <(int)face.size(); iter++){
        sum = sum + face[iter];
    }
    return sum;
}

void Patch::BubbleSort(std::vector<angle_pair> &angle_list){
    for(int i = 0; i< (int)angle_list.size();i++){
        for(int j = 0 ; j<(int)angle_list.size()-1-i;j++){
            if(angle_list[j].angle > angle_list[j+1].angle){
                angle_pair temp;
                temp.angle = angle_list[j].angle;
                temp.index = angle_list[j].index;
                angle_list[j] = angle_list[j+1];
                angle_list[j+1] = temp;
            }
        }
    }
}

void Patch::RotateToX(){
    rotate_z.set_rotation_matrix(2*kPi-this->phi, Axis::z);
    rotate_y.set_rotation_matrix(kPi/2-this->theta,Axis::y);
    Face face1,face2;
    for (int iter = 0 ; iter<inside_face.size(); iter++){
        //rotate the two faces.(first z-axis, then y-axis,can not exchange)
        Point3D pt = inside_face[iter];
        pt = rotate_z.Rotate(pt);
        pt = rotate_y.Rotate(pt);
        face1.push_back(pt);

        pt = outside_face[iter];
        pt = rotate_z.Rotate(pt);
        pt = rotate_y.Rotate(pt);
        face2.push_back(pt);
    }

    // sort the points in anticlockwise order
    std::vector<angle_pair> angle_list;
    //use face1 to sort the points
    Point3D face_center = SumVector(face1)/face1.size();

    //compute the angle;
    for(int i = 0 ;i <face1.size();i++){
        Point3D vec = face1[i] - face_center;
        float phi = std::atan2(vec.get_pz(),vec.get_py());
        phi = phi >0? phi:phi+2*kPi;
        angle_pair _pair;
        _pair.angle = phi;
        _pair.index =i;
        angle_list.push_back(_pair);
    }
    //sort the points with bubble sort method.
    BubbleSort(angle_list);
    for(int i = 0 ;i <face1.size();i++){
        int index = angle_list[i].index;
        rotated_inside_face.push_back(face1[index]);
        rotated_outside_face.push_back(face2[index]);
    }


}

void Patch::FindBlockSize(const Face &in_face, const Face &out_face, Point3D &bottom_bound, Point3D &top_bound) const{
    //compute the minimum block
    //the faces are now perpendicular to x axis, to find a minimum block, compute the max_z, min_z, max_y, min_y first.
    float max_z = -FLT_MAX;
    float min_z = FLT_MAX;
    float max_y = -FLT_MAX;
    float min_y = FLT_MAX;
    Point3D in_pt;
    Point3D out_pt;
    for(int iter = 0; iter < in_face.size();iter++){
        in_pt = in_face[iter];
        out_pt = out_face[iter];
        float in_y = in_pt.get_py();
        float in_z = in_pt.get_pz();
        float out_y = out_pt.get_py();
        float out_z = out_pt.get_pz();

        if(in_y > max_y)
            max_y = in_y;
        if(in_y < min_y)
            min_y = in_y;
        if(in_z > max_z)
            max_z = in_z;
        if(in_z < min_z)
            min_z = in_z;

        if(out_y > max_y)
            max_y = out_y;
        if(out_y < min_y)
            min_y = out_y;
        if(out_z > max_z)
            max_z = out_z;
        if(out_z < min_z)
            min_z = out_z;

    }
    bottom_bound.SetPoint(in_pt.get_px(), min_y, min_z);
    top_bound.SetPoint(out_pt.get_px(), max_y, max_z);
}

/*
 * check if the point is in a patch, the rotated faces are used.
*/
bool Patch::IsInPatch(const Point3D &pt) const{
    //rotate the point
    Point3D rpt;
    rpt = rotate_z.Rotate(pt);
    rpt = rotate_y.Rotate(rpt);
    const Face& in_face = this->get_rotated_inside_face();
    const Face& out_face = this->get_rotated_outside_face();

    float x0 = in_face[0].get_px();
    float x1 = out_face[0].get_px();
    float x = rpt.get_px();
    if(x < x0 || x > x1){
        return false;
    }
    //compute a parallel face which contains the point.
    Face between_face;
    int num_points = in_face.size();
    for(int i = 0 ;i < num_points; i++){
        Point3D pt1 = in_face[i];
        Point3D pt2 = out_face[i];
        Point3D pt3 = pt1+(pt2-pt1)/(x1-x0)*(x-x0);
        between_face.push_back(pt3);
    }
    return IsInPolygon(between_face,rpt);

}

bool Patch::IsInPolygon(const Face &ordered_face, const Point3D &pt){
    int num_points = ordered_face.size();


    if(cross(ordered_face[0],pt,ordered_face[1])>0) return false;
    if(cross(ordered_face[0],pt,ordered_face[num_points-1])<0) return false;
    int i = 2,j = num_points-1;
    int line = -1;
    while(i<=j){
        int mid = (i+j)>>1;
        if(cross(ordered_face[0],pt,ordered_face[mid])>0){
            line = mid;
            j =mid -1;
        }
        else
            i = mid+1;
    }
    if(line == -1){
//        std::cout<<"error here!"<<std::endl;
        return false;
    }
    return cross(ordered_face[line-1],pt,ordered_face[line])<0;
}

double Patch::cross(const Point3D &p0, const Point3D &p1, const Point3D &p2) {
    return (p1.get_py()-p0.get_py())*(p2.get_pz()-p0.get_pz()) - (p2.get_py()-p0.get_py())*(p1.get_pz()-p0.get_pz());
}

// not used at present.
bool Patch::IsValidFace(const Face &face){
    Face face_vector;
    Point3D pt1 = face[0];
    for (int iter = 1; iter <face.size();iter++){
        Point3D pt2 = face[iter];
        face_vector.push_back(pt2-pt1);
    }
    //Point3D cross_product = Point3D::ComputeCrossProduct(face_vector[0],face_vector[1]);
    Face cross_vector;
    for (int iter = 1; iter <face_vector.size();iter++){
        cross_vector.push_back(Point3D::ComputeCrossProduct(face_vector[iter-1],face_vector[iter]));
    }
    for(int iter = 1;iter<cross_vector.size();iter++){
        Point3D ratio = Point3D(cross_vector[iter-1])/cross_vector[iter];
    }

    //Point3D sum_vector = SumVector(face_vector);

    //float angle =  Point3D::ComputeScalarProduct(sum_vector,cross_product);
    //return (std::abs(angle)<0.1);
    return true;
}

}
















