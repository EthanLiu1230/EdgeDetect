#ifndef DXFWRITER_H
#define DXFWRITER_H

#include <stdio.h>
#include <vector>

#include <QString>
#include <QMessageBox>

#include <opencv2/opencv.hpp>




struct Vertex{
    double x;
    double y;
};
struct Line{
    Vertex pt1;
    Vertex pt2;
};

struct Circle
{
    Vertex center;
    float r;
};

class DXFwriter
{
public:
    DXFwriter();
    ~DXFwriter();

    FILE* fpDXF;
    long DXFhandle;

    std::vector<Line> lines;
    std::vector<Circle> circles;

    Line connectPts(cv::Point a, cv::Point b);
    void setLines(std::vector<std::vector<cv::Point>> polys);
    void setCircles(std::vector<std::vector<float>> holes);

    int CreateDXFFile(QString str);

    int CloseDXFFile();

    int WriteHeaderSection();

    int DXFWriteParamString(int GroupCode, QString value);
    int DXFWriteParamInteger(int GroupCode, int value);
    int DXFWriteParamDouble(int GroupCode, double value);

    int WriteClassesSection();

    int WriteTablesSection();
    int WriteTable_LTYPE();
    int WriteTable_LAYER();
    int WriteTable_STYLE();
    int WriteTable_VPORT();
    int WriteTable_VIEW();
    int WriteTable_UCS();
    int WriteTable_APPID();
    int WriteTable_DIMSTYLE();
    int WriteTable_BLOCK_RECORD();

    int WriteBlocksSection();

    int WriteObjectsSection();

    int WriteEntitiesSection();

    int WriteEntities_Lines();
    int WriteEntities_Circles();

};

#endif // DXFWRITER_H
