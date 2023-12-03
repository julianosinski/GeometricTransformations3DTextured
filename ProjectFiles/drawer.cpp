#include "drawer.h"
#include <QFileDialog>
Drawer::Drawer(QWidget *parent)
    : QWidget{parent}
{
    translateMatrix.matrix[2][3] = f;
    im = QImage(im_width, im_height, QImage::Format_RGB32);
    im.fill(Qt::white);
    if(tryLoadImage() == false){
        return;
    }
    executeOperations();
    update();
}

void Drawer::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), Qt::white);
    p.drawImage(0, 0, im);
}

bool Drawer::tryLoadImage()//app tested with jpg image 500x500
{
        QString imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"));
        imInput.load(imagePath);
        if(imInput.isNull()){
            cout<<"couldnt find an image"<<endl;
            qDebug() << imagePath;
            return false;
        }
        cout<<"tryLoadImage"<<endl;

    return true;
}

void Drawer::transform(Matrix matrix)
{
    for (int i = 0; i < 8; i++)
    {
        double x = matrix.matrix[0][0] * cube[i][0] + matrix.matrix[0][1] * cube[i][1] + matrix.matrix[0][2] * cube[i][2] + matrix.matrix[0][3];
        double y = matrix.matrix[1][0] * cube[i][0] + matrix.matrix[1][1] * cube[i][1] + matrix.matrix[1][2] * cube[i][2] + matrix.matrix[1][3];
        double z = matrix.matrix[2][0] * cube[i][0] + matrix.matrix[2][1] * cube[i][1] + matrix.matrix[2][2] * cube[i][2] + matrix.matrix[2][3];

        cube2D[i][0] = static_cast<int>(x * f / (z + f)) + im_width/2;
        cube2D[i][1] = static_cast<int>(y * f / (z + f)) + im_height/2;

        cube3D[i][0] = static_cast<int>(x);
        cube3D[i][1] = static_cast<int>(y);
        cube3D[i][2] = static_cast<int>(z);
    }
}

void Drawer::executeOperations()
{
    Matrix resultMatrix;

    // Wykonanie operacji przesunięcia, obrotu, skalowania i pochylenia
    resultMatrix.multiplyMatrixBy(translateMatrix);
    resultMatrix.multiplyMatrixBy(rotateXMatrix);
    resultMatrix.multiplyMatrixBy(rotateYMatrix);
    resultMatrix.multiplyMatrixBy(rotateZMatrix);
    resultMatrix.multiplyMatrixBy(scaleMatrix);
    resultMatrix.multiplyMatrixBy(shearMatrix);

    //multiplyMatrices(matrix, matrixShear);

    transform(resultMatrix);
    im.fill(Qt::white);
    drawCube();
    update();
}


void Drawer::drawPixel(QPoint p, QColor color)
{
    if (p.x() < im.width() && p.x() >= 0 && p.y() < im.height() && p.y() >= 0)
    {
        uchar *pix = im.scanLine(p.y()) + 4 * p.x();
        pix[0] = color.blue();
        pix[1] = color.green();
        pix[2] = color.red();
    }
}

void Drawer::drawLine(QPoint start, QPoint end, QColor color)
{
    int x, y;
    if ((start.x() == end.x()) && (start.y() == end.y()))
    {
        drawPixel(start, color);
        return;
    }
    else if (abs(end.y() - start.y()) <= abs(end.x() - start.x()))
    {
        if (start.x() > end.x())
        {
            QPoint temp = start;
            start = end;
            end = temp;
        }
        for (x = start.x(); x <= end.x(); x++)
        {
            y = start.y() + (x - start.x()) * (end.y() - start.y()) / (end.x() - start.x());
            drawPixel(QPoint(x, y), color);
        }
    }
    else
    {
        if (start.y() > end.y())
        {
            QPoint temp = start;
            start = end;
            end = temp;
        }
        for (y = start.y(); y <= end.y(); y++)
        {
            x = start.x() + (y - start.y()) * (end.x() - start.x()) / (end.y() - start.y());
            drawPixel(QPoint(x, y), color);
        }
    }
}

void Drawer::normalVector(double wall[4][3], double normal[3]) {
    double v1[3] = { wall[1][0] - wall[0][0], wall[1][1] - wall[0][1], wall[1][2] - wall[0][2] };
    double v2[3] = { wall[2][0] - wall[0][0], wall[2][1] - wall[0][1], wall[2][2] - wall[0][2] };

    normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
}


void Drawer::drawCube()
{
    int walls[6][4] = {
        {0, 1, 5, 4},
        {7, 6, 2, 3},
        {6, 5, 1, 2},
        {7, 3, 0, 4},
        {7, 4, 5, 6},
        {3, 2, 1, 0}
    };

    for (int i = 0; i < 6; i++) {
        double wall[4][3];
        for (int n = 0; n < 4; n++) {
            wall[n][0] = cube3D[walls[i][n]][0];
            wall[n][1] = cube3D[walls[i][n]][1];
            wall[n][2] = cube3D[walls[i][n]][2];
        }

        double normal[3];
        normalVector(wall, normal);

        double viewVector[3] = { cameraPosition[0] - wall[0][0], cameraPosition[1] - wall[0][1], cameraPosition[2] - wall[0][2] };

        double dotProduct = normal[0] * viewVector[0] + normal[1] * viewVector[1] + normal[2] * viewVector[2];
        if (dotProduct > 0) {
            textureWalls(walls, i);

            for (int n = 0; n < 4; n++) {
                drawLine({cube2D[walls[i][n]][0], cube2D[walls[i][n]][1]}, {cube2D[walls[i][(n+1)%4]][0], cube2D[walls[i][(n+1)%4]][1]}, Qt::black);
            }
        }
    }
}

void Drawer::textureWalls(int walls[6][4],int wallIndex){
    for (int n = 0; n < 4; n++) {
        int triangle[3][2];
        triangle[0][0] = cube2D[walls[wallIndex][0]][0];
        triangle[0][1] = cube2D[walls[wallIndex][0]][1];
        triangle[1][0] = cube2D[walls[wallIndex][1]][0];
        triangle[1][1] = cube2D[walls[wallIndex][1]][1];
        triangle[2][0] = cube2D[walls[wallIndex][2]][0];
        triangle[2][1] = cube2D[walls[wallIndex][2]][1];
        mapTriangle(triangle);
        triangle[0][0] = cube2D[walls[wallIndex][0]][0];
        triangle[0][1] = cube2D[walls[wallIndex][0]][1];
        triangle[1][0] = cube2D[walls[wallIndex][3]][0];
        triangle[1][1] = cube2D[walls[wallIndex][3]][1];
        triangle[2][0] = cube2D[walls[wallIndex][2]][0];
        triangle[2][1] = cube2D[walls[wallIndex][2]][1];
        mapTriangle(triangle);
    }
}

void Drawer::mapTriangle(int triangle[3][2]){
    int yMax=triangle[0][1];
    int yMin=triangle[0][1];
    QVector<int> X;

    for(int i=0; i<3; i++){
        if(triangle[i][1]>yMax) yMax=triangle[i][1];
        if(triangle[i][1]<yMin) yMin=triangle[i][1];
    }

    for(int y=yMin; y<=yMax; y++){
        if (y<0 || y>=im_height) continue;
        for(int i=0; i<3; i++){
            int p1=i, p2=(i+1)%3;

            if(triangle[p1][1]>triangle[p2][1]) std::swap(p1, p2);

            if (y >= triangle[p1][1] && y < triangle[p2][1] && triangle[p1][1] != triangle[p2][1]){
                X.push_back(triangle[p1][0]+(y-triangle[p1][1])*(triangle[p2][0]-triangle[p1][0])/(triangle[p2][1]-triangle[p1][1]));
            }
        }
        std::sort(X.begin(), X.end());
        if(X.size()>1){
            for(int x=X[0]; x<X[1]; x++){
                if (x<0 || x>=im_width)  continue;
                double W, Wv, Ww, u, v, w;
                double xt, yt;
                QPoint a={(int)triangle[0][0], (int)triangle[0][1]}, b={(int)triangle[1][0], (int)triangle[1][1]}, c={(int)triangle[2][0], (int)triangle[2][1]};
                QPoint at={0, 0}, bt={0, 500}, ct={500, 500};

                W = (b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x());
                Wv = ((double)x - a.x())*(c.y() - a.y()) - ((double)y - a.y())*(c.x() - a.x());
                Ww = (b.x() - a.x())*((double)y - a.y()) - (b.y() - a.y())*((double)x - a.x());
                v=Wv/W;
                w=Ww/W;

                u=1-v-w;
                xt=u*at.x() + v*bt.x() + w*ct.x();
                yt=u*at.y() + v*bt.y() + w*ct.y();
                if (xt<0 || xt>=im_width)  continue;
                if (yt<0 || yt>=im_height) continue;

                im.setPixel(x, y, imInput.pixel(xt, yt));
            }
        }
        X.clear();
    }
}


void Drawer::setDX(int dx)
{
    translateMatrix.matrix[0][3] = dx;
    executeOperations();
}

void Drawer::setDY(int dy)
{
    dy *= -1;
    translateMatrix.matrix[1][3] = dy;
    executeOperations();
}

void Drawer::setDZ(int dz)
{
    translateMatrix.matrix[2][3] = dz + f;
    executeOperations();
}

void Drawer::setSX(int sx_int)
{
    double sx = (double)sx_int / 100;
    scaleMatrix.matrix[0][0] = sx;
    executeOperations();
}

void Drawer::setSY(int sy_int)
{
    double sy = (double)sy_int / 100;
    scaleMatrix.matrix[1][1] = sy;
    executeOperations();
}

void Drawer::setSZ(int sz_int)
{
    double sz = (double)sz_int / 100;
    scaleMatrix.matrix[2][2] = sz;
    executeOperations();
}

void Drawer::setAngleX(int angleX)
{
    double radians = qDegreesToRadians(angleX);
    double cosVal = qCos(radians);
    double sinVal = qSin(radians);
    rotateXMatrix.matrix[1][1] = cosVal;
    rotateXMatrix.matrix[1][2] = -sinVal;
    rotateXMatrix.matrix[2][1] = sinVal;
    rotateXMatrix.matrix[2][2] = cosVal;
    executeOperations();
}

void Drawer::setAngleY(int angleY)
{
    double radians = qDegreesToRadians(angleY);
    double cosVal = qCos(radians);
    double sinVal = qSin(radians);
    rotateYMatrix.matrix[0][0] = cosVal;
    rotateYMatrix.matrix[0][2] = sinVal;
    rotateYMatrix.matrix[2][0] = -sinVal;
    rotateYMatrix.matrix[2][2] = cosVal;
    executeOperations();
}

void Drawer::setAngleZ(int angleZ)
{

    double radians = qDegreesToRadians(angleZ);
    double cosVal = qCos(radians);
    double sinVal = qSin(radians);
    rotateZMatrix.matrix[0][0] = cosVal;
    rotateZMatrix.matrix[0][1] = -sinVal;
    rotateZMatrix.matrix[1][0] = sinVal;
    rotateZMatrix.matrix[1][1] = cosVal;
    executeOperations();
}

void Drawer::setSHX(int shx_int)
{
    double shx = (double)shx_int / 100;
    shearMatrix.matrix[0][2] = shx;
    executeOperations();
}

void Drawer::setSHY(int shy_int)
{
    double shy = (double)shy_int / 100;
    shearMatrix.matrix[1][2] = shy;
    executeOperations();
}

