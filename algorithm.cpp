#include <QImage>
#include <QString>
#include <QDebug>
#include <algorithm.h>
#include <math.h>
#include <qstring.h>
#include <stdlib.h>
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

//    std::vector<uchar> r(width*height);
//    std::vector<uchar> g(width*height);
//    std::vector<uchar> b(width*height);

    uchar* r = (uchar*)malloc(sizeof (uchar)*width*height);
    uchar* b = (uchar*)malloc(sizeof (uchar)*width*height);
    uchar* g = (uchar*)malloc(sizeof (uchar)*width*height);
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
    free(r);
    free(g);
    free(b);
}



typedef uchar dtype;
typedef int num_t;


num_t* histogram(dtype *img, int img_size)
{
        int gray_level = 256;
        num_t* hist = (num_t*)malloc(sizeof(num_t)*(gray_level));  // each gray level may have great number
        for(int i=0;i<gray_level;i++)
        {
                for(int j=0;j<img_size;j++)
                {
                        if(img[j]==i)
                        {
                                hist[i] +=1;
                        }
                }
        }
        return hist;
}


dtype* hist_equalization(dtype *img, int img_size)
{
        int gray_level = 256;
        num_t* hist = histogram(img, img_size);
        num_t* hist_acc = (num_t*)malloc(sizeof(num_t)*gray_level);  // Accumulate histogram data
        dtype* new_img = (dtype*)malloc(sizeof(dtype)*img_size);

        for(int i=0;i<gray_level;i++)
        {
                if(i==0)
                {
                        hist_acc[i] = hist[i];
                }
                else
                {
                        hist_acc[i] = hist_acc[i-1]+hist[i];
                        hist_acc[i-1] = hist_acc[i-1]*(gray_level-1)/img_size;          // don't need hist_acc[i-1] to store accumulate number any more
                }
        }
        //hist_acc[gray_level-1] =  hist_acc[gray_level-1]*(gray_level-1)/img_size;    //actually hist_acc[gray_level-1] equals to img_size before reassignment
        hist_acc[gray_level-1] = gray_level-1;

        for(int i=0;i<img_size;i++)
        {
                int pixel_value = img[i];
                new_img[i] = hist_acc[pixel_value];
        }

        free(hist_acc);
        free(hist);
        return new_img;
}



void _histEqu(QString sourceFile, QString destFile)
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

//    std::vector<uchar> r(width*height);
//    std::vector<uchar> g(width*height);
//    std::vector<uchar> b(width*height);
    uchar* r = (uchar*)malloc(sizeof (uchar)*width*height);
    uchar* b = (uchar*)malloc(sizeof (uchar)*width*height);
    uchar* g = (uchar*)malloc(sizeof (uchar)*width*height);
    // BGRA
    qDebug() <<"Qimage last: "<< pimage[width*height*4-1]<<endl;

    for(int i = 0; i < width*height; i++)
    {
           b[i] = pimage[i*4];
           g[i] = pimage[i*4+1];
           r[i] = pimage[i*4+2];
    }

    qDebug() << "get 3 channels" <<endl;

    b = hist_equalization(b, width*height);


    for(int i=0; i < width*height; i++)
    {
        pimage[i*4] = b[i];
        pimage[i*4+1] = b[i];
        pimage[i*4+2] = b[i];
        // Don't change the channel A
    }
    qDebug() << "Before reload." <<endl;
    // This function can work to transfer uchar* to QImage
    image.fromData(pimage, width*height);
    //qDebug()<<"reload success? "<<tb<<endl;
    bool tb = image.save(destFile);
    qDebug()<<"save success? "<<tb<<endl;

    free(r);
    free(g);
    free(b);
}


void _smooth(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    int r, g, b;
    QRgb color;
    int xLimit = width - 1;
    int yLimit = height - 1;
    for(int i = 1; i < xLimit; i++)
    {
        for(int j = 1; j < yLimit; j++)
        {
            r = 0;
            g = 0;
            b = 0;
            for(int m = 0; m < 9; m++)
            {
                int s = 0;
                int p = 0;
                switch(m)
                {
                case 0:
                    s = i - 1;
                    p = j - 1;
                    break;
                case 1:
                    s = i;
                    p = j - 1;
                    break;
                case 2:
                    s = i + 1;
                    p = j - 1;
                    break;
                case 3:
                    s = i + 1;
                    p = j;
                    break;
                case 4:
                    s = i + 1;
                    p = j + 1;
                    break;
                case 5:
                    s = i;
                    p = j + 1;
                    break;
                case 6:
                    s = i - 1;
                    p = j + 1;
                    break;
                case 7:
                    s = i - 1;
                    p = j;
                    break;
                case 8:
                    s = i;
                    p = j;
                }
                color = image.pixel(s, p);
                r += qRed(color);
                g += qGreen(color);
                b += qBlue(color);
            }

            r = (int) (r / 9.0);
            g = (int) (g / 9.0);
            b = (int) (b / 9.0);

            r = qMin(255, qMax(0, r));
            g = qMin(255, qMax(0, g));
            b = qMin(255, qMax(0, b));

            image.setPixel(i, j, qRgb(r, g, b));
        }
    }

    image.save(destFile);
}


