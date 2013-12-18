/**
 * parseucd.cpp
 *
 * Copyright 2006 Jacob Rideout <kde@jacobrideout.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

int main()
{
    QString str;
    QTextStream in(stdin);

    QFile sout;
    sout.open(stdout, QIODevice::WriteOnly);
    QDataStream out(&sout);

    bool ok = true, ok2;
    QHash<quint32,quint8> data;
    QHash<qint8,QString> catalog;

    while ( ok )
    {
        QString line = in.readLine();
        ok =  !in.atEnd();

        if ( line.isEmpty() || line.startsWith('#') )
            continue;

        QRegExp rx(";");
        int split = rx.indexIn(line);

        QString catagoryString = line.right( line.size() - split - 1 ).simplified();

        qint8 catagory = catalog.key(catagoryString);
        if(!catagory)
        {
            catalog[ catagory = catalog.size()+1 ] = catagoryString;
        }

        QString codes = line.left( split ).simplified();
        QStringList codeList = codes.split ( ".." );

        quint32 start = codeList.at(0).toInt(&ok2, 16);
        quint32 end = (codeList.size() == 2) ? codeList.at(1).toInt(&ok2, 16) : start;
        for (quint32 code = start; code<=end; ++code)
        {
            data.insert( code, catagory );
            qDebug() << "[" << catagory << "] " << code;
        }
    }

    out << catalog << data;
}
