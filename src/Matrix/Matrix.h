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

    //constructs empty matrix with desired dimension. e.g 4 -> 4x4
    Matrix(int dimension); 

    Matrix();
    ~Matrix();
    
    //Operator overloads
    float& operator() (int row, int col); 
    float&  operator() (int row, int col) const;

    bool operator== (Matrix const& other) const;
    bool operator!= (Matrix const& other) const;
    void operator=(Matrix const& other); //copy constructor

    Matrix operator* (Matrix const& other) const;
    Tuple operator* (Tuple const& tuple) const;
    Matrix operator* (float const& scalar) const;

    static Matrix Identity(int const& dimension); //returns a identity matrix with specified dimension

    void print();
};

Matrix transpose(Matrix const& matrix);

float determinant(Matrix const& matrix);

Matrix subMatrix(Matrix const& matrix, int row, int column);

float minor(Matrix const& matrix, int row, int column);

float cofactor(Matrix const& matrix, int row, int column);

bool isInvertible(Matrix const& matrix);

Matrix inverse(Matrix const& matrix);