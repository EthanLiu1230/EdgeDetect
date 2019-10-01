#include "myimgitem.h"

MyImgItem::MyImgItem(QImage &img)
{
    qimage = img;

}

MyImgItem::MyImgItem()
{

}

void MyImgItem::addImg(QImage &img)
{
    qimage = img;
}

QRectF MyImgItem::boundingRect() const
{

    return qimage.rect();
}

void MyImgItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{  
   painter->drawImage(boundingRect(),qimage);
}



