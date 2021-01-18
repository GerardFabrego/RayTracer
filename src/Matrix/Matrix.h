#pragma once

#include "Tuple.h"

class Matrix {
public:
    float* array;
    int dimension;
    
    //Constructors 
    Matrix(
        float x1, float x2, float x3, float x4,
        float y1, float y2, float y3, float y4, 
        float z1, float z2, float z3, float z4,
        float w1, float w2, float w3, float w4
    );

    Matrix(
        float x1, float x2, float x3,
        float y1, float y2, float y3,
        float z1, float z2, float z3
    );

    Matrix(
        float x1, float x2,
        float y1, float y2
    );

    //Operator overloads
    float& operator() (int row, int col); 
    float  operator() (int row, int col) const;

    bool operator== (Matrix const& other);
    bool operator!= (Matrix const& other);

    Matrix operator* (Matrix const& other);
    Tuple operator* (Tuple const& tuple);
};


Matrix transpose(Matrix matrix);