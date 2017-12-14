#include "RenderView.h"

#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QSGSimpleTextureNode>
#include <QLoggingCategory>
#include <QDebug>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osg/Camera>


namespace
{

QLoggingCategory lc("RenderView");

class RendererOpenSceneGraph : public QQuickFramebufferObject::Renderer
{
    class FBWindow : public osgViewer::GraphicsWindowEmbedded
    {
    public:
        explicit FBWindow(const RendererOpenSceneGraph *renderer)
            : osgViewer::GraphicsWindowEmbedded(0, 0, 0, 0)
            , _renderer(*renderer)
        {
        }

        virtual bool makeCurrentImplementation() Q_DECL_OVERRIDE
        {
            return _renderer.framebufferObject()->bind();
        }

        virtual bool releaseContextImplementation() Q_DECL_OVERRIDE
        {
            return _renderer.framebufferObject()->release();
        }

    private:
        const RendererOpenSceneGraph &_renderer;
    };

public:
    explicit RendererOpenSceneGraph(const RenderView *renderView)
        : QQuickFramebufferObject::Renderer()
        , _renderView(*renderView)
    {
    }

    virtual void render() Q_DECL_OVERRIDE
    {
        if (_viewer.valid())
        {
            QOpenGLContext::currentContext()->functions()->glUseProgram(0);
            

            if (sizeHasChanged())
                updateSize();

            // GL_TEXTURE_BIT
           
            _viewer->frame();
            
        }
		update();
    }

    virtual QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) Q_DECL_OVERRIDE
    {
        QOpenGLFramebufferObjectFormat fmt;
        fmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        fmt.setSamples(4);
        return new QOpenGLFramebufferObject(size, fmt);
    }

    virtual void synchronize(QQuickFramebufferObject* item) Q_DECL_OVERRIDE
    {
        RenderView& renderView = static_cast<RenderView&>(*item);

        if (!_viewer.valid())
            init();

        if (_viewer.valid())
        {
            if (_viewer->getSceneData() != renderView.model())
            {
                _viewer->getEventQueue()->clear();
                _viewer->setSceneData(renderView.model());
                _viewer->home();
            }
            else
            {
                osgGA::EventQueue::Events events;
                renderView.pendingEvents().takeEvents(events);
                _viewer->getEventQueue()->appendEvents(events);
            }
        }
    }
private:
    void init()
    {
        _viewer = new osgViewer::Viewer;
        _viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
        _viewer->setSceneData(_renderView.model());
        _viewer->setCameraManipulator(new osgGA::TrackballManipulator);
        _viewer->addEventHandler(new osgViewer::StatsHandler);
        _viewer->addEventHandler(new osgViewer::HelpHandler);

        _window = new FBWindow(this);
        updateSize();

        osg::Camera &camera = *_viewer->getCamera();
        camera.setGraphicsContext(_window.get());
    }

    bool sizeHasChanged() const
    {
        const osg::GraphicsContext::Traits &t = *_window->getTraits();
        return t.width != _renderView.width() || t.height != _renderView.height();
    }

    void updateSize()
    {
        _window->resized(0, 0, _renderView.width(), _renderView.height());
        osg::Camera &camera = *_viewer->getCamera();
        const osg::GraphicsContext::Traits &t = *_window->getTraits();
        camera.setViewport(new osg::Viewport(0, 0, t.width, t.height));
        const double aspectRatio = static_cast<double>(t.width)/static_cast<double>(t.height);
        camera.setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 10000.0f );
    }

    const RenderView &_renderView;
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    osg::observer_ptr<osgViewer::GraphicsWindow> _window;
};

static int button(const QMouseEvent& event)
{
    switch ( event.button() )
    {
        case Qt::LeftButton:  return 1;
        case Qt::MidButton:   return 2;
        case Qt::RightButton: return 3;
        default:              return 0;
    }
}

}

RenderView::RenderView()
    : QQuickFramebufferObject()
    , _pendingEvents(new osgGA::EventQueue)
{
    setAcceptedMouseButtons(Qt::AllButtons);
}

RenderView::~RenderView()
{
}

QQuickFramebufferObject::Renderer* RenderView::createRenderer() const
{
    return new RendererOpenSceneGraph(this);
}

osg::Node *RenderView::model() const
{
    return _model;
}

void RenderView::setModel(osg::Node *node)
{
    if (node == _model)
        return;

    _model = node;
    update();
    emit modelChanged(node);
}

osgGA::EventQueue &RenderView::pendingEvents()
{
    return *_pendingEvents;
}

QSGNode* RenderView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *upnData)
{
    QSGNode *node = QQuickFramebufferObject::updatePaintNode(oldNode, upnData);
    QSGSimpleTextureNode &textureNode = static_cast<QSGSimpleTextureNode&>(*node);

    textureNode.setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);

    return node;
}

void RenderView::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    _pendingEvents->windowResize(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
    QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
}

void RenderView::mousePressEvent(QMouseEvent *event)
{
    _pendingEvents->mouseButtonPress(event->x(), event->y(), button(*event));
    update();
}

void RenderView::mouseMoveEvent(QMouseEvent *event)
{
    _pendingEvents->mouseMotion(event->x(), event->y(), button(*event));
    update();
}

void RenderView::mouseReleaseEvent(QMouseEvent *event)
{
    _pendingEvents->mouseButtonRelease(event->x(), event->y(), button(*event));
    update();
}

void RenderView::mouseDoubleClickEvent(QMouseEvent *event)
{
    _pendingEvents->mouseDoubleButtonPress(event->x(), event->y(), button(*event));
    update();
}

void RenderView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        _pendingEvents->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
    else
        _pendingEvents->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);

    update();
}

void RenderView::keyPressEvent(QKeyEvent *event)
{
    const std::string text = event->text().toStdString();
    if (text.size() == 1)
    {
        _pendingEvents->keyPress(text[0]);
        update();
    }
}
