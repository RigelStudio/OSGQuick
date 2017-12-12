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

#include "osgitem.hpp"

#include <QtGui/QOpenGLFramebufferObject>

#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>

#include <cassert>

////////////////////////////////////////////////////////////////////////////////
//                            OSGItem::PendingEvents                          //
////////////////////////////////////////////////////////////////////////////////

class OSGItem::PendingEvents : public osgGA::EventQueue
{
public:
    PendingEvents()
        : osgGA::EventQueue(osgGA::GUIEventAdapter::Y_INCREASING_DOWNWARDS)
    {}
};

////////////////////////////////////////////////////////////////////////////////
//                              class RendererImpl                            //
////////////////////////////////////////////////////////////////////////////////

namespace
{

class RendererImpl : public QQuickFramebufferObject::Renderer
{
    // Frame-buffer based implementation of osgViewer::GraphicsWindow
    class FBWindow : public osgViewer::GraphicsWindowEmbedded
    {
    public:
        explicit FBWindow(const RendererImpl* renderer)
            : osgViewer::GraphicsWindowEmbedded(0, 0, 0, 0)
            , renderer_(*renderer)
        {
        }

        virtual bool makeCurrentImplementation()
        {
            assert ( renderer_.framebufferObject() != NULL );
            return renderer_.framebufferObject()->bind();
        }

        virtual bool releaseContextImplementation()
        {
            assert ( renderer_.framebufferObject() != NULL );
            return renderer_.framebufferObject()->release();
        }

    private:
        const RendererImpl& renderer_;
    };

public:
    explicit RendererImpl(const OSGItem* osgItem)
        : osgItem_(*osgItem)
    {
        if ( osgItem_.model().get() )
            init();
    }

    void render()
    {
        if ( viewer_.get() )
        {
            // Without this line the model is not displayed in the second
            // and subsequent frames.
            QOpenGLContext::currentContext()->functions()->glUseProgram(0);

            if ( sizeHasChanged() )
            {
                updateSize();
            }

            viewer_->frame();
        }
    }

    const OSGItem& osgItem() const
    {
        return osgItem_;
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject* item)
    {
        assert(item == &osgItem_);

        OSGItem& osgItem = static_cast<OSGItem&>(*item);

        if ( osgItem_.model() && !viewer_ )
        {
            init();
        }

        if ( viewer_.get() )
        {
            if ( viewer_->getSceneData() != osgItem.model() )
            {
                viewer_->getEventQueue()->clear();
                viewer_->setSceneData(osgItem.model().get());
                viewer_->home();
            }
            else
            {
                osgGA::EventQueue::Events events;
                osgItem.pendingEvents().takeEvents(events);
                viewer_->getEventQueue()->appendEvents(events);
            }
        }
    }

private: // functions

    void init()
    {
        viewer_ = new osgViewer::Viewer;
        viewer_->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
        viewer_->setSceneData(osgItem().model().get());
        viewer_->setCameraManipulator(new osgGA::TrackballManipulator);
        viewer_->addEventHandler(new osgViewer::StatsHandler);
        viewer_->addEventHandler(new osgViewer::HelpHandler);

        window_ = new FBWindow(this);
        updateSize();

        osg::Camera* camera = viewer_->getCamera();
        camera->setGraphicsContext( window_.get() );

        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
    }

    bool sizeHasChanged() const
    {
        const osg::GraphicsContext::Traits& t = *window_->getTraits();
        return t.width != osgItem().width() || t.height != osgItem().height();
    }

    void updateSize()
    {
        window_->resized(0, 0, osgItem().width(), osgItem().height());
        osg::Camera& camera = *viewer_->getCamera();
        const osg::GraphicsContext::Traits& t = *window_->getTraits();
        camera.setViewport( new osg::Viewport(0, 0, t.width, t.height) );
        const double aspectRatio = static_cast<double>(t.width)/static_cast<double>(t.height);
        camera.setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 10000.0f );
    }

private: // data
    const OSGItem& osgItem_;
    osg::ref_ptr<osgViewer::Viewer> viewer_;
    osg::observer_ptr<osgViewer::GraphicsWindow> window_;
};

int button(const QMouseEvent& event)
{
    switch ( event.button() )
    {
        case Qt::LeftButton:  return 1;
        case Qt::MidButton:   return 2;
        case Qt::RightButton: return 3;
        default:              return 0;
    }
}

} // unnamed namespace

////////////////////////////////////////////////////////////////////////////////
//                                 OSGItem                                    //
////////////////////////////////////////////////////////////////////////////////

OSGItem::OSGItem(ModelPtr model)
    : model_(model)
    , pendingEvents_(new PendingEvents)
{
    setAcceptedMouseButtons(Qt::AllButtons);
}

OSGItem::~OSGItem()
{
}

OSGItem::ModelPtr
OSGItem::model() const
{
    return model_;
}

void
OSGItem::setModel(ModelPtr model)
{
    model_ = model;
    update();
    emit modelChanged();
}

OSGItem::PendingEvents&
OSGItem::pendingEvents()
{
    return *pendingEvents_;
}

QQuickFramebufferObject::Renderer*
OSGItem::createRenderer() const
{
    return new RendererImpl(this);
}

QSGNode*
OSGItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* upnData)
{
    QSGNode* node = QQuickFramebufferObject::updatePaintNode(oldNode, upnData);
    QSGSimpleTextureNode& textureNode = dynamic_cast<QSGSimpleTextureNode&>(*node);
    textureNode.setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
    return node;
}

void
OSGItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    pendingEvents_->windowResize(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
    QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
}

void
OSGItem::mousePressEvent(QMouseEvent *event)
{
    pendingEvents_->mouseButtonPress(event->x(), event->y(), button(*event));

    update();
}

void
OSGItem::mouseMoveEvent(QMouseEvent *event)
{
    pendingEvents_->mouseMotion(event->x(), event->y());

    update();
}

void
OSGItem::mouseReleaseEvent(QMouseEvent *event)
{
    pendingEvents_->mouseButtonRelease(event->x(), event->y(), button(*event));

    update();
}

void
OSGItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    pendingEvents_->mouseDoubleButtonPress(event->x(), event->y(), button(*event));

    update();
}

void
OSGItem::wheelEvent(QWheelEvent* event)
{
    if ( event->delta() > 0 )
        pendingEvents_->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
    else
        pendingEvents_->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);

    update();
}

void
OSGItem::keyPressEvent(QKeyEvent* event)
{
    const std::string text = event->text().toStdString();
    if ( text.size() == 1 )
    {
        pendingEvents_->keyPress(text[0]);
        update();
    }
}
