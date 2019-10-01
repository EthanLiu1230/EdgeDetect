#include "dxfwriter.h"

DXFwriter::DXFwriter()
{
    fpDXF=NULL;
    DXFhandle=100;
}

DXFwriter::~DXFwriter()
{
    if (!lines.empty())
    {
        lines.clear();
    }
}

Line DXFwriter::connectPts(cv::Point a, cv::Point b)
{
    Line L;
    L.pt1.x = a.x;
    L.pt1.y = a.y;
    L.pt2.x = b.x;
    L.pt2.y = b.y;

    return L;
}


void DXFwriter::setLines(std::vector<std::vector<cv::Point>> polys)
{
    if (!lines.empty())
    {
        lines.clear();
    }

    for(int i = 0; i < polys.size(); i++)
    {
        std::vector<cv::Point> poly = polys.at(i);
        Line lastLine;
        lastLine = connectPts(poly.at(poly.size()-1), poly.at(0));
        lines.push_back(lastLine);
        for(int j = 1; j < poly.size(); j++)
        {
            Line L = connectPts(poly.at(j), poly.at(j-1));
            lines.push_back(L);
        }
    }
}

void DXFwriter::setCircles(std::vector<std::vector<float>> holes)
{
    if (!circles.empty())
    {
        circles.clear();
    }

    for(int i = 0; i < holes.size(); i++)
    {
        std::vector<float> hole = holes.at(i);
        for(int j = 1; j < hole.size(); j++)
        {
            Circle C;

            C.center.x = hole.at(0);
            C.center.y = hole.at(1);
            C.r = hole.at(2);

            circles.push_back(C);
        }
    }

}



int DXFwriter::CreateDXFFile(QString str)
{
    if (fpDXF)
        return 0;
    fopen_s(&fpDXF,str.toStdString().c_str(),"w");
    if(!fpDXF)
    {

        return 0;
    }

    return 1;
}


int DXFwriter::CloseDXFFile()
{
    DXFWriteParamString(0,"EOF");
    fclose(fpDXF);
    return 0;
}

int DXFwriter::DXFWriteParamString(int GroupCode, QString value)
{
    if(!fpDXF)
        return 0;

    if(fprintf_s(fpDXF,"%d\n\%s\n",GroupCode,value.toLocal8Bit().constData())<=0)
        return 0;

    return 1;
}


int DXFwriter::DXFWriteParamInteger(int GroupCode, int value)
{
    if(!fpDXF)
        return 0;

    if(fprintf_s(fpDXF,"%d\n\%d\n",GroupCode,value)<=0)
        return 0;

    return 1;
}


int DXFwriter::DXFWriteParamDouble(int GroupCode, double value)
{
    if(!fpDXF)
        return 0;

    if(fprintf_s(fpDXF,"%d\n\%f\n",GroupCode,value)<=0)
        return 0;

    return 1;
}


int DXFwriter::WriteHeaderSection()
{
    if(!fpDXF)
        return 0;

    DXFWriteParamString(0,"SECTION");
    DXFWriteParamString(2,"HEADER");
    DXFWriteParamString(9,"$ACADVER");
    DXFWriteParamString(1,"AC1018");   //AutoCad 版本号： 2004；

    DXFWriteParamString(9,"$INSUNITS");
    DXFWriteParamInteger(70,4);   //设置单位毫米；

    DXFWriteParamString(9,"$TEXTSTYLE");
    DXFWriteParamString(7,"Standard");   //设置当前文字样式名；

    DXFWriteParamString(9,"$HANDSEED");
    DXFWriteParamString(5,"22F");   //下一个可用句柄；

    DXFWriteParamString(0,"ENDSEC");

    return 1;
}



int DXFwriter::WriteClassesSection()
{
    return 0;
}


int DXFwriter::WriteTablesSection()
{
    if(!fpDXF)
        return 0;

    DXFWriteParamString(0,"SECTION");
    DXFWriteParamString(2,"TABLES");

    WriteTable_VPORT();
    WriteTable_LTYPE();
    WriteTable_LAYER();
    WriteTable_STYLE();
    WriteTable_VIEW();
    WriteTable_UCS();
    WriteTable_APPID();
    WriteTable_DIMSTYLE();
    WriteTable_BLOCK_RECORD();

    DXFWriteParamString(0,"ENDSEC");

    return 1;
}


int DXFwriter::WriteTable_LTYPE()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"LTYPE");
    DXFWriteParamInteger(5,5);
    DXFWriteParamString(100,"AcDbSymbolTable");

    DXFWriteParamString(0,"LTYPE");
    DXFWriteParamInteger(5,14);
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbLinetypeTableRecord");
    DXFWriteParamString(2,"ByBlock");
    DXFWriteParamInteger(70,0);
// 	DXFWriteParamString(3,"");
// 	DXFWriteParamInteger(72,65);
// 	DXFWriteParamInteger(73,0);
// 	DXFWriteParamDouble(40,0.0);

    DXFWriteParamString(0,"LTYPE");
    DXFWriteParamInteger(5,15);
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbLinetypeTableRecord");
    DXFWriteParamString(2,"ByLayer");
    DXFWriteParamInteger(70,0);
// 	DXFWriteParamString(3,"");
// 	DXFWriteParamInteger(72,65);
// 	DXFWriteParamInteger(73,0);
// 	DXFWriteParamDouble(40,0.0);
//
// 	DXFWriteParamString(0,"LTYPE");
// 	DXFWriteParamString(2,"Continuous");
// 	DXFWriteParamInteger(70,0);
// 	DXFWriteParamString(3,"Solid line");
// 	DXFWriteParamInteger(72,65);
// 	DXFWriteParamInteger(73,0);
// 	DXFWriteParamDouble(40,0.0);

    DXFWriteParamString(0,"ENDTAB");

    return 1;
}


int DXFwriter::WriteTable_LAYER()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"LAYER");
    DXFWriteParamInteger(5,2);
    DXFWriteParamString(100,"AcDbSymbolTable");

    DXFWriteParamString(0,"LAYER");
    DXFWriteParamInteger(5,10);
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbLayerTableRecord");
    DXFWriteParamString(2,"0");
    DXFWriteParamInteger(70,0);
    DXFWriteParamString(390,"F");
// 	DXFWriteParamInteger(62,7);
// 	DXFWriteParamString(6,"Continuous");

    DXFWriteParamString(0,"ENDTAB");

    return 1;
}


int DXFwriter::WriteTable_STYLE()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"STYLE");
    DXFWriteParamInteger(5,3);
    DXFWriteParamString(100,"AcDbSymbolTable");

    DXFWriteParamString(0,"STYLE");
    DXFWriteParamInteger(5,11);
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbTextStyleTableRecord");
    DXFWriteParamString(2,"Standard");
    DXFWriteParamInteger(70,0);
// 	DXFWriteParamDouble(40,0.0);
// 	DXFWriteParamDouble(41,1.0);
// 	DXFWriteParamDouble(50,0.0);
// 	DXFWriteParamInteger(71,0);
// 	DXFWriteParamDouble(42,2.5);
// 	DXFWriteParamString(3,"");
//
// 	DXFWriteParamString(0,"STYLE");
// 	DXFWriteParamString(2,"Annotative");
// 	DXFWriteParamInteger(70,0);
// 	DXFWriteParamDouble(40,0.0);
// 	DXFWriteParamDouble(41,1.0);
// 	DXFWriteParamDouble(50,0.0);
// 	DXFWriteParamInteger(71,0);
// 	DXFWriteParamDouble(42,2.5);
// 	DXFWriteParamString(3,"");

    DXFWriteParamString(0,"ENDTAB");

    return 1;
}


int DXFwriter::WriteTable_VPORT()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"VPORT");

    DXFWriteParamInteger(5,8);
    DXFWriteParamString(100,"AcDbSymbolTable");

    DXFWriteParamString(0,"VPORT");
    DXFWriteParamString(5,"EA");
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbViewportTableRecord");

    DXFWriteParamString(2,"*Active");
    DXFWriteParamInteger(70,0);

    DXFWriteParamString(0,"ENDTAB");

    return 0;
}


int DXFwriter::WriteTable_VIEW()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"VIEW");
    DXFWriteParamInteger(5,6);
    DXFWriteParamString(100,"AcDbSymbolTable");
    DXFWriteParamInteger(70,0);
    DXFWriteParamString(0,"ENDTAB");

    return 0;
}


int DXFwriter::WriteTable_UCS()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"UCS");
    DXFWriteParamInteger(5,7);
    DXFWriteParamString(100,"AcDbSymbolTable");
    DXFWriteParamInteger(70,0);
    DXFWriteParamString(0,"ENDTAB");
    return 0;
}


int DXFwriter::WriteTable_APPID()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"APPID");
    DXFWriteParamInteger(5,9);
    DXFWriteParamString(100,"AcDbSymbolTable");
    DXFWriteParamInteger(70,8);

    DXFWriteParamString(0,"APPID");
    DXFWriteParamInteger(5,12);
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbRegAppTableRecord");
    DXFWriteParamString(2,"ACAD");
    DXFWriteParamInteger(70,0);
    DXFWriteParamString(0,"ENDTAB");

    return 0;
}


int DXFwriter::WriteTable_DIMSTYLE()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"DIMSTYLE");
    DXFWriteParamString(5,"A");
    DXFWriteParamString(100,"AcDbSymbolTable");
    DXFWriteParamInteger(70,4);
    DXFWriteParamString(100,"AcDbDimStyleTable");
    DXFWriteParamInteger(71,2);

    DXFWriteParamString(0,"ENDTAB");

    return 0;
}


int DXFwriter::WriteTable_BLOCK_RECORD()
{
    DXFWriteParamString(0,"TABLE");
    DXFWriteParamString(2,"BLOCK_RECORD");
    DXFWriteParamInteger(5,1);
    DXFWriteParamString(100,"AcDbSymbolTable");
    DXFWriteParamInteger(70,2);

    DXFWriteParamString(0,"BLOCK_RECORD");
    DXFWriteParamString(5,"1F");
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbBlockTableRecord");
    DXFWriteParamString(2,"*Model_Space");

    DXFWriteParamString(0,"BLOCK_RECORD");
    DXFWriteParamString(5,"D2");
    DXFWriteParamString(100,"AcDbSymbolTableRecord");
    DXFWriteParamString(100,"AcDbBlockTableRecord");
    DXFWriteParamString(2,"*Paper_Space");

    DXFWriteParamString(0,"ENDTAB");

    return 0;
}


int DXFwriter::WriteBlocksSection()
{
    if(!fpDXF)
        return 0;

    DXFWriteParamString(0,"SECTION");
    DXFWriteParamString(2,"BLOCKS");

    DXFWriteParamString(0,"BLOCK");
    DXFWriteParamInteger(5,20);
    DXFWriteParamString(100,"AcDbEntity");
    DXFWriteParamString(100,"AcDbBlockBegin");
    DXFWriteParamString(2,"*Model_Space");
    DXFWriteParamInteger(70,0);
    DXFWriteParamString(0,"ENDBLK");
    DXFWriteParamInteger(5,21);
    DXFWriteParamString(100,"AcDbEntity");
    DXFWriteParamString(100,"AcDbBlockEnd");

    DXFWriteParamString(0,"BLOCK");
    DXFWriteParamString(5,"D4");
    DXFWriteParamString(100,"AcDbEntity");
    DXFWriteParamString(100,"AcDbBlockBegin");
    DXFWriteParamString(2,"*Paper_Space");
    DXFWriteParamInteger(70,0);
    DXFWriteParamString(0,"ENDBLK");
    DXFWriteParamString(5,"D5");
    DXFWriteParamString(100,"AcDbEntity");
    DXFWriteParamString(100,"AcDbBlockEnd");

    DXFWriteParamString(0,"ENDSEC");

    return 0;
}


int DXFwriter::WriteObjectsSection()
{
    if(!fpDXF)
        return 0;

    DXFWriteParamString(0,"SECTION");
    DXFWriteParamString(2,"OBJECTS");

    DXFWriteParamString(0,"DICTIONARY");
    DXFWriteParamString(5,"C");
    DXFWriteParamString(100,"AcDbDictionary");
    DXFWriteParamString(3,"ACAD_GROUP");
    DXFWriteParamString(350,"D");

    DXFWriteParamString(0,"DICTIONARY");
    DXFWriteParamString(5,"D");
    DXFWriteParamString(100,"AcDbDictionary");
    DXFWriteParamString(0,"ENDSEC");


    return 0;
}


int DXFwriter::WriteEntitiesSection()
{
    DXFWriteParamString(0,"SECTION");
    DXFWriteParamString(2,"ENTITIES");

    WriteEntities_Lines();
    WriteEntities_Circles();

    DXFWriteParamString(0,"ENDSEC");

    return 0;
}





//Line
int DXFwriter::WriteEntities_Lines()
{
    int i;
    for (i=0; i<lines.size(); i++)
    {
        DXFWriteParamString(0,"LINE");
        DXFWriteParamInteger(5,DXFhandle);
        DXFWriteParamString(100,"AcDbEntity");
        DXFWriteParamInteger(8,0);
        DXFWriteParamString(100,"AcDbLine");
        DXFWriteParamDouble(10,lines.at(i).pt1.x);
        DXFWriteParamDouble(20,lines.at(i).pt1.y);

        DXFWriteParamDouble(11,lines.at(i).pt2.x);
        DXFWriteParamDouble(21,lines.at(i).pt2.y);


        DXFhandle++;
    }

    return 0;
}

int DXFwriter::WriteEntities_Circles()
{
    int i;
    for (i=0; i < circles.size(); i++)
    {

        DXFWriteParamString(0,"CIRCLE");
        DXFWriteParamInteger(5,DXFhandle);
        DXFWriteParamString(100,"AcDbEntity");
        DXFWriteParamInteger(8,0);
        DXFWriteParamString(100,"AcDbCircle");
        DXFWriteParamDouble(10,circles.at(i).center.x);
        DXFWriteParamDouble(20,circles.at(i).center.y);

        DXFWriteParamDouble(40,circles.at(i).r);

        DXFhandle++;
    }

    return 0;
}
