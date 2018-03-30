#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    screenShotWidget(0)
{
    ui->setupUi(this);
    ui->comboBox->addItems(QStringList()<<"LS"<<"AB"<<"PRO");
    ui->comboBox->setCurrentIndex(0);


    Trans.load(":/Lang/Lang_ko_KR.qm");
    QApplication::installTranslator(&Trans);
    ui->retranslateUi(this);
    Setting=new QSettings("EachOne","RayoutProgram");
    ui->lineEdit->setText(Setting->value("FolderPath").toString());

    /*   QImage img("d:/2.png");

    img.invertPixels(QImage::InvertRgba);
    QImage img1(img.width(),img.height(),QImage::Format_Indexed8);
    //img1=img.convertToFormat(QImage::Format_Indexed8,Qt::ThresholdDither|Qt::AutoColor);
    img1=img.convertToFormat(QImage::Format_Indexed8);
    img1.setColorCount(2);
    img1.setColor(0,qRgb(255,255,255));
    img1.setColor(1,qRgb(0,0,0));
    img1=img1.convertToFormat(QImage::Format_RGB32);
    QColor color;
    for(int i=0; i<img1.width(); i++)
    {
        for(int j=0; j<img1.height(); j++)
        {
            color=img1.pixel(i,j);
            if(color==Qt::white)
            {
                img1.setPixel(QPoint(i,j),qRgb(0,0,0));
            }
            else if(color==Qt::black)
            {
                img1.setPixel(QPoint(i,j),qRgb(255,255,255));
            }
        }
    }
    QImage img2;

    img2=img1.copy();
    img2=img2.convertToFormat(QImage::Format_RGB32);


    img1.save("d:/22.png");
    img2=floodFill.floodfill(&img2,QPoint(1,1),qRgb(64,64,64));
    img2.save("d:/222.png");

    QDesktopServices::openUrl(QUrl("file:///d:/22.png"));
    QDesktopServices::openUrl(QUrl("file:///d:/222.png"));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ScreenShot_clicked()
{
    if(screenShotWidget==NULL)
    {
        screenShotWidget=new ScreenShotWidget(ui->comboBox->currentIndex(),ui->lineEdit->text());
        connect(screenShotWidget,SIGNAL(destroyed(QObject*)),this,SLOT(ScreenShotWidget_destroyed(QObject*)));
        connect(screenShotWidget,SIGNAL(keyPressEscape()),this,SLOT(keyPressEscape()));
    }
    this->hide();
    //screenShotWidget->SaveFileName=ui->lineEdit->text();
    //screenShotWidget->SelectPLC=ui->comboBox->currentIndex();
    screenShotWidget->show();
}
//
void MainWindow::on_pushButton_SavePath_clicked()
{
    // QString FilePath=QString(tr("S://ControlData/Doc/Board/BMP/"));
    //  ui->lineEdit->setText(QFileDialog::getSaveFileName(this,tr("Save Path"),FilePath,tr("")));//tr("All Files (*.*)")));//jpg (*.jpg);;bmp (*.bmp);;png (*.png)")));
    QString FilePath=QFileDialog::getSaveFileName(this,tr("Save Path"),Setting->value("FolderPath").toString(),tr(""));
    ui->lineEdit->setText(FilePath);
    if(FilePath!=Setting->value("FolderPath").toString())
    {
        Setting->setValue("FolderPath",FilePath);
    }
}

void MainWindow::ScreenShotWidget_destroyed(QObject *obj)
{
    screenShotWidget=NULL;
}

void MainWindow::keyPressEscape()
{
    this->show();
}


void MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl(QString("file:///%1/help.pdf").arg(QApplication::applicationDirPath())));
}
