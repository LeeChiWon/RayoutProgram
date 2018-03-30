#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "floodfill.h"
#include "screenshotwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ScreenShotWidget *screenShotWidget;
    QTranslator Trans;
signals:


private slots:
    void on_pushButton_ScreenShot_clicked();
    void on_pushButton_SavePath_clicked();
    void ScreenShotWidget_destroyed(QObject *obj);
    void keyPressEscape();

    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;
    QSettings *Setting;
};

#endif // MAINWINDOW_H
