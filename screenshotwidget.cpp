#include "screenshotwidget.h"

ScreenShotWidget::ScreenShotWidget(int Select, QString FileName)
{    
    SelectPLC=Select;
    SaveFileName=FileName;
    Shot((float)0.6);
    Clicked=false;
    Moving=false;
}

ScreenShotWidget::~ScreenShotWidget()
{

}

void ScreenShotWidget::Shot(float n)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect deskRect = desktop->screenGeometry();
    this->setMouseTracking(true);
    this->resize(deskRect.width(),deskRect.height());
    this ->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    setBackground(deskRect.width(),deskRect.height(),n);
    rubber =  NULL;
    origin = end = QPoint(0,0);
    label=new QLabel(this);
    done=new QPushButton(this);
    rubber=new QRubberBand(QRubberBand::Rectangle,this);
    PointListWidget=new QListWidget(this);

    label->setAttribute(Qt::WA_TranslucentBackground);
    done->setAttribute(Qt::WA_TranslucentBackground);
    PointListWidget->setAttribute(Qt::WA_TranslucentBackground);

    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    label->setPalette(pal);
    done->setPalette(pal);

    label->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    done->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    PointListWidget->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    done->setText(tr("Done"));
    label->setText(tr("Size: 280 x 280"));

    connect(done,SIGNAL(clicked()),this,SLOT(grabScreen()));
}

void ScreenShotWidget::setBackground(int w, int h,float n)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    // screen->grabWindow(0).save("bg.bmp","bmp");

    int r,g,b;
    bg = screen->grabWindow(0).toImage();
    QImage bg_grey(w,h,QImage::Format_RGB32);
    for(int i=0;i<w;i++)
    {
        for(int j=0;j<h;j++)
        {
            /*if((i>=rubber->geometry().left() && i<=rubber->geometry().right()) &&(j>=rubber->geometry().top() && j<=rubber->geometry().bottom()))
            {
                r = qRed(bg.pixel(i,j));
                g = qGreen(bg.pixel(i,j));
                b = qBlue(bg.pixel(i,j));
            }*/
            //else
            //{
            r = qRed(bg.pixel(i,j))*n;
            g = qGreen(bg.pixel(i,j))*n;
            b = qBlue(bg.pixel(i,j))*n;
            //}
            bg_grey.setPixel(i,j,qRgb(r,g,b));
        }
    }
    QPalette palette;
    /*palette.setBrush(this->backgroundRole(),QBrush(bg));
    this->setPalette(palette);*/
    palette.setBrush(this->backgroundRole(),QBrush(bg_grey));
    this->setPalette(palette);
    this->showFullScreen();
}

void ScreenShotWidget::LabelMove(QPoint pos)
{
    QPoint LabelPos(pos.x(),pos.y()-15);
    QString Text=QString(tr("Point: %1,%2  Size: %3  x  %4")).arg(pos.x()).arg(pos.y()).arg(rubber->geometry().width()).arg(rubber->geometry().height());
    label->setText(Text);
    label->move(LabelPos);
    label->show();
}

void ScreenShotWidget::ButtonMove(QPoint pos)
{
    QPoint ButtonPos(pos.x()-74,pos.y());
    done->move(ButtonPos);
    done->show();
}

void ScreenShotWidget::ListWidgetMove(QPoint pos)
{
    PointListWidget->move(QPoint(pos.x(),pos.y()));
    PointListWidget->show();
}

void ScreenShotWidget::ImageSave(QImage img)
{
    QImage TempImg,TempImg2;
    TempImg=img.copy();
    TempImg.invertPixels(QImage::InvertRgba);
    QImage TempImg1(TempImg.width(),TempImg.height(),QImage::Format_Indexed8);
    //img1=img.convertToFormat(QImage::Format_Indexed8,Qt::ThresholdDither|Qt::AutoColor);
    TempImg1=TempImg.convertToFormat(QImage::Format_Indexed8);
    TempImg1.setColorCount(2);
    TempImg1.setColor(0,qRgb(255,255,255));
    TempImg1.setColor(1,qRgb(0,0,0));
    TempImg1=TempImg1.convertToFormat(QImage::Format_RGB32);

    if(SaveFileName.isEmpty())
    {
        SaveFileName="c:/temp";
    }

    switch(SelectPLC)
    {
    case LS:
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(1,1),LS_COLOR_1);
        for(int i=0; i<PointList.count(); i++)
        {
            TempImg1=floodFill.floodfill(&TempImg1,PointList.at(i),LS_COLOR_2);
        }

        /*TempImg1=floodFill.floodfill(&TempImg1,QPoint(95,173),LS_COLOR_2);
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(200,85),LS_COLOR_2);
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(195,215),LS_COLOR_2);*/
        SaveFileName.append(".jpg");
        break;
    case AB:
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(1,1),AB_COLOR_1);
        for(int i=0; i<PointList.count(); i++)
        {
            TempImg1=floodFill.floodfill(&TempImg1,PointList.at(i),AB_COLOR_2);
        }

     /*   TempImg1=floodFill.floodfill(&TempImg1,QPoint(90,50),AB_COLOR_2);
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(95,173),AB_COLOR_2);
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(200,85),AB_COLOR_2);
        TempImg1=floodFill.floodfill(&TempImg1,QPoint(195,215),AB_COLOR_2);*/
        SaveFileName.append(".jpg");
        break;
    case PRO:
        QColor color;
        for(int i=0; i<TempImg1.width(); i++)
        {
            for(int j=0; j<TempImg1.height(); j++)
            {
                color=TempImg1.pixel(i,j);
                if(color==Qt::white)
                {
                    TempImg1.setPixel(QPoint(i,j),qRgb(0,0,0));
                }
                else if(color==Qt::black)
                {
                    TempImg1.setPixel(QPoint(i,j),qRgb(255,255,255));
                }
            }
        }
        SaveFileName.append(".bmp");
        break;
    }
    TempImg2=TempImg1.copy();
   // TempImg2=TempImg2.convertToFormat(QImage::Format_RGB32);

    TempImg2.save(SaveFileName);
    QDesktopServices::openUrl(QUrl(SaveFileName.prepend("file:///")));
}

void ScreenShotWidget::grabScreen()
{
    QImage pic = bg.copy(rubber->geometry().x(),rubber->geometry().y(),rubber->geometry().width(),rubber->geometry().height());
    /*QClipboard *b = QApplication::clipboard();
    b->setImage(pic);*/
    ImageSave(pic);

   /* done->close();
    label->close();
    rubber->close();
    PointListWidget->close();
    //this->hide();
    SaveFileName.clear();
    PointList.clear();*/
    emit keyPressEscape();
    this->deleteLater();
}

void ScreenShotWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Escape:
        //this->hide();
        emit keyPressEscape();
        this->deleteLater();
        break;
    case Qt::Key_D:
        rubber->move(QPoint(rubber->geometry().x()+1,rubber->geometry().y()));
        break;
    case Qt::Key_S:
        rubber->move(QPoint(rubber->geometry().x(),rubber->geometry().y()+1));
        break;
    case Qt::Key_A:
        rubber->move(QPoint(rubber->geometry().x()-1,rubber->geometry().y()));
        break;
    case Qt::Key_W:
        rubber->move(QPoint(rubber->geometry().x(),rubber->geometry().y()-1));
        break;
    }
    rubber->show();
    if(e->key()!=Qt::Key_Escape)
    {
        LabelMove(QPoint(rubber->geometry().x(),rubber->geometry().y()));
        ButtonMove(rubber->geometry().bottomRight());
        ListWidgetMove(rubber->geometry().topRight());
    }
}

void ScreenShotWidget::mouseMoveEvent(QMouseEvent *e)
{
    //QPoint Temp;
    if(e->buttons() & Qt::LeftButton)
    {
        Moving=true;
       // qDebug()<<"origin:"<<origin<<"mouse:"<<e->pos();
        rubber->move(rubber->pos()+(e->pos()-origin));
        rubber->show();
        LabelMove(rubber->pos());
        ButtonMove(rubber->geometry().bottomRight());
        ListWidgetMove(rubber->geometry().topRight());
        origin=e->pos();
    }
}

void ScreenShotWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        origin=e->pos();

        if(!Clicked && rubber->isHidden())
        {
            rubber->setGeometry(e->pos().x(),e->pos().y()+15,280,280);
            label->setGeometry(e->pos().x(),e->pos().y(),280,15);
            done->setGeometry(rubber->geometry().bottomRight().x()-74,rubber->geometry().bottomRight().y(),75,23);
            PointListWidget->setGeometry(rubber->geometry().topRight().x(),rubber->geometry().topRight().y(),100,280);
            done->show();
            rubber->show();
            label->show();
            PointListWidget->show();
            Clicked=true;
        }
        else
        {
            done->hide();
            label->hide();
            rubber->hide();
            PointListWidget->clear();
            PointListWidget->hide();
            PointList.clear();
            Clicked=false;
        }
    }

    if(e->button() & Qt::RightButton)
    {
        //qDebug()<<e->pos()<<rubber->mapFromParent(e->pos());
        PointListWidget->addItem(new QListWidgetItem(QString("%1,%2").arg(rubber->mapFromParent(e->pos()).x()).arg(rubber->mapFromParent(e->pos()).y())));
        PointList.append(rubber->mapFromParent(e->pos()));
    }
}

void ScreenShotWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        if(Moving)
        {
            Moving=false;
        }
    }
}
