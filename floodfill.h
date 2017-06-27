#ifndef FLOODFILL_H
#define FLOODFILL_H

#include <QtWidgets>
#include <QObject>

class Floodfill : public QObject
{
    Q_OBJECT
public:
    explicit Floodfill(QObject *parent = 0);
    ~Floodfill();
    QImage floodfill(QImage *img, const QPoint &pos, const QRgb &newColor);
signals:

public slots:
};

#endif // FLOODFILL_H
