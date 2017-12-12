#include "OSGItem.h"
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "OSGRender.h"
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include "RigelOSG.h"

OSGItem::OSGItem(QQuickItem *parent/* = Q_NULLPTR*/)
	:QQuickFramebufferObject(parent)
{
	std::cout << "OSGItem::OSGItem()" << std::endl;
	m_pRoot = nullptr;
	m_pCamera = nullptr;
	m_pViewer = nullptr;
	m_pGraphWindow = nullptr;
	setFlag(ItemHasContents, true);
	setAcceptedMouseButtons(Qt::AllButtons);
	//setTextureFollowsItemSize(true);
	initOSG();
}


OSGItem::~OSGItem()
{
	delete m_pViewer;
}

osg::Camera* OSGItem::getCamera()
{
	return m_pCamera;
}

osgViewer::Viewer* OSGItem::getViewer()
{
	return m_pViewer;
}

osg::Group* OSGItem::getSceneData()
{
	return m_pRoot;
}

void OSGItem::slotHome()
{
	m_pViewer->home();
}

QQuickFramebufferObject::Renderer* OSGItem::createRenderer() const
{
	std::cout << "OSGItem::createRenderer()" << std::endl;
	return new OSGRender(m_pViewer);
}

void OSGItem::initOSG()
{
	m_pViewer = new osgViewer::Viewer;
	m_pRoot = new osg::Group;
	osg::Node* node = osgDB::readNodeFile("D:/WorkSpace/Rigel/OSGQuick/cow.osg");
	m_pRoot->addChild(node);
	m_pViewer->setSceneData(m_pRoot);
	m_pCamera = m_pViewer->getCamera();
	
	m_pViewer->setCameraManipulator(new osgGA::TrackballManipulator);
	m_pViewer->addEventHandler(new osgViewer::StatsHandler);
	m_pViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

	auto context = m_pViewer->setUpViewerAsEmbeddedInWindow(0, 0, 640, 480);
	m_pViewer->getEventQueue()->setGraphicsContext(context);
	
	m_pGraphWindow = dynamic_cast<osgViewer::GraphicsWindow*>(context);

	startTimer(1000);
}

QSGNode * OSGItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *nodeData)
{
	std::cout << "OSGItem::updatePaintNode()" << std::endl;
	if(nullptr == oldNode) 
	{
		oldNode = QQuickFramebufferObject::updatePaintNode(oldNode, nodeData);
		QSGSimpleTextureNode* textureNode = static_cast<QSGSimpleTextureNode*>(oldNode);
		if (textureNode) 
		{
			textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
		}
		return oldNode;
	}
	return oldNode;
	//return QQuickFramebufferObject::updatePaintNode(oldNode, nodeData);
}

void OSGItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	m_pGraphWindow->getEventQueue()->windowResize(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
	m_pCamera->setViewport(new osg::Viewport(0, 0, newGeometry.width(), newGeometry.height()));
	const double aspectRatio = newGeometry.width() / newGeometry.height();
	m_pCamera->setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 10000.0f);
	QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
	update();
}

void OSGItem::mousePressEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseButtonPress(event->x(), event->y(), event->button());
}

void OSGItem::mouseMoveEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseMotion(event->x(), event->y());
	update();
}

void OSGItem::mouseReleaseEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseButtonRelease(event->x(), event->y(), event->button());
	update();
}

void OSGItem::mouseDoubleClickEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), event->button());
	update();
}

void OSGItem::wheelEvent(QWheelEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseScroll(
		event->orientation() == Qt::Vertical ?
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
	update();
}

void OSGItem::keyPressEvent(QKeyEvent *event)
{
	m_pGraphWindow->getEventQueue()->keyPress(
		(osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
	update();
}

void OSGItem::keyReleaseEvent(QKeyEvent *event)
{
	m_pGraphWindow->getEventQueue()->keyPress(
		(osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
	update();
}

void OSGItem::timerEvent(QTimerEvent *event)
{
	QQuickFramebufferObject::update();
}

