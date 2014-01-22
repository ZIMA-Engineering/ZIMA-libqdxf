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

#ifndef DXFSCENEVIEW_H
#define DXFSCENEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class DXFSceneView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DXFSceneView(QWidget *parent = 0);

private:
    QPoint mouseLast;
    bool mouseClicked;
signals:

public slots:

protected:
    void wheelEvent(QWheelEvent* event);
 /*   void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);*/

};

#endif // DXFSCENEVIEW_H
