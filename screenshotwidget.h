#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <floodfill.h>
#define LS 0
#define AB 1
#define PRO 2
#define LS_COLOR_1 qRgb(63,63,63)
#define LS_COLOR_2 qRgb(127,127,127)
#define AB_COLOR_1 qRgb(84,84,84)
#define AB_COLOR_2 qRgb(160,160,164)

namespace Ui {
class ScreenShotWidget;
}

class ScreenShotWidget : public QWidget
{
    Q_OBJECT
public:
    ScreenShotWidget(int Select, QString FileName);
    ~ScreenShotWidget();
    int SelectPLC;
    QString SaveFileName;


private:
    Ui::ScreenShotWidget *ui;

    Floodfill floodFill;
    QImage bg;
    QRubberBand *rubber;
    QPoint origin,end;
    QLabel *label;
    QPushButton *done;
    QListWidget *PointListWidget;
    QList <QPoint> PointList;
    bool Clicked,Moving;
    void Shot(float);
    void setBackground(int w,int h,float n);
    void LabelMove(QPoint pos);
    void ButtonMove(QPoint pos);
    void ListWidgetMove(QPoint pos);
    void ImageSave(QImage);

signals:
    void keyPressEscape();
public slots:
    void keyPressEvent(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void grabScreen();
};

#endif // SCREENSHOTWIDGET_H
