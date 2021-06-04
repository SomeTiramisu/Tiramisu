#ifndef CROPRUNNABLE_H
#define CROPRUNNABLE_H

#include <QObject>
#include <QRunnable>

class CropRunnable: public QObject, public QRunnable
{
    Q_OBJECT
public:
    CropRunnable();
};

#endif // CROPRUNNABLE_H
