#ifndef MATRIX_H
#define MATRIX_H


class Matrix
{
public:
    Matrix();
    double matrix[4][4]= {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0 ,1}
    };
    void multiplyMatrixBy(Matrix multiplierMatrix);
    void printMatrix();
};

#endif // MATRIX_H
