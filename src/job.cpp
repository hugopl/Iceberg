/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2003,2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>

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

Job::Job(unsigned int id, unsigned int client, const QString &filename, const QString &lang)
    : m_id(id),
      m_fileName(filename),
      m_lang(lang),
      m_client(client),
      m_server(0)
{
    real_msec = 0;
    user_msec = 0;
    sys_msec = 0;
    pfaults = 0;
    exitcode = 0;
    in_compressed = 0;
    in_uncompressed = 0;
    out_compressed = 0;
    out_uncompressed = 0;
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
