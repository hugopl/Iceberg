#ifndef BlingBlingView_h
#define BlingBlingView_h

#include <QGLWidget>
#include <QTimer>
#include "statusview.h"

class BlingBlingView : public QGLWidget, public StatusView
{
public:
    BlingBlingView(HostInfoManager*);
    virtual void update(const Job &job);
    virtual QWidget* widget() { return this; }
    virtual QString id() const { return "blingbling"; }
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    QTimer m_tick;
};

#endif
