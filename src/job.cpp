/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2003,2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>
    Copyright (c) 2012 Luis Gabriel Lima <lampih@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "job.h"

#include <QObject>

Job::Job(quint32 id, quint32 client, const QString &filename, const QString &lang)
    : m_id(id),
      m_fileName(filename),
      m_server(0),
      m_client(client),
      m_lang(lang),
      m_state(WaitingForCS),
      m_startTime(QDateTime()),
      m_realTime(0),
      m_userTime(0),
      m_systemTime(0),
      m_pageFaults(0),
      m_exitCode(0),
      m_compressedInputSize(0),
      m_inputSize(0),
      m_compressedOutputSize(0),
      m_outputSize(0)
{
}

QString Job::stateAsString() const
{
    switch (m_state) {
    case WaitingForCS:
        return tr("Waiting");
    case Compiling:
        return tr("Compiling");
    case Finished:
        return tr("Finished");
    case Failed:
        return tr("Failed");
    case Idle:
        return tr("Idle");
    case LocalOnly:
        return tr("LocalOnly");
    }
    return QString();
}

void Job::setExecInfo(quint32 real, quint32 user, quint32 sys, quint32 pf)
{
    m_realTime = real;
    m_userTime = user;
    m_systemTime = sys;
    m_pageFaults = pf;
}

void Job::setSizes(quint32 compIn, quint32 in, quint32 compOut, quint32 out)
{
    m_compressedInputSize = compIn;
    m_inputSize = in;
    m_compressedOutputSize = compOut;
    m_outputSize = out;
}

QString Job::formatTime(quint32 time)
{
        return QObject::tr("%1 s").arg(QString::number(time/qreal(1000), 'f', 2));
}

QString Job::formatSize(quint32 size)
{
    if (size >= 1048576)
        return QObject::tr("%1 MiB").arg(QString::number(size/qreal(1048576), 'f', 2));
    else if (size >= 1024)
        return QObject::tr("%1 KiB").arg(QString::number(size/qreal(1024), 'f', 2));
    else
        return QObject::tr("%1 B").arg(QString::number(size));
}
