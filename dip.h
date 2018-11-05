#ifndef DIP_H
#define DIP_H
#include <QObject>
#include <QString>

//  This class is uesd to provide logic operation
class ImageProcessorPrivate;

// This class is used to provide UI.
class Dip : public QObject
{
    /*
     * if a C++ class need to imported in QML file,
     * it should have 2 characteristics:
     *      1. Have "Q_OBJECT" macro;
     *      2. inherit from the derived classes
     *           of QObject or QObject.
     **/
    Q_OBJECT


    // Must have this macro to make the variable can be accessed in QML.
    Q_ENUMS(ImageAlgorithm)
    Q_PROPERTY(QString sourceFile READ sourceFile)
    //Q_PROPERTY(ImageAlgorithm algorithm READ algorithm)

public:

    Dip(QObject *parent = nullptr);  // constructor
    /* "QObject *parent" is the parameter.
     * And it equals to "nullptr" by default.
     *
     * */
    ~Dip(); // Destructor

    QString sourceFile() const;
    /* "const" after function means
     * the func can't and won't change the
     * variable in class.
    */

    /*
     * Define an enum type.
     * The enum type will be a parameter of process() function.
     * Different kinds of algorithm will be used depends of the value of the parameter.
     * */
    enum ImageAlgorithm{
        Gray = 0,
        Reverse,
        HistEqu,
        Smooth,
        AlgorithmCount
    };
    // Define a variabel.
    ImageAlgorithm algorithm;

signals:
    // The signals function just is declared here. Its implemention is in QML file.
    // The signals function is called in C++ file.

    // The parameter of signals funtion will be uesd in qml.
    void finished(QString newFile);

public slots: // slots function is called when signal is trigged in QML.
    // The slots function just is declared here. Its implemention is also in C++ source file.

    void process(QString fileUrl, ImageAlgorithm algorithm);


private: // This object is uesd to excecute logic operation
  //  ImageProcessorPrivate *m_d;
};




#endif // DIP_H
