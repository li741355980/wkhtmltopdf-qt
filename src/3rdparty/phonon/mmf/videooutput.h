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

#ifndef PHONON_MMF_VIDEOOUTPUT_H
#define PHONON_MMF_VIDEOOUTPUT_H

#include <QtGui/QWidget>

namespace Phonon
{
    namespace MMF
    {
        class VideoOutput		:	public QWidget
        {
            Q_OBJECT
            
        public:
        	VideoOutput(QWidget* parent);      
        	~VideoOutput();
			
        private:
        	
        };
    }
}

#endif