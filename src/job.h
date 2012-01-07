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
#ifndef ICEBERG_JOB_H
#define ICEBERG_JOB_H

#include <QCoreApplication>
#include <QDateTime>
#include <QMap>


class Job
{
    Q_DECLARE_TR_FUNCTIONS(Job)

public:
    Job(quint32 id = 0, quint32 client = 0, const QString &filename = QString(),
        const QString &lang = QString());

    enum State {
        WaitingForCS,
        LocalOnly,
        Compiling,
        Finished,
        Failed,
        Idle
    };

    bool operator==(const Job &rhs) const { return m_id == rhs.jobId(); }
    bool operator!=(const Job &rhs) const { return m_id != rhs.jobId(); }
    int operator<(const Job &rhs) const { return m_id < rhs.jobId(); }

    quint32 jobId() const { return m_id; }
    QString fileName() const { return m_fileName; }
    quint32 client() const { return m_client; }

    QString stateAsString() const;

    quint32 server() const { return m_server; }
    void setServer(quint32 hostid) { m_server = hostid; }

    QDateTime startTime() const { return m_startTime; }
    void setStartTime(const QDateTime& t) { m_startTime = t; }

    State state() const { return m_state; }
    void setState(State s) { m_state = s; }

    quint32 realTime() const { return m_realTime; }
    quint32 userTime() const { return m_userTime; }
    quint32 systemTime() const { return m_systemTime; }
    quint32 pageFaults() const { return m_pageFaults; }
    void setExecInfo(quint32 real, quint32 user, quint32 sys, quint32 pf);

    int exitCode() const { return m_exitCode; }
    void setExitCode(int ec) { m_exitCode = ec; }

    quint32 compressedInputSize() const { return m_compressedInputSize; }
    quint32 inputSize() const { return m_inputSize; }
    quint32 compressedOutputSize() const { return m_compressedOutputSize; }
    quint32 outputSize() const { return m_outputSize; }
    void setSizes(quint32 compIn, quint32 in, quint32 compOut, quint32 out);

    QString formattedRealTime() const { return formatTime(m_realTime); }
    QString formattedUserTime() const { return formatTime(m_userTime); }
    QString formattedInputSize() const { return formatSize(m_inputSize); }
    QString formattedOutputSize() const { return formatSize(m_outputSize); }

private:
    static QString formatTime(quint32 time);
    static QString formatSize(quint32 size);

    quint32 m_id;
    QString m_fileName;
    quint32 m_server;
    quint32 m_client;
    QString m_lang;
    State m_state;
    QDateTime m_startTime;

    quint32 m_realTime;    /* real time it used in milliseconds */
    quint32 m_userTime;    /* user time used in milliseconds */
    quint32 m_systemTime;  /* system time used in milliseconds */
    quint32 m_pageFaults;  /* page faults */
    int m_exitCode;        /* exit code */

    quint32 m_compressedInputSize;
    quint32 m_inputSize;
    quint32 m_compressedOutputSize;
    quint32 m_outputSize;
};

typedef QMap<quint32, Job> JobList;

#endif
