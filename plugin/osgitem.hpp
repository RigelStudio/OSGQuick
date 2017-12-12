/****************************************************************************
**
** Copyright (c) 2016, Leon Manukyan
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in
**    the documentation and/or other materials provided with the
**    distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#ifndef QTQUICK2_OSGITEM_HPP
#define QTQUICK2_OSGITEM_HPP

#include <QtQuick/QQuickFramebufferObject>

#include <osg/Node>

#include <memory>

class OSGItem : public QQuickFramebufferObject
{
    Q_OBJECT

public: // types
    typedef osg::ref_ptr<osg::Node> ModelPtr;
    class PendingEvents;

public: // functions
    explicit OSGItem(ModelPtr model = ModelPtr());
    ~OSGItem();

    Renderer *createRenderer() const;

    ModelPtr model() const;
    void setModel(ModelPtr model);

    PendingEvents& pendingEvents();

signals:
    void modelChanged();

private: // overrides
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_OVERRIDE;

    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* );
    void keyPressEvent(QKeyEvent*);

private: // data
    ModelPtr model_;
    const std::unique_ptr<PendingEvents> pendingEvents_;
};

#endif // QTQUICK2_OSGITEM_HPP
