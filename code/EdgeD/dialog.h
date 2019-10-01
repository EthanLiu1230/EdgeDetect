#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QFileDialog>

#include <QDebug>

#include "mygraphicsview.h"
#include "myimgitem.h"
#include "convertor.h"
#include "watershedSegmentation.h"

#include "dxfwriter.h"


namespace Ui {
class Dialog;
}

using namespace cv;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::Dialog *ui;

    QGraphicsScene *scene;
    QGraphicsScene *scene_2;

    MyImgItem *item;
    MyImgItem *item_2;

    //OpenCV
    Convertor cvt;
    Mat image;
    Mat gray;
    Mat imgWater;
    Mat imgBinary;
    Mat marker;

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    std::vector<std::vector<cv::Point>> polys;
    std::vector<std::vector<float>> circles;

    Mat imgSharp;

    DXFwriter *m_WriteDXF;

    void SaveGraphicsAsDXF(QString str);
};

#endif // DIALOG_H
