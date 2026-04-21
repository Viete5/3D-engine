#include "../include/math/matrix.hpp"


void swap(float &n1, float &n2) {
    float temp = n1;
    n1 = n2;
    n2 = temp;
}



Matrix4::Matrix4() {
    for (int i=0;i<dim*dim;++i) {
        if (i/dim==i%dim) m[i] = 1.0f;
        else m[i] = 0;
    }
}

Matrix4::Matrix4(float n) {
    for (int i=0;i<dim*dim;++i) {
        if (i/dim==i%dim) m[i] = n;
        else m[i] = 0;
    }
}

float& Matrix4::at(int line, int row){
    return m[line*dim+row];
}

const float& Matrix4::at(int line, int row) const {
    return m[line*dim+row];
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const {

    Matrix4 res;
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            float sum = 0.0;
            for (int k = 0; k < dim; ++k) {
                sum += at(i,k) * other.at(k,j);
            }
            res.at(i,j) = sum; 
        }
    }
    return res;
}

//матрица перемещения
Matrix4 Matrix4::get_translate(float x, float y, float z) {
    Matrix4 tr;
    tr.at(0,3) = x;
    tr.at(1,3) = y;
    tr.at(2,3) = z;
    return tr;
}

//матрица вращений относительно х
Matrix4 Matrix4::get_rotate_x(float angle) {
    Matrix4 rotX;
    float sinAn = std::sin(angle);
    float cosAn = std::cos(angle);
    rotX.at(1,1) = cosAn;
    rotX.at(1,2) = -sinAn;
    rotX.at(2,1) = sinAn;
    rotX.at(2,2) = cosAn;
    return rotX;
}

//матрица вращений относительно у
Matrix4 Matrix4::get_rotate_y(float angle) {
    Matrix4 rotY;
    float sinAn = std::sin(angle);
    float cosAn = std::cos(angle);
    rotY.at(0,0) = cosAn;
    rotY.at(0,2) = sinAn;
    rotY.at(2,0) = -sinAn;
    rotY.at(2,2) = cosAn;
    return rotY;
}

//матрица вращений относительно z
Matrix4 Matrix4::get_rotate_z(float angle) {
    Matrix4 rotZ;
    float sinAn = std::sin(angle);
    float cosAn = std::cos(angle);
    rotZ.at(0,0) = cosAn;
    rotZ.at(0,1) = -sinAn;
    rotZ.at(1,0) = sinAn;
    rotZ.at(1,1) = cosAn;
    return rotZ;
}

//растяжение/сжатие
Matrix4 Matrix4::get_scale(float x, float y, float z) {
    Matrix4 sc;
    sc.at(0,0) = x;
    sc.at(1,1) = y;
    sc.at(2,2) = z;
    return sc;
}

//матрица, преобразующая view коорды в камерные
Matrix4 Matrix4::get_perspective(float fov, float aspectRatio, float near, float far) {
    Matrix4 pers(0.0f);
    float T = std::tan(fov/2);
    float coifTranslate = (-far-near)/(far-near);
    float coifScale = ((-2)*far*near)/(far-near);
    pers.at(0,0) = 1/(T*aspectRatio);
    pers.at(1,1) = 1/T;
    pers.at(2,2) = coifTranslate;
    pers.at(2,3) = coifScale;
    pers.at(3,2) = -1.0f;
    return pers;
}


//делаем так, чтобы камера как бы смотрела на объект сверху вниз
Matrix4 Matrix4::get_look_at(const Vector& position,const Vector& target,const Vector& up) {
    
    Vector Z = position-target;
    Z = Z.normalize();

    Vector X = up.crossprod(Z);
    X = X.normalize();

    Vector Y = Z.crossprod(X);

    Matrix4 temp;

    temp.at(0,0) = X.get_x();
    temp.at(0,1) = X.get_y();
    temp.at(0,2) = X.get_z();
    temp.at(1,0) = Y.get_x();
    temp.at(1,1) = Y.get_y();
    temp.at(1,2) = Y.get_z();
    temp.at(2,0) = Z.get_x();
    temp.at(2,1) = Z.get_y();
    temp.at(2,2) = Z.get_z();

    temp.at(0,3) = (-1)*(X*position);
    temp.at(1,3) = (-1)*(Y*position);
    temp.at(2,3) = (-1)*(Z*position);

    return temp;
}


Matrix4 Matrix4::get_trans() {
    Matrix4 temp;
    int lane;
    int row;
    for (int lane=0;lane<dim;++lane) {
        for (int row=0;row<dim;++row) {
            temp.at(lane,row) = at(row,lane);
        }
    }
    return temp;
}


int Matrix4::choice_leading(Matrix4 &trM, int row, int *min) {
    int mxrow = row;
    if (trM.at(row,row)==0) {
        for (int i = row+1;i<dim;++i) {
            mxrow = trM.at(i,row)==0?mxrow:i;
        }
        if (mxrow==row) return 0;
    }
    else {
        for (int i = row+1;i<dim;++i) {
            mxrow = abs(trM.at(mxrow,row))<abs(trM.at(i,row)) ? i : mxrow;
        }
        if (mxrow == row) return 1;
    }
    for (int i = 0;i<dim;++i) swap(trM.at(row,i), trM.at(mxrow,i));
    *min+=1;
    return 1;
}


Matrix4 Matrix4::get_trian_matrix(int &min) {
    float coif;
    Matrix4 trM;
    for (int i = 0;i<(dim*dim);++i) {
        trM.m[i] = m[i];
    }

    for (int i = 0;i<dim;++i) {
        if (!choice_leading(trM,i, &min)){
            Matrix4 temp(0);
            return temp;
        }
        for (int k = i+1;k<dim;++k) {
            coif = trM.at(k,i)/trM.at(i,i);
            for (int j = 0;j<dim;++j) {
                trM.at(k,j) -= trM.at(i,j)*coif;
            }
        }
    }

    return trM;
}

float Matrix4::det(){
    float d = 1;
    int min=0;
    Matrix4 trM = get_trian_matrix(min);
    for (int i = 0;i<dim;++i) {
        d *= trM.at(i,i);
    }
    d *= std::pow((-1),min);
    return d;
}

float det3(float *m) {
    return *m * (*(m+4) * *(m+8) - *(m+5) * *(m+7)) - *(m+1) * (*(m+3) * *(m+8) - *(m+5) * *(m+6)) + *(m+2) * (*(m+3) * *(m+7) - *(m+4) * *(m+6));
}


Matrix4 Matrix4::reversed() {

    double d = det();
    if (std::abs(d) < 1e-6) return Matrix4(0.0f);//error
    
    int c;
    float *temp = new float[9];
    Matrix4 rev;
    for (int lane = 0;lane<dim;++lane) {
        for (int row = 0;row<dim;++row) {
            c = 0;
            for (int laneMain = 0;laneMain<dim;++laneMain) {
                for (int rowMain = 0;rowMain<dim;++rowMain) {
                    if ((laneMain!=lane) && (rowMain != row)) {
                        *(temp+c) = at(laneMain,rowMain);  
                        ++c;
                    }
                }
            }

            rev.at(lane,row) = det3(temp)*std::pow((-1),row+lane);

        }
    }
    delete []temp;
    for (int lane=0;lane<dim;++lane) {
        for (int row=0;row<dim;++row) {
            rev.at(lane,row) /= d;
        }
    }
    rev = rev.get_trans();
    return rev;
}

Matrix4 Matrix4::get_norm_matrix() const{
    Matrix4 temp = *this;
    temp = temp.reversed().get_trans();
    return temp;
}


Matrix4 Matrix4::get_ortho(float left, float right, float bottom, float top, float zNear, float zFar) 
{
    Matrix4 result; 
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(zFar + zNear) / (zFar - zNear);
    float R_minus_L = right - left;
    float T_minus_B = top - bottom;
    float F_minus_N = zFar - zNear;

    result.at(0, 0) = 2.0f / R_minus_L;
    result.at(0, 1) = 0.0f;
    result.at(0, 2) = 0.0f;
    result.at(0, 3) = tx; 
    

    result.at(1, 0) = 0.0f;
    result.at(1, 1) = 2.0f / T_minus_B;
    result.at(1, 2) = 0.0f;
    result.at(1, 3) = ty; 

    result.at(2, 0) = 0.0f;
    result.at(2, 1) = 0.0f;
    result.at(2, 2) = -2.0f / F_minus_N; 
    result.at(2, 3) = tz; 


    result.at(3, 0) = 0.0f;
    result.at(3, 1) = 0.0f;
    result.at(3, 2) = 0.0f;
    result.at(3, 3) = 1.0f;

    return result;
};