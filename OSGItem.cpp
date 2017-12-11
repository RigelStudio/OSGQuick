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
	m_pCamera->setViewport(new osg::Viewport(0, 0, width(), height()));
	const double aspectRatio = static_cast<double>(width()) / static_cast<double>(height());
	m_pCamera->setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 10000.0f);
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

	startTimer(16);
}

QSGNode * OSGItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *nodeData)
{
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
	return QQuickFramebufferObject::updatePaintNode(oldNode, nodeData);
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
}

void OSGItem::mouseDoubleClickEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), event->button());
}

void OSGItem::wheelEvent(QWheelEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseScroll(
		event->orientation() == Qt::Vertical ?
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
}

void OSGItem::keyPressEvent(QKeyEvent *event)
{
	m_pGraphWindow->getEventQueue()->keyPress(
		(osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
}

void OSGItem::keyReleaseEvent(QKeyEvent *event)
{
	m_pGraphWindow->getEventQueue()->keyPress(
		(osgGA::GUIEventAdapter::KeySymbol)*(event->text().toLatin1().data()));
}

void OSGItem::timerEvent(QTimerEvent *event)
{
	//update();
}

