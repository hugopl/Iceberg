/*
    This file is part of Icecream.

    Copyright (c) 2003 Frerich Raabe <raabe@kde.org>
    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>
    Copyright (c) 2011 Hugo Parente Lima <hugo.pl@gmail.com>
    Copyright (c) 2012 Anselmo L. S. Melo <anselmolsm@gmail.com>

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

#include "starview.h"

#include "hostinfo.h"

#include <QBoxLayout>
#include <QToolTip>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QRegExp>
#include <QCheckBox>
#include <QDir>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDialogButtonBox>
#include <QSettings>
#include <QDebug>
#include <cmath>

static bool s_shouldSuppressDomainName = false;
static bool s_shouldHideAllText = false;

StarViewConfigDialog::StarViewConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setContentsMargins(10, 10, 10, 10);
    QBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel(tr("Number of nodes per ring:"), this);
    topLayout->addWidget(label);

    QBoxLayout *nodesLayout = new QHBoxLayout();
    topLayout->addLayout(nodesLayout);

    QSettings settings;
    const int nodesPerRing = settings.value("StarView/NodesPerRing", 25).toInt();
    const QString archFilter = settings.value("StarView/ArchFilter", "").toString();
    s_shouldSuppressDomainName = settings.value("StarView/SuppressDomainName", false).toBool();
    s_shouldHideAllText = settings.value("StarView/HideAllText", false).toBool();

    m_nodesPerRingSlider = new QSlider(Qt::Horizontal, this);
    m_nodesPerRingSlider->setMinimum(1);
    m_nodesPerRingSlider->setMaximum(50);
    m_nodesPerRingSlider->setSingleStep(1);
    m_nodesPerRingSlider->setValue(nodesPerRing);
    nodesLayout->addWidget(m_nodesPerRingSlider);

    m_nodesPerRingLabel = new QLabel(QString::number(nodesPerRing), this);
    nodesLayout->addWidget(m_nodesPerRingLabel);

    label = new QLabel(tr("Architecture filter:"), this);
    m_archFilterEdit = new QLineEdit(this);
    m_hideAllTextCheckBox = new QCheckBox(tr("Hide all text"), this);
    m_suppressDomainNameCheckBox = new QCheckBox(tr("Suppress domain name"), this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal);

    topLayout->addWidget(label);
    topLayout->addWidget(m_archFilterEdit);
    topLayout->addWidget(m_hideAllTextCheckBox);
    topLayout->addWidget(m_suppressDomainNameCheckBox);
    topLayout->addWidget(buttonBox);

    connect(m_nodesPerRingSlider, SIGNAL(valueChanged(int)), SIGNAL(configChanged()));
    connect(m_nodesPerRingSlider, SIGNAL(valueChanged(int)), SLOT(slotNodesPerRingChanged(int)));
    connect(m_archFilterEdit, SIGNAL(textChanged(const QString &)), SIGNAL(configChanged()));
    connect(m_hideAllTextCheckBox, SIGNAL(toggled(bool)), SLOT(slotHideAllText(bool)));
    connect(m_suppressDomainNameCheckBox, SIGNAL(toggled(bool)), SLOT(slotSuppressDomainName(bool)));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(hide()));

    m_archFilterEdit->setText(archFilter);

    if (s_shouldHideAllText)
        m_hideAllTextCheckBox->setChecked(s_shouldHideAllText);
    else
        m_suppressDomainNameCheckBox->setChecked(s_shouldSuppressDomainName);
}

void StarViewConfigDialog::slotNodesPerRingChanged(int nodes)
{
    m_nodesPerRingLabel->setText(QString::number(nodes));
}

void StarViewConfigDialog::setMaxNodes(int maxNodes)
{
    m_nodesPerRingSlider->setMaximum(maxNodes + 1);
}

int StarViewConfigDialog::nodesPerRing()
{
    return m_nodesPerRingSlider->value();
}

QString StarViewConfigDialog::archFilter()
{
    return m_archFilterEdit->text();
}

bool StarViewConfigDialog::shouldSuppressDomainName() const
{
    return m_suppressDomainNameCheckBox->isChecked();
}

bool StarViewConfigDialog::shouldHideAllText() const
{
    return m_hideAllTextCheckBox->isChecked();
}

void StarViewConfigDialog::slotSuppressDomainName(bool b)
{
    s_shouldSuppressDomainName = b;
    emit configChanged();
}

void StarViewConfigDialog::slotHideAllText(bool b)
{
    s_shouldHideAllText = b;
    m_suppressDomainNameCheckBox->setEnabled(!b);
    emit configChanged();
}

HostItem::HostItem(const QString &text, QGraphicsScene *canvas, HostInfoManager *m)
    : QGraphicsItemGroup(0),
      m_hostInfo(0),
      m_hostInfoManager(m),
      m_stateItem(0)
{
    canvas->addItem(this);
    init();

    m_textItem->setText(text);

    updateName();
}

HostItem::HostItem(HostInfo *hostInfo, QGraphicsScene *canvas, HostInfoManager *m)
    : QGraphicsItemGroup(0),
      m_hostInfo(hostInfo),
      m_hostInfoManager(m),
      m_stateItem(0)
{
    canvas->addItem(this);
    init();
}

HostItem::~HostItem()
{
}

void HostItem::init()
{
    m_baseWidth = 0;
    m_baseHeight = 0;

    m_boxItem = new QGraphicsEllipseItem(this);
    m_boxItem->setZValue(80);
    m_boxItem->setPen(QPen(Qt::NoPen));

    m_textItem = new QGraphicsSimpleTextItem(this);
    m_textItem->setZValue(100);

    setHostColor(QColor(200, 200, 200));

    m_isActiveClient = false;
    m_isCompiling = false;

    m_client = 0;
}

void HostItem::setHostColor(const QColor &color)
{
    m_boxItem->setBrush(color);
    m_textItem->setBrush(StatusView::textColor(color));
}

QString HostItem::hostName() const
{
    return m_hostInfo->name();
}

void HostItem::updateName()
{
    if (s_shouldHideAllText)
        m_textItem->setText("");
    else if (m_hostInfo) {
        QString s = m_hostInfo->name();
        if (s_shouldSuppressDomainName) {
            int l = s.indexOf('.');
            if (l>0)
                s.truncate(l);
        }
        m_textItem->setText(s);
    } else // Scheduler.
        m_textItem->setText(m_hostInfoManager->networkName());

    QRectF r = m_textItem->boundingRect();
    m_baseWidth = r.width() + 10 ;
    m_baseHeight = r.height() + 10 ;

    m_boxItem->setRect(-5, -5, m_baseWidth, m_baseHeight);

    updateHalos();
}

void HostItem::setCenterPos(double x, double y)
{
    // move all items (also the sub items)
    setPos(x - m_textItem->boundingRect().width()/2, y - m_textItem->boundingRect().height()/2);
    //  setPos(x, y);
}

void HostItem::update(const Job &job)
{
    setIsCompiling(job.state() == Job::Compiling);
    setClient(job.client());

    if (job.state() == Job::WaitingForCS)
        return;

    bool finished = job.state() == Job::Finished ||
                    job.state() == Job::Failed;

    JobList::Iterator it = m_jobs.find(job.jobId());
    bool newJob = (it == m_jobs.end());

    if (newJob && finished)
        return;
    if (!newJob && !finished)
        return;

    if (newJob) {
        m_jobs.insert(job.jobId(), job);
        createJobHalo(job);
    } else if (finished) {
        deleteJobHalo(job);
        m_jobs.erase(it);
    }
}

void HostItem::createJobHalo(const Job &job)
{
    QGraphicsEllipseItem *halo = new QGraphicsEllipseItem(
        centerPosX(), centerPosY(), m_baseWidth, m_baseHeight,
        this);

    halo->setZValue(70 - m_jobHalos.size());
    halo->setPen(QPen(Qt::NoPen));
    halo->show();

    m_jobHalos.insert(job, halo);

    updateHalos();
}

void HostItem::deleteJobHalo(const Job &job)
{
    QMap<Job,QGraphicsEllipseItem*>::Iterator it = m_jobHalos.find(job);
    if (it == m_jobHalos.end())
        return;

    QGraphicsEllipseItem *halo = *it;
    delete halo;
    m_jobHalos.erase(it);

    updateHalos();
}

void HostItem::updateHalos()
{
    int count = 1;

    QMap<Job,QGraphicsEllipseItem*>::Iterator it;
    for(it = m_jobHalos.begin(); it != m_jobHalos.end(); ++it) {
        QGraphicsEllipseItem *halo = it.value();
        halo->setRect(halo->x() - 5 - count * 3,
                      halo->y() - 5 - count * 3,
                      m_baseWidth + count * 6,
                      m_baseHeight + count * 6);
        halo->setBrush(m_hostInfoManager->hostColor(it.key().client()));
        halo->setPen(Qt::NoPen);
        ++count;
    }
}

StarView::StarView(HostInfoManager *m, QWidget *parent)
    : QWidget(parent), StatusView(m)
{
    m_configDialog = new StarViewConfigDialog(this);
    connect(m_configDialog, SIGNAL(configChanged()), SLOT(slotConfigChanged()));

    m_canvas = new QGraphicsScene (this);
    m_canvas->setSceneRect(0, 0, width(), height());

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    m_canvasView = new QGraphicsView(m_canvas, this);
    m_canvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_canvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_canvasView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    layout->addWidget(m_canvasView);

    m_schedulerItem = new HostItem("", m_canvas, hostInfoManager());
    m_schedulerItem->setZValue(150);
    centerSchedulerItem();
    m_schedulerItem->show();
    createKnownHosts();
}

void StarView::update(const Job &job)
{
    if (job.state() == Job::WaitingForCS) {
        drawNodeStatus();
        return;
    }

    unsigned int hostid = processor(job);
    if (!hostid)
        return;

    HostItem *hostItem = findHostItem(hostid);
    if (!hostItem)
        return;

    hostItem->update(job);

    bool finished = job.state() == Job::Finished || job.state() == Job::Failed;

    QMap<unsigned int, HostItem *>::Iterator it;
    it = m_jobMap.find(job.jobId());
    if (it != m_jobMap.end()) {
        (*it)->update(job);
        if (finished) {
            m_jobMap.erase(it);
            unsigned int clientid = job.client();
            HostItem *clientItem = findHostItem(clientid);
            if (clientItem)
                clientItem->setIsActiveClient(false);
        }
        drawNodeStatus();
        return;
    }

    if (!finished)
        m_jobMap.insert(job.jobId(), hostItem);

    if (job.state() == Job::Compiling) {
        unsigned int clientid = job.client();
        HostItem *clientItem = findHostItem(clientid);
        if (clientItem) {
            clientItem->setClient(clientid);
            clientItem->setIsActiveClient(true);
        }
    }

    drawNodeStatus();
}

HostItem *StarView::findHostItem(unsigned int hostid)
{
    HostItem *hostItem = 0;
    QMap<unsigned int, HostItem*>::iterator it = m_hostItems.find(hostid);
    if (it != m_hostItems.end())
        hostItem = it.value();
    return hostItem;
}

void StarView::checkNode(unsigned int hostid)
{
    if (!hostid || !filterArch(hostid))
        return;

    HostItem *hostItem = findHostItem(hostid);
    if (!hostItem) {
        hostItem = createHostItem(hostid);
        arrangeHostItems();
    }
}

void StarView::removeNode(unsigned int hostid)
{
    HostItem *hostItem = findHostItem(hostid);
    if (hostItem && hostItem->hostInfo()->isOffline())
        removeItem(hostItem);
}

void StarView::forceRemoveNode(unsigned int hostid)
{
    HostItem *hostItem = findHostItem(hostid);
    if (hostItem)
        removeItem(hostItem);
}

void StarView::removeItem(HostItem *hostItem)
{
    m_hostItems.remove(hostItem->hostInfo()->id());

    QList<unsigned int> obsoleteJobs;

    QMap<unsigned int,HostItem *>::Iterator it;
    for(it = m_jobMap.begin(); it != m_jobMap.end(); ++it) {
        if (it.value() == hostItem)
            obsoleteJobs.append(it.key());
    }

    QList<unsigned int>::ConstIterator it2;
    for(it2 = obsoleteJobs.constBegin(); it2 != obsoleteJobs.constEnd(); ++it2)
        m_jobMap.remove(*it2);

    delete hostItem->stateItem();
    delete hostItem;

    arrangeHostItems();
}

void StarView::updateSchedulerState(bool online)
{
    QString txt = online ? hostInfoManager()->networkName() : "???";
    delete m_schedulerItem;

    if (!online) {
        QMap<unsigned int,HostItem *>::ConstIterator it;
        for(it = m_hostItems.constBegin(); it != m_hostItems.constEnd(); ++it)
            delete *it;

        m_hostItems.clear();
        m_jobMap.clear();
    }

    m_schedulerItem = new HostItem(txt, m_canvas, hostInfoManager());
    m_schedulerItem->setZValue(100);
    m_schedulerItem->show();
    centerSchedulerItem();
}

QWidget *StarView::widget()
{
    return this;
}

void StarView::resizeEvent(QResizeEvent *)
{
    m_canvas->setSceneRect(0, 0, width(), height());
    centerSchedulerItem();
    arrangeHostItems();
    drawNodeStatus();
}

bool StarView::event (QEvent* e)
{
    if (e->type() != QEvent::ToolTip)
        return QWidget::event(e);

    QPoint p (static_cast<QHelpEvent*>(e)->pos());

    HostItem *item = 0;
    QGraphicsItem* graphicsItem = m_canvasView->itemAt(p);
    if (graphicsItem)
        item = dynamic_cast<HostItem*>(graphicsItem->parentItem());
    if (item) {
        HostInfo *hostInfo = item->hostInfo();
        const QPoint gp(static_cast<QHelpEvent*>(e)->globalPos());
        const QRect itemRect = m_canvasView->mapFromScene(graphicsItem->sceneBoundingRect()).boundingRect();
        if (hostInfo) {
            QToolTip::showText(gp+QPoint(5,5),
                           "<p><table><tr><td>"
                           "<img align=\"right\" source=\":/images/computer.png\"><br><b>" + item->hostName() +
                           "</b><br>" +

                           "<table>" +
                           "<tr><td>" + tr("IP:") + "</td><td>" + hostInfo->ip()
                           + "</td></tr>" +
                           "<tr><td>" + tr("Platform:") + "</td><td>" +
                           hostInfo->platform() + "</td></tr>" +
                           "<tr><td>" + tr("MaxJobs:") + "</td><td>" +
                           QString::number(hostInfo->maxJobs()) + "</td></tr>" +
                           "<tr><td>" + tr("Speed:") + "</td><td>" +
                           QString::number(hostInfo->serverSpeed()) + "</td></tr>" +
                           "</table>"

                           "</td></tr></table></p>", this, itemRect);
        } else {
            QToolTip::showText(gp+QPoint(5,5),
                           "<p><table><tr><td>"
                           "<img align=\"right\" source=\":/images/computer.png\"><br><b>" + tr("Scheduler") + "</b><br/>"
                           "<table>" +
                           "<tr><td>" + tr("Host: %1").arg(QString(hostInfoManager()->schedulerName())) + "</td></tr>" +
                           "<tr><td>" + tr("Network name: %1").arg(QString(hostInfoManager()->networkName())) + "</td></tr>" +
                           "</table>"
                           "</td></tr></table></p>", this, itemRect);
        }
    } else {
         QToolTip::hideText();
    }
    return QWidget::event(e);
}

void StarView::centerSchedulerItem()
{
    m_schedulerItem->setCenterPos(width() / 2, height() / 2);
}

void StarView::slotConfigChanged()
{
    HostInfoManager::HostMap hostMap = hostInfoManager()->hostMap();
    HostInfoManager::HostMap::ConstIterator it;
    for(it = hostMap.constBegin(); it != hostMap.constEnd(); ++it) {
        if (filterArch(*it))
            checkNode(it.key());
        else
            forceRemoveNode(it.key());
    }

    arrangeHostItems();
    m_schedulerItem->updateName();
    centerSchedulerItem();

    //write settings
    QSettings settings;
    settings.setValue("StarView/NodesPerRing", m_configDialog->nodesPerRing());
    settings.setValue("StarView/ArchFilter", m_configDialog->archFilter());
    settings.setValue("StarView/HideAllText", m_configDialog->shouldHideAllText());
    settings.setValue("StarView/SuppressDomainName", m_configDialog->shouldSuppressDomainName());
}

void StarView::arrangeHostItems()
{
    int count = m_hostItems.count();
    int nodesPerRing = m_configDialog->nodesPerRing();
    int ringCount = int(count / nodesPerRing) + 1;

    double radiusFactor = 2.5;
    if (s_shouldSuppressDomainName)
        radiusFactor = 4;

    const int xRadius = qRound(m_canvas->width() / radiusFactor);
    const int yRadius = qRound(m_canvas->height() / radiusFactor);

    const double step = 2 * M_PI / count;

    double angle = 0.0;
    int i = 0;
    QMap<unsigned int, HostItem*>::ConstIterator it;
    for (it = m_hostItems.constBegin(); it != m_hostItems.constEnd(); ++it) {
        double factor = 1 - (1.0 / (ringCount + 1)) * (i % ringCount);

        double xr = xRadius * factor;
        double yr = yRadius * factor;

        HostItem *item = it.value();

        item->updateName();

        item->setCenterPos(width() / 2 + cos(angle) * xr,
                           height() / 2 + sin(angle) * yr);

        angle += step;
        ++i;
    }
}

HostItem *StarView::createHostItem(unsigned int hostid)
{
    HostInfo *i = hostInfoManager()->find(hostid);

    if (!i || i->isOffline() || i->name().isEmpty())
        return 0;

    HostItem *hostItem = new HostItem(i, m_canvas, hostInfoManager());
    hostItem->setHostColor(hostColor(hostid));
    m_hostItems.insert(hostid, hostItem);
    hostItem->show();

    arrangeHostItems();

    if (m_hostItems.count() > 25)
        m_configDialog->setMaxNodes(m_hostItems.count());

    return hostItem;
}

void StarView::drawNodeStatus()
{
    QMap<unsigned int, HostItem*>::ConstIterator it;
    for (it = m_hostItems.constBegin(); it != m_hostItems.constEnd(); ++it)
        drawState(*it);
}

void StarView::drawState(HostItem *node)
{
    delete node->stateItem();
    QGraphicsLineItem *newItem = 0;

    unsigned int client = node->client();
    QColor color = client ? hostColor(client) : Qt::green;

    if (node->isCompiling() || node->isActiveClient()) {
        newItem = new QGraphicsLineItem(qRound(node->centerPosX()),
                                        qRound(node->centerPosY()),
                                        qRound(m_schedulerItem->centerPosX()),
                                        qRound(m_schedulerItem->centerPosY()));
        if (node->isCompiling()) {
            newItem->setPen(QPen(color, 0));
            newItem->setZValue(-301);
        } else if (node->isActiveClient()) {
            newItem->setPen(QPen(color, 1, Qt::DashLine));
            newItem->setZValue(-300);
        }
        m_canvas->addItem(newItem);
    }

    node->setStateItem(newItem);
}

void StarView::createKnownHosts()
{
    HostInfoManager::HostMap hosts = hostInfoManager()->hostMap();

    HostInfoManager::HostMap::ConstIterator it;
    for(it = hosts.constBegin(); it != hosts.constEnd(); ++it) {
        unsigned int id = (*it)->id();
        if (!findHostItem(id))
            createHostItem(id);
    }
}

void StarView::configureView()
{
    m_configDialog->show();
    m_configDialog->raise();
}

bool StarView::filterArch(unsigned int hostid)
{
    HostInfo *i = hostInfoManager()->find(hostid);
    if (!i) {
        qWarning() << "No HostInfo for id " << hostid << endl;
        return false;
    }

    return filterArch(i);
}

bool StarView::filterArch(HostInfo *i)
{
    if (m_configDialog->archFilter().isEmpty())
        return true;

    QRegExp regExp(m_configDialog->archFilter());
    if (regExp.indexIn(i->platform()) >= 0)
        return true;

    return false;
}

#include "starview.moc"
