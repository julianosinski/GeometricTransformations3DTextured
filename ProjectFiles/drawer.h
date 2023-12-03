#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QPainter>
#include <iostream>
#include "matrix.h"


using namespace std;

class Drawer : public QWidget
{
    Q_OBJECT
public:
    explicit Drawer(QWidget *parent = nullptr);

private:
    QImage im, imInput;
    const int im_width = 500;
    const int im_height = 500;
    const int im_depth = 500;
    double f = 500;
    double r = 200;

    double cameraPosition[3] = { 0,0, -f };

    double cube[8][3] = {
        {-r, -r, -r},
        { r, -r, -r},
        { r,  r, -r},
        {-r,  r, -r},
        {-r, -r,  r},
        { r, -r,  r},
        { r,  r,  r},
        {-r,  r,  r}
    };
    int cube3D[8][3];
    int cube2D[8][2];

    Matrix translateMatrix;
    Matrix scaleMatrix;
    Matrix rotateXMatrix,rotateYMatrix,rotateZMatrix;
    Matrix shearMatrix;

    void paintEvent(QPaintEvent *);
    bool tryLoadImage();
    void transform(Matrix matrix);
    void executeOperations();
    void drawPixel(QPoint p, QColor color);
    void drawLine(QPoint start, QPoint end, QColor color);
    void drawCube();
    void normalVector(double wall[4][3], double normal[3]);
    void textureWalls(int walls[6][4],int wallIndex);
    void mapTriangle(int triangle[3][2]);
public slots:
    void setDX(int dx);
    void setDY(int dy);
    void setDZ(int dz);
    void setSX(int sx_int);
    void setSY(int sy_int);
    void setSZ(int sz_int);
    void setAngleX(int angleX);
    void setAngleY(int angleY);
    void setAngleZ(int angleZ);
    void setSHX(int shx_int);
    void setSHY(int shy_int);
signals:

};

#endif // DRAWER_H
