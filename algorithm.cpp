#include <QImage>
#include <QString>
#include <QDebug>
#include <algorithm.h>
#include <math.h>
#include <qstring.h>
void _gray(QString sourceFile, QString destFile)
{

    QImage image(sourceFile);

    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    //qDebug() << "depth - " << image.depth();

    int width = image.width();
    int height = image.height();
    QRgb color;
    int gray;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            color = image.pixel(i, j);
            gray = qGray(color);
            image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(color)));
        }
    }
    image.save(destFile);
}



void _reverse(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    qDebug() << "lighten " << sourceFile << endl;

    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    //qDebug() << "depth - " << image.depth();

    int width = image.width();
    int height = image.height();

    uchar* pimage = image.bits();

    std::vector<uchar> r(width*height);
    std::vector<uchar> g(width*height);
    std::vector<uchar> b(width*height);

    // BGRA
    qDebug() <<"Qimage last: "<< pimage[width*height*4-1]<<endl;

    for(int i = 0; i < width*height; i++)
    {
           b[i] = pimage[i*4];
           g[i] = pimage[i*4+1];
           r[i] = pimage[i*4+2];
    }
    qDebug() << "get 3 channels" <<endl;
    for(int i=0; i < width*height; i++)
    {
        pimage[i*4] = 255 - b[i];
        pimage[i*4+1] = 255 - g[i];
        pimage[i*4+2] = 255 - r[i];
        // Don't change the channel A
    }
    qDebug() << "Before reload." <<endl;
    // This function can work to transfer uchar* to QImage
    image.fromData(pimage, width*height);
    //qDebug()<<"reload success? "<<tb<<endl;
    bool tb = image.save(destFile);
    qDebug()<<"save success? "<<tb<<endl;
}
