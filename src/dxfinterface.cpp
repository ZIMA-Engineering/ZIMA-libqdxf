/*
  ZIMA-libqdxf
  http://www.zima-engineering.cz/projekty/ZIMA-libqdxf

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dxfinterface.h"
#include "spline.h"
#include <iostream>
#include <QPen>
#include <QDebug>
#include "scene_items.h"

DXFInterface::DXFInterface(QString filename)
{

    dxfRW *rw = new dxfRW(filename.toUtf8());
    rw->read(this, false);

    mScene.setBackgroundBrush(QBrush(Qt::black));
}

DXFInterface::~DXFInterface()
{

}

void DXFInterface::add3dFace(const DRW_3Dface & /*data*/)
{

}

void DXFInterface::addArc(const DRW_Arc &data)
{
    SceneArc *  arc = new SceneArc(data);
    arc->setPen(attributesToPen(&data));
    arches.append(arc);
    mScene.addItem(arc);
}

void DXFInterface::addBlock(const DRW_Block & /*data*/)
{

}

void DXFInterface::setBlock(const int /*handle*/)
{

}

void DXFInterface::addCircle(const DRW_Circle &data)
{
    mScene.addEllipse(data.basePoint.x-data.radious, data.basePoint.y-data.radious, 2*data.radious, 2*data.radious, attributesToPen(&data));

}

void DXFInterface::addComment(const char * /*comment*/)
{

}

void DXFInterface::addDimAlign(const DRW_DimAligned * /*data*/)
{

}

void DXFInterface::addDimAngular(const DRW_DimAngular * /*data*/)
{

}

void DXFInterface::addDimAngular3P(const DRW_DimAngular3p * /*data*/)
{

}

void DXFInterface::addDimDiametric(const DRW_DimDiametric * /*data*/)
{

}

void DXFInterface::addDimLinear(const DRW_DimLinear * /*data*/)
{

}

void DXFInterface::addDimOrdinate(const DRW_DimOrdinate * /*data*/)
{

}

void DXFInterface::addDimRadial(const DRW_DimRadial * /*data*/)
{

}

void DXFInterface::addDimStyle(const DRW_Dimstyle & /*data*/)
{

}

void DXFInterface::addEllipse(const DRW_Ellipse &data)
{
    DRW_Ellipse *el = new DRW_Ellipse(data);
    DRW_Polyline *pl = new DRW_Polyline();
    el->toPolyline(pl);

    drawPolyline(pl->vertlist, attributesToPen(&data));

}

void DXFInterface::addHatch(const DRW_Hatch * /*data*/)
{

}

void DXFInterface::addHeader(const DRW_Header * /*data*/)
{
}

void DXFInterface::addImage(const DRW_Image * /*data*/)
{

}

void DXFInterface::addInsert(const DRW_Insert & /*data*/)
{

}

void DXFInterface::addKnot(const DRW_Entity & /*data*/)
{

}

void DXFInterface::addLayer(const DRW_Layer &data)
{
    layers.append(data);
}

void DXFInterface::addLeader(const DRW_Leader * /*data*/)
{

}

void DXFInterface::addLine(const DRW_Line &data)
{
    mScene.addLine(QLineF(data.basePoint.x, data.basePoint.y, data.secPoint.x, data.secPoint.y), attributesToPen(&data));
}

void DXFInterface::addLType(const DRW_LType & /*data*/)
{

}

void DXFInterface::addLWPolyline(const DRW_LWPolyline &data)
{
    QPen pen = attributesToPen(&data);

    for(unsigned int i = 1; i < data.vertlist.size(); i++)
    {
        DRW_Vertex2D *verta = data.vertlist[i-1];
        DRW_Vertex2D *vertb = data.vertlist[i];

        mScene.addLine(verta->x, verta->y, vertb->x, vertb->y, pen);
    }
}

void DXFInterface::addMText(const DRW_MText & /*data*/)
{
}

void DXFInterface::addPoint(const DRW_Point &data)
{
    mScene.addLine(data.basePoint.x, data.basePoint.y, data.basePoint.x, data.basePoint.y, attributesToPen(&data));
}

void DXFInterface::addPolyline(const DRW_Polyline &data)
{
    drawPolyline(data.vertlist, attributesToPen(&data));
}

void DXFInterface::addRay(const DRW_Ray & /*data*/)
{

}

void DXFInterface::addSolid(const DRW_Solid & /*data*/)
{

}

void DXFInterface::addSpline(const DRW_Spline *data)
{
    QList<QLineF> qlines;
    Spline sp(data);
    sp.update(qlines);

    QPen pen = attributesToPen(data);

    foreach(QLineF line, qlines)
    {
        mScene.addLine(line, pen);

    }
}

void DXFInterface::addText(const DRW_Text & /*data*/)
{
}

void DXFInterface::addTextStyle(const DRW_Textstyle & /*data*/)
{

}

void DXFInterface::addTrace(const DRW_Trace & /*data*/)
{

}

void DXFInterface::addViewport(const DRW_Viewport & /*data*/)
{

}

void DXFInterface::addVport(const DRW_Vport & /*data*/)
{

}

void DXFInterface::addXline(const DRW_Xline & /*data*/)
{

}

void DXFInterface::endBlock()
{

}

void DXFInterface::linkImage(const DRW_ImageDef * /*data*/)
{

}

void DXFInterface::writeBlockRecords()
{

}

void DXFInterface::writeBlocks()
{

}

void DXFInterface::writeDimstyles()
{

}

void DXFInterface::writeEntities()
{

}

void DXFInterface::writeHeader(DRW_Header & /*data*/)
{

}

void DXFInterface::writeLayers()
{

}

void DXFInterface::writeLTypes()
{

}

void DXFInterface::writeTextstyles()
{

}

void DXFInterface::writeVports()
{

}


QPen DXFInterface::attributesToPen(const DRW_Entity *e)
{
    QPen p(Qt::yellow, 0, Qt::SolidLine);

    if(e->visible == false)
        qDebug() << "nevid";

    if (e->color == 256)
    { // BYLAYER
        DRW_Layer l = getLayer(e->layer);
        p.setColor( numberToColor( l.color));
    }

    else if (e->color24 >= 0)
        p.setColor(QColor(e->color24 >> 16,
                          e->color24 >> 16 & 0xFF,
                          e->color24 & 0xFF));

    else
        p.setColor(numberToColor(e->color));

    if(e->lineType == "BYLAYER")
    {
        DRW_Layer l = getLayer(e->layer);
        setQPenLinetype(p, l.lineType);
    }
    else
        setQPenLinetype(p, e->lineType);


    return p;
}

QColor DXFInterface::numberToColor(int col)
{
    // 7 je black or white - potrebuju white (255)
    if (col == 7)
        col = 255;

    if (col==0)
    {
        //return RS_Color(RS2::FlagByBlock);
    }
    else if (col==256)
    {
        //return RS_Color(RS2::FlagByLayer);
    }
    else if (col<=255 && col>=0)
    {

        return QColor::fromRgb(DRW::dxfColors[col][0],
                DRW::dxfColors[col][1],
                DRW::dxfColors[col][2]);
    }

    return QColor(Qt::red);
    // else: neznama barva
}

DRW_Layer DXFInterface::getLayer(std::string name)
{
    foreach(DRW_Layer l, layers)
    {
        if (l.name == name)
            return l;
    }

    qDebug() << "layer not found :" << name.c_str();
    return DRW_Layer();
}

void DXFInterface::setQPenLinetype(QPen & p, std::string linetype)
{
    if (linetype == "CONTINUOUS" || linetype == "Continuous")
        p.setStyle(Qt::SolidLine);

    else if(linetype == "HIDDEN")
        p.setStyle(Qt::DashLine);

    else if(linetype == "CENTER")
    {
        QVector<qreal> dashes;
        dashes << 6 << 2 << 2 << 2;
        p.setDashPattern(dashes);
    }
    else
    {
        qDebug() << "unknown linetype" << linetype.c_str();
    }
    return;
}

void DXFInterface::drawPolyline(std::vector<DRW_Vertex*> vertlist, QPen pen)
{
    for(unsigned int i = 1; i < vertlist.size(); i++)
    {
        DRW_Vertex *verta = vertlist[i-1];
        DRW_Vertex *vertb = vertlist[i];

        mScene.addLine(verta->basePoint.x, verta->basePoint.y, vertb->basePoint.x, vertb->basePoint.y, pen);
    }
}

QGraphicsScene * DXFInterface::scene()
{
    return &mScene;
}
