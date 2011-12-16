/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2003,2004 Stephan Kulow <coolo@kde.org>
    Copyright (c) 2004, 2005 Andre Wöbbeking <Woebbeking@web.de>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>
    Copyright (c) 2011 Anselmo L. S. Melo <anselmolsm@gmail.com>

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

#include "joblistview.h"

#include "hostinfo.h"
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QApplication>
#include <QHeaderView>

enum JobJobColumns
{
    JobColumnID,
    JobColumnFilename,
    JobColumnClient,
    JobColumnServer,
    JobColumnState,
    JobColumnReal,
    JobColumnUser,
    JobColumnFaults,
    JobColumnSizeIn,
    JobColumnSizeOut
};


JobListViewItem::JobListViewItem(QTreeWidget* parent, const Job& job)
    :  QTreeWidgetItem(parent)
{
    setTextAlignment(JobColumnID, Qt::AlignRight);
    setTextAlignment(JobColumnReal, Qt::AlignRight);
    setTextAlignment(JobColumnUser, Qt::AlignRight);
    setTextAlignment(JobColumnFaults, Qt::AlignRight);
    setTextAlignment(JobColumnSizeIn, Qt::AlignRight);
    setTextAlignment(JobColumnSizeOut, Qt::AlignRight);
    updateText(job);
}


void JobListViewItem::updateText(const Job& job)
{
    const bool fileNameChanged(mJob.fileName() != job.fileName());

    mJob = job;

    setText(JobColumnID, QString::number(job.jobId()));
    if (JobListView* view = dynamic_cast<JobListView*>(treeWidget()))
    {
        setText(JobColumnClient, view->hostInfoManager()->nameForHost(job.client()));
        if (job.server())
            setText(JobColumnServer, view->hostInfoManager()->nameForHost(job.server()));
        else
            setText(JobColumnServer, QString());
    }
    setText(JobColumnState, job.stateAsString());
    setText(JobColumnReal, QString::number(job.real_msec));
    setText(JobColumnUser, QString::number(job.user_msec));
    setText(JobColumnFaults, QString::number(job.pfaults));
    setText(JobColumnSizeIn, tr("%1 KiB").arg(QString::number(job.in_uncompressed/1024, 'g', 2)));
    setText(JobColumnSizeOut, tr("%1 KiB").arg(QString::number(job.out_uncompressed/1024, 'g', 2)));

    if (fileNameChanged)
        updateFileName();
}


void JobListViewItem::updateFileName()
{
    JobListView* view = dynamic_cast<JobListView*>(treeWidget());
    if (!view)
        return;

    QChar separator = QDir::separator();

    QString fileName = mJob.fileName();

    const int numberOfFilePathParts = view->numberOfFilePathParts();
    if (numberOfFilePathParts > 0)
    {
        int counter = numberOfFilePathParts;
        int index = 0;
        do
        {
            index = fileName.lastIndexOf(separator, index - 1);
        }
        while (counter-- && (index > 0));

        if (index > 0)
            fileName = QString::fromLatin1("...") + fileName.mid(index);
    }
    else if (numberOfFilePathParts == 0)
    {
        fileName = fileName.mid(fileName.lastIndexOf(separator) + 1);
    }

    setText(JobColumnFilename, fileName);
}


inline int compare(unsigned int i1, unsigned int i2)
{
    if (i1 < i2)
        return -1;
    else if (i1 == i2)
        return 0;
    else
        return 1;
}


int JobListViewItem::compare(QTreeWidgetItem* item,
                              int column,
                              bool) const
{
    const JobListViewItem* first = this;
    const JobListViewItem* other = dynamic_cast<JobListViewItem*>(item);

    // Workaround a Qt4 regression: before the item creation is complete
    // compare() is called (insertItem() -> firstChild() -> enforceSortOrder())
    if (!other)
        return 0;

    switch (column)
    {
    case JobColumnID:
        return ::compare(first->mJob.jobId(), other->mJob.jobId());
    case JobColumnReal:
        return ::compare(first->mJob.real_msec, other->mJob.real_msec);
    case JobColumnUser:
        return ::compare(first->mJob.user_msec, other->mJob.user_msec);
    case JobColumnFaults:
        return ::compare(first->mJob.pfaults, other->mJob.pfaults);
    case JobColumnSizeIn:
        return ::compare(first->mJob.in_uncompressed, other->mJob.in_uncompressed);
    case JobColumnSizeOut:
        return ::compare(first->mJob.out_uncompressed, other->mJob.out_uncompressed);
    default:
        return first->text(column).compare(other->text(column));
    }
}


JobListView::JobListView(const HostInfoManager* manager,
                          QWidget* parent,
                          const char* name)
    : QTreeWidget(parent),
      m_hostInfoManager(manager),
      m_numberOfFilePathParts(2),
      m_expireDuration(-1),
      m_expireTimer(new QTimer(this))
{
    setObjectName(QLatin1String(name));
    qApp->setStyleSheet("QTreeView::branch { border-image: none; image: none }");

    QStringList headers;
    headers << tr("ID") << tr("Filename") << tr("Client") << tr("Server")
            << tr("State") << tr("Real") << tr("User") << tr("Faults")
            << tr("Size In") << tr("Size Out");

    setHeaderLabels(headers);
    const int nHeaders = headers.count();
    setColumnCount(nHeaders);

    // Auto adjust columns according to their content
    QHeaderView* headerView = header();
    for (int i = 0; i < nHeaders; ++i) {
        headerView->setResizeMode(i, QHeaderView::ResizeToContents);
    }
    headerView->setStretchLastSection(false);

    setAllColumnsShowFocus(true);

    setSortingEnabled(true);
    sortByColumn(JobColumnID, Qt::DescendingOrder);

    connect(m_expireTimer, SIGNAL(timeout()),
            this, SLOT(slotExpireFinishedJobs()));
}


void JobListView::update(const Job& job)
{
    ItemMap::iterator it = m_items.find(job.jobId());
    if (it == m_items.end())
        it = m_items.insert(job.jobId(), new JobListViewItem(this, job));
    else
        (*it)->updateText(job);

    const bool finished = (job.state() == Job::Finished) || (job.state() == Job::Failed);
    if (finished)
        expireItem(*it);
}


int JobListView::numberOfFilePathParts() const
{
    return m_numberOfFilePathParts;
}


void JobListView::setNumberOfFilePathParts(int number)
{
    if (number == m_numberOfFilePathParts)
        return;

    m_numberOfFilePathParts = number;

    for (ItemMap::const_iterator it(m_items.begin()),
                                  itEnd(m_items.end());
          it != itEnd; ++it)
        it.value()->updateFileName();
}


bool JobListView::isClientColumnVisible() const
{
    return columnWidth(JobColumnClient);
}


void JobListView::setClientColumnVisible(bool visible)
{
    if (visible == isClientColumnVisible())
        return;

    if (visible)
    {
        setColumnWidth(JobColumnClient, 50); // at least the user can see it again
    }
    else
    {
        setColumnWidth(JobColumnClient, 0);
    }
}


bool JobListView::isServerColumnVisible() const
{
    return columnWidth(JobColumnServer);
}


void JobListView::setServerColumnVisible(bool visible)
{
    if (visible == isServerColumnVisible())
        return;

    if (visible)
    {
        setColumnWidth(JobColumnServer, 50); // at least the user can see it again
    }
    else
    {
        setColumnWidth(JobColumnServer, 0);
    }
}


int JobListView::expireDuration() const
{
    return m_expireDuration;
}


void JobListView::setExpireDuration(int duration)
{
    m_expireDuration = duration;
}


void JobListView::clear()
{
    m_expireTimer->stop();

    m_items.clear();
    m_finishedJobs.clear();

    QTreeWidget::clear();
}


void JobListView::slotExpireFinishedJobs()
{
    const uint currentTime = QDateTime::currentDateTime().toTime_t();

    // this list is sorted by the age of the finished jobs, the oldest is the first
    // so we've to find the first job which isn't old enough to expire
    FinishedJobs::iterator it = m_finishedJobs.begin();
    for (const FinishedJobs::iterator itEnd = m_finishedJobs.end(); it != itEnd; ++it)
    {
        if (currentTime - (*it).first < (uint)m_expireDuration)
            break;

        removeItem((*it).second);
    }

    m_finishedJobs.erase(m_finishedJobs.begin(), it);

    if (m_finishedJobs.empty())
        m_expireTimer->stop();
}


void JobListView::expireItem(JobListViewItem* item)
{
    if (m_expireDuration == 0)
    {
        removeItem(item);
    }
    else if (m_expireDuration > 0)
    {
        m_finishedJobs.push_back(FinishedJob(QDateTime::currentDateTime().toTime_t(), item));

        if (!m_expireTimer->isActive())
            m_expireTimer->start(1000);
    }
}


void JobListView::removeItem(JobListViewItem* item)
{
    m_items.remove(item->job().jobId());
    delete item;
}

#include "joblistview.moc"
