#ifndef CONVERTOR_H
#define CONVERTOR_H
#include <QImage>
#include <opencv2/opencv.hpp>

class Convertor
{

public:
    Convertor();
    static QImage Mat2QImage(const cv::Mat &mat);

    int getQPointFx(QPointF qpt);
    int getQPointFy(QPointF qpt);

    int getQPointFleft(QPointF qptA, QPointF qptB);
    int getQPointFup(QPointF qptA,QPointF qptB);

    int getQPointFwidth(QPointF qptA,QPointF qptB);
    int getQPointFheight(QPointF qptA,QPointF qptB);
};

#endif // CONVERTOR_H
