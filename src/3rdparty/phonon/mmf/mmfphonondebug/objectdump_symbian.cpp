/*  This file is part of the KDE project.

Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).

This library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 or 3 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QTextStream>
#include <QWidget>
#include <coecntrl.h>
#include "objectdump_symbian.h"

QT_BEGIN_NAMESPACE

namespace ObjectDump
{
namespace Symbian
{

QList<QByteArray> QAnnotatorControl::annotation(const QObject& object)
{
    QList<QByteArray> result;
    
    const QWidget* widget = qobject_cast<const QWidget*>(&object);
    if(widget) {
     
        const CCoeControl* control = widget->effectiveWinId();
        if(control) {
            
            QByteArray array;
            QTextStream stream(&array);
            
            stream << "control: " << control << ' ';
            stream << "parent " << control->Parent() << ' ';
            
            if(control->IsVisible())
                stream << "visible ";
            else
                stream << "invisible ";
            
            stream << control->Position().iX << ',' << control->Position().iY << ' ';
            stream << control->Size().iWidth << 'x' << control->Size().iHeight;
            
            if(control->OwnsWindow())
                stream << " ownsWindow ";
            
            stream.flush();
            result.append(array);
        }
    }
    
    return result;
}

QList<QByteArray> QAnnotatorWindow::annotation(const QObject& object)
{
    QList<QByteArray> result;
        
    const QWidget* widget = qobject_cast<const QWidget*>(&object);
    if(widget) {
        
        const CCoeControl* control = widget->effectiveWinId();
        if(control) {
            
            RDrawableWindow& window = *(control->DrawableWindow());
            
            QByteArray array;
            QTextStream stream(&array);
            
            stream << "window: ";
            
            // Client-side window handle
            // Cast to a void pointer so that log output is in hexadecimal format.
            stream << "cli " << reinterpret_cast<const void*>(window.ClientHandle()) << ' ';
            
            // Server-side address of CWsWindow object
            // This is useful for correlation with the window tree dumped by the window
            // server (see RWsSession::LogCommand).
            // Cast to a void pointer so that log output is in hexadecimal format.
            stream << "srv " << reinterpret_cast<const void*>(window.WsHandle()) << ' ';
            
            stream << "group " << window.WindowGroupId() << ' ';
            
            // Client-side handle to the parent window.
            // Cast to a void pointer so that log output is in hexadecimal format. 
            stream << "parent " << reinterpret_cast<const void*>(window.Parent()) << ' ';
            
            stream << window.Position().iX << ',' << window.Position().iY << ' ';
            stream << '(' << window.AbsPosition().iX << ',' << window.AbsPosition().iY << ") ";
            stream << window.Size().iWidth << 'x' << window.Size().iHeight;
            
            stream.flush();
            result.append(array);
        }    
    }
    
    return result;
}

} // namespace Symbian

void addDefaultAnnotators_sys(QDumper& dumper)
{
    dumper.addAnnotator(new Symbian::QAnnotatorControl);
    dumper.addAnnotator(new Symbian::QAnnotatorWindow);
}

void addDefaultAnnotators_sys(QVisitor& visitor)
{
    visitor.addAnnotator(new Symbian::QAnnotatorControl);
    visitor.addAnnotator(new Symbian::QAnnotatorWindow);
}

} // namespace ObjectDump

QT_END_NAMESPACE

