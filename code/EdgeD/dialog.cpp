#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    Qt::WindowFlags flags=Qt::Dialog;
        flags |=Qt::WindowMinMaxButtonsHint;
        flags |=Qt::WindowCloseButtonHint;
        setWindowFlags(flags);


        scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();

        scene_2 = new QGraphicsScene(this);
        ui->graphicsView_2->setScene(scene_2);
        ui->graphicsView_2->show();


        item = new MyImgItem();
        item_2 = new MyImgItem();
        item->setFlag(QGraphicsItem::ItemIsMovable);

        item_2->setFlag(QGraphicsItem::ItemIsMovable);

        scene->addItem(item);
        scene_2->addItem(item_2);

        ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        ui->graphicsView_2->setAlignment(Qt::AlignLeft | Qt::AlignTop);

}

Dialog::~Dialog()
{
    delete item;
    delete item_2;
    delete ui;

}

//打开图像
void Dialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                       ".",tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty())
    {
        image = imread(fileName.toStdString());
        gray = imread(fileName.toStdString(), 0);

        //imwrite("gray.jpg",gray);
        item->addImg(cvt.Mat2QImage(image));
        scene->update();

        item_2->addImg(cvt.Mat2QImage(gray));
        scene_2->update();
    }
    return;
}

//分水岭
void Dialog::on_pushButton_2_clicked()
{
    if(image.empty())
    {
        return;
    }
//    Mat imageMask(image.size(),CV_8U,Scalar(0));
//    //背景标记

    //去除黑边，视场调整的话删去
    cv::rectangle(marker, Point(0,0),
                  Point(image.cols,image.rows),
                  Scalar(128),255);
    item->addImg(cvt.Mat2QImage(marker));
    scene->update();


//    //前景标记
//    cv::rectangle(imageMask, Point(image.cols/2-10, image.rows/2-10),
//                  Point(image.cols/2+10, image.rows/2+10),
//                  Scalar(255),10);

    WatershedSegmenter segmenter;
    segmenter.setMarkers(marker);
    segmenter.process(image);

    imgWater = segmenter.getSegmentation();

    cv::threshold(imgWater,imgWater,128,255,THRESH_BINARY);

    item_2->addImg(cvt.Mat2QImage(imgWater));
    scene_2->update();
}
//保存分水岭
void Dialog::on_pushButton_3_clicked()
{
    if(imgWater.empty())
    {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr(" *.jpg;; *.bmp;; *.png;; *.tif;; *.GIF")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        imwrite(filename.toStdString(),imgWater);
    }
}


//寻找轮廓
void Dialog::on_pushButton_4_clicked()
{


    cv::findContours(imgWater, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

//    qDebug() << "number: " << contours.size();
//    std::vector<cv::Point> poly;
//    cv::approxPolyDP(contours.at(0), poly, 8, true);

//    polys.push_back(poly);
    polys.clear();
    for(int i = 0; i<contours.size(); i++)
    {
        std::vector<cv::Point> poly;
        cv::approxPolyDP(contours.at(i), poly, 8, true);
        polys.push_back(poly);
    }



    cv::drawContours(image, polys, -1, Scalar(0, 255, 0), 5);

//    for(int i = 0; i < poly.size(); i++)
//    {
//        cv::circle(image, poly.at(i), 25, Scalar(0, 0, 255), 3);
//    }

    item_2->addImg(cvt.Mat2QImage(image));
    scene_2->update();

}

//锐化
void Dialog::on_pushButton_5_clicked()
{
    //创建并初始化滤波模板
        cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
        kernel.at<float>(1,1) = 5.0;
        kernel.at<float>(0,1) = -1.0;
        kernel.at<float>(1,0) = -1.0;
        kernel.at<float>(1,2) = -1.0;
        kernel.at<float>(2,1) = -1.0;

        //result.create(image.size(),image.type());

        //对图像进行滤波
        cv::filter2D(image,image,image.depth(),kernel);

        item_2->addImg(cvt.Mat2QImage(image));
        scene_2->update();
}
//2值化
void Dialog::on_pushButton_6_clicked()
{

    cv::threshold(gray, imgBinary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
    item_2->addImg(cvt.Mat2QImage(imgBinary));
    scene_2->update();
}
//保存dxf
void Dialog::on_pushButton_7_clicked()
{
    m_WriteDXF = new DXFwriter();
    if(imgWater.empty())
    {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save DXF"),
        "",
        tr("*.dxf")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
       SaveGraphicsAsDXF(filename);
    }
    delete m_WriteDXF;
}

void Dialog::SaveGraphicsAsDXF(QString str)
{

    m_WriteDXF->setLines(polys);

    if(!circles.empty())
    {
        m_WriteDXF->setCircles(circles);
        //可能要删
        circles.clear();
    }

    m_WriteDXF->CreateDXFFile(str);
    m_WriteDXF->WriteHeaderSection();
    m_WriteDXF->WriteTablesSection();
    m_WriteDXF->WriteBlocksSection();
    m_WriteDXF->WriteEntitiesSection();
    m_WriteDXF->WriteObjectsSection();
    m_WriteDXF->CloseDXFFile();

}
//标记图像
void Dialog::on_pushButton_8_clicked()
{
    Mat binary;
    //cv::threshold(gray, binary, 100, 255, cv::THRESH_BINARY_INV);

//    Mat fg;
//    cv::erode(binary, fg, Mat(), cv::Point(-1,-1), 10);


    item_2->addImg(cvt.Mat2QImage(binary));
    scene_2->update();
}
//中值滤波
void Dialog::on_pushButton_9_clicked()
{
    //cv::medianBlur(image, image, 21);
    cv::medianBlur(gray, gray, 21);

    item_2->addImg(cvt.Mat2QImage(gray));
    scene_2->update();

    item->addImg(cvt.Mat2QImage(image));
    scene->update();

}
//腐蚀+膨胀
void Dialog::on_pushButton_10_clicked()
{
    Mat fg;
    cv::erode(imgBinary, fg, Mat(), Point(-1,-1), 4);
    Mat bg;
    cv::dilate(imgBinary, bg, Mat(), Point(-1,-1), 4);
    cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

    marker = fg + bg;
    item->addImg(cvt.Mat2QImage(marker));
    scene->update();

}

//保存图像
void Dialog::on_pushButton_11_clicked()
{
    if(image.empty())
    {
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"),
        "",
        tr(" *.jpg;; *.bmp;; *.png")); //选择路径
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        imwrite(filename.toStdString(),image);
    }
}


//圆孔轮廓
void Dialog::on_pushButton_12_clicked()
{
    hierarchy.clear();
    cv::findContours(imgWater, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

//    qDebug() << "number: " << contours.size();
//    std::vector<cv::Point> poly;
//    cv::approxPolyDP(contours.at(0), poly, 8, true);

//    polys.push_back(poly);
    polys.clear();

    for(int i = 0; i<contours.size(); i++)
    {
        if(!(i == 1))
        {
            std::vector<cv::Point> poly;
            cv::approxPolyDP(contours.at(i), poly, 0, true);
            polys.push_back(poly);
        }
        else
        {
            std::vector<cv::Point> poly;
            cv::approxPolyDP(contours.at(i), poly, 8, true);
            polys.push_back(poly);
        }
    }



    cv::drawContours(image, polys, -1, Scalar(0, 255, 0), 5);



//    for(int i = 0; i < poly.size(); i++)
//    {
//        cv::circle(image, poly.at(i), 25, Scalar(0, 0, 255), 3);
//    }

    item_2->addImg(cvt.Mat2QImage(image));
    scene_2->update();
}


//圆孔轮廓2
void Dialog::on_pushButton_13_clicked()
{
    hierarchy.clear();
    cv::findContours(imgWater, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

  qDebug() << "轮廓数: " << contours.size();
//    std::vector<cv::Point> poly;
//    cv::approxPolyDP(contours.at(0), poly, 8, true);

//    polys.push_back(poly);
    polys.clear();
    circles.clear();
    for(int i = 0; i<contours.size(); i++)
    {
        if(!(i == 0))
        {
            float radius;
            cv::Point2f center;
            cv::minEnclosingCircle(contours[i], center, radius);
            //缩小圆孔
            radius = 0.95 * radius;


            //画圆孔，可删
            cv::circle(image, center, static_cast<int>(radius), Scalar(255,0,0), 5);



            std::vector<float> circle;
            circle.push_back(center.x);
            circle.push_back(center.y);
            circle.push_back(radius);
            circles.push_back(circle);
        }
        else
        {
            std::vector<cv::Point> poly;
            cv::approxPolyDP(contours.at(i), poly, 8, true);
            polys.push_back(poly);

            //画顶点，可删
            for(int i = 0; i < poly.size(); i++)
            {
                cv::circle(image, poly.at(i), 6, Scalar(0, 0, 255), 6);
            }

        }
    }



    cv::drawContours(image, polys, -1, Scalar(0, 255, 0), 5);




    item_2->addImg(cvt.Mat2QImage(image));
    scene_2->update();
}
