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
	m_pViewer->addEventHandler(new osgViewer::StatsHandler());
	m_pViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

	auto context = m_pViewer->setUpViewerAsEmbeddedInWindow(0, 0, 1920, 1080);
	//m_pCamera->setGraphicsContext(context);
	m_pViewer->getEventQueue()->setGraphicsContext(context);
	
	m_pGraphWindow = dynamic_cast<osgViewer::GraphicsWindow*>(context);
	m_pGraphWindow->getEventQueue()->keyPress(115);
}

QSGNode * OSGItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *nodeData)
{
	std::cout << "OSGItem::updatePaintNode()" << std::endl;
	QSGNode* node = QQuickFramebufferObject::updatePaintNode(oldNode, nodeData);
	QSGSimpleTextureNode& textureNode = dynamic_cast<QSGSimpleTextureNode&>(*node);
	textureNode.setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
	return node;
}

void OSGItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
 	//m_pGraphWindow->getEventQueue()->windowResize(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
	//m_pViewer->getCamera()->setViewport(new osg::Viewport(0, 0, newGeometry.width(), newGeometry.height()));
	//const double aspectRatio = newGeometry.width() / newGeometry.height();
	//m_pViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 10000.0f);
	QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
}

void OSGItem::mousePressEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseButtonPress(event->x(), event->y(), event->button());
}

void OSGItem::mouseMoveEvent(QMouseEvent *event)
{
	m_pGraphWindow->getEventQueue()->mouseMotion(event->x(), event->y(), event->button());
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
	const std::string text = event->text().toStdString();
	if (text.size() == 1)
	{
		m_pGraphWindow->getEventQueue()->keyPress(text[0]);
		update();
	}
}

