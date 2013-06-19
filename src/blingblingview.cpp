#include "blingblingview.h"

BlingBlingView::BlingBlingView(HostInfoManager* hostInfoManager)
    : StatusView(hostInfoManager)
{
    m_tick.setSingleShot(false);
    connect(&m_tick, SIGNAL(timeout()), this, SLOT(update()));
    m_tick.start(25);
}

void BlingBlingView::update(const Job &job)
{

}

void BlingBlingView::initializeGL()
{
    glEnable(GL_MULTISAMPLE_ARB);
}

void BlingBlingView::resizeGL(int w, int h)
{
}

void BlingBlingView::paintGL()
{
}

