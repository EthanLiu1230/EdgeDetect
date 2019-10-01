#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0  && scaleFactor >= 0)
    {
        scaleFactor += 5;
        QMatrix old_matrix;
        old_matrix = matrix();
        resetMatrix();
        translate(old_matrix.dx(), old_matrix.dy());
        scale(scaleFactor/100.0, scaleFactor/100.0);
    }
    else if(event->delta() < 0 && scaleFactor >= 0)
    {
        scaleFactor -= 5;
        QMatrix old_matrix;
        old_matrix = matrix();
        resetMatrix();
        translate(old_matrix.dx(), old_matrix.dy());
        scale( scaleFactor/100.0,  scaleFactor/100.0);
    }
    else if (scaleFactor < 0){

            scaleFactor = 100;
        }
}
