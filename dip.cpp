#include <dip.h>
#include <QObject>
#include <algorithm.h>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QUrl>
#include <QDir>
#include <QRunnable>
#include <QThread>

//  This class is uesd to provide logic operation
class ImageProcessorPrivate : public QObject
{
public:


    ImageProcessorPrivate(Dip* processor):m_processor(processor)
    {}
    ~ImageProcessorPrivate()
    {}

    QString m_tempPath;

    /* This class have a memeber which is an object of Dip class.
     * This memeber is used to implement method in Dip.
     *
     *  And in the implemetation of method in Dip,
     * it just need to use object of ImageProcessorPrivate class to
     * call ImageProcessorPrivate class' method.
     *
     */
    Dip* m_processor;
    //TODO
};





/*
 * Implement of class Dip's method
 * */
QString Dip::sourceFile() const
{
    //return m_d->m_notifiedSourceFile;
    return nullptr;
}
void Dip::process(QString fileUrl, ImageAlgorithm algorithm)
{


    // The fileUrl is a url, not a path. So we need to get the absolute path.
    QUrl url(fileUrl);

    // Get the source image's path.
    QString filepath = url.toLocalFile();
    qDebug() << filepath << endl;

     // this path is used to store processed image.
    QString tmp_path =  QString("%1/tmp.jpg").arg(QDir::currentPath());
    qDebug() << tmp_path << endl;

    switch (algorithm) {
    case Gray:
    {
        qDebug("To Gray");
        _gray(filepath, tmp_path);
        break;
    }
    case Reverse:
    {
        qDebug("Reverse");
        _reverse(filepath, tmp_path);
        break;
    }
    case HistEqu:
    {
        qDebug("HistEqu");
        _histEqu(filepath, tmp_path);
        break;
    }
    case Smooth:
    {
        qDebug("Smooth");
        _smooth(filepath, tmp_path);
        break;
    }
    default:
        break;
    }
    qDebug() <<"tmp_path is " <<tmp_path << endl;
    finished(tmp_path);
}
/*
 * Implement of constructor function.
 * Here it means parent(the parameter in Object()) = QObject *parent
 * Ref: https://www.cnblogs.com/bonelee/p/5825885.html
 * */
Dip::Dip(QObject *parent):QObject(parent)  // constructor
{}

Dip::~Dip() // Destructor
{}


