#include "matrix.h"
#include <iostream>
using namespace std;

Matrix::Matrix()
{

}
void Matrix::multiplyMatrixBy(Matrix multiplierMatrix)
{

    double result[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                result[i][j] += matrix[i][k] * multiplierMatrix.matrix[k][j];
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrix[i][j] = result[i][j];
        }
    }
}

void Matrix::printMatrix(){
    cout<<matrix[0][0]<<matrix[0][1]<<matrix[0][2]<<endl;
    cout<<matrix[1][0]<<matrix[1][1]<<matrix[2][2]<<endl;
    cout<<matrix[2][0]<<matrix[2][1]<<matrix[2][2]<<endl;
}
