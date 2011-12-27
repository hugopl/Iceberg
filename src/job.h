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
#ifndef ICEBERG_JOB_H
#define ICEBERG_JOB_H

#include <QCoreApplication>
#include <time.h>
#include <QMap>


class Job
{
    Q_DECLARE_TR_FUNCTIONS(Job)

public:
    Job(unsigned int id = 0, unsigned int client = 0,
        const QString &filename = QString(), const QString &lang = QString());

    enum State {
        WaitingForCS,
        LocalOnly,
        Compiling,
        Finished,
        Failed,
        Idle
    };

    bool operator==(const Job &rhs) const { return m_id == rhs.m_id; }
    bool operator!=(const Job &rhs) const { return m_id != rhs.m_id; }
    int operator<(const Job &rhs) const { return m_id < rhs.m_id; }

    unsigned int jobId() const { return m_id; }
    QString fileName() const { return m_fileName; }
    unsigned int client() const { return m_client; }
    QString stateAsString() const;

    unsigned int server() const { return m_server; }
    void setServer(unsigned int hostid) { m_server = hostid; }

    time_t stime() const { return m_stime; }
    void setStartTime(time_t t) { m_stime = t; }

    State state() const { return m_state; }
    void setState(State s) { m_state = s; }

    unsigned int real_msec;  /* real time it used */
    unsigned int user_msec;  /* user time used */
    unsigned int sys_msec;   /* system time used */
    unsigned int pfaults;    /* page faults */

    int exitcode;            /* exit code */

    unsigned int in_compressed;
    unsigned int in_uncompressed;
    unsigned int out_compressed;
    unsigned int out_uncompressed;

private:
    unsigned int m_id;
    QString m_fileName;
    unsigned int m_server;
    unsigned int m_client;
    QString m_lang;
    State m_state;
    time_t m_stime;
};

class IdleJob : public Job
{
  public:
    IdleJob() : Job() { setState(Job::Idle); }
};

class JobList : public QMap<unsigned int, Job>
{
  public:
    JobList() { }
};

#endif
