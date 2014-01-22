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

#include <QApplication>
#include <QFileDialog>
#include "../src/dxfinterface.h"
#include "../libdxfrw/src/libdxfrw.h"
#include <../src/dxfsceneview.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ZIMA-libQDXF");
    app.setApplicationVersion("1.0.0");

    QString filename = QFileDialog::getOpenFileName(0, "Open DXF", QString(), "DXF Files (*.dxf)");

    if(filename.isEmpty())
    {
        exit(1);
    }

    DXFInterface dxf(filename);

    DXFSceneView view;
    view.resize(800, 600);
    view.setScene(dxf.scene());

    view.fitInView(dxf.scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    view.show();

    return app.exec();
}
