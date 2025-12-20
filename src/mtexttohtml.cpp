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

#include "mtexttohtml.h"
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCursor>
#include <QDebug>

QString MTextToHTML::convert(QString mtext)
{
    // Capture font family, bold flag, and italic flag (non-greedy for the font name).
    QRegularExpression font(QStringLiteral("f(.+?)\\|b(.)\\|i(.)\\|c.+\\|p.+;"));

    //mtext = "{\\fArial|b0|i0|c238|p34;Mezní úchylka\\PISO 2768 - mK}";
    //mtext = "Mezní úchylka\\PISO 2768 - mK";
//    mtext.replace(font, "");

    QTextDocument doc;

    QTextCursor cur(&doc);

    QTextCharFormat format;

    QRegularExpression onechars(QStringLiteral("[LlOoKkP\\\\]"));

    bool istag = false;
    QString tag;

    for(int i=0; i< mtext.length(); i++)
    {
        QChar c = mtext[i];

        if(c == '\\' && istag==false)
        {
            istag = true;
            continue;
        }

        if(istag)
            tag += c;

        if(!istag)
            cur.insertText(c);

        if(istag && tag.length() == 1)
        {
            if(onechars.match(tag).hasMatch())
            {
                switch( tag[0].toLatin1() )
                {
                case 'L':
                    format.setFontUnderline(true);
                    break;
                case 'l':
                    format.setFontUnderline(false);
                    break;

                case 'O':
                    format.setFontOverline(true);
                    break;

                case 'o':
                    format.setFontOverline(false);
                    break;

                case 'K':
                    format.setFontStrikeOut(true);
                    break;

                case 'k':
                    format.setFontStrikeOut(false);
                    break;

                case 'X':
                case 'P':
                    cur.insertBlock();
                    break;

                case '\\':
                    cur.insertText("\\");
                    break;
                }

                istag = false;
                tag.clear();
            }

            else
            {
                int index = mtext.indexOf(";", i);
                int n = index-i;

                tag += mtext.mid(i+1, n);

                qDebug() << "TAG: " << tag;

                auto fontMatch = font.match(tag);
                if(fontMatch.hasMatch())
                {
                    // captured texts: [1] font family [2] bold [3] italic
                    format.setFontFamily(fontMatch.captured(1));
                    format.setFontWeight( fontMatch.captured(2) == "1" ? QFont::Bold : QFont::Normal);
                    format.setFontItalic(fontMatch.captured(3).toInt());
                }

                i = index;
                istag = false;
                tag.clear();
            }
        }



    }


    /*QRegExp font("\\\\f(.+)\\|b.\\|i.\\|c.+\\|p.+;");
    font.setMinimal(true);

    mtext.replace(font, "<span style=\" font-family: \\1 \">");

    //zbavení se nepodporovaných funkcí
    QRegExp re("\\\\.+;");
    re.setMinimal(true);
    mtext.replace(re, "");

    mtext.replace("{", "");
    mtext.replace("}", "");*/

    mtext = doc.toHtml();
    qDebug() << doc.toPlainText();

    return mtext;
}
