#include "OSGItem.h"
#include <QQuickWindow>

OSGItem::OSGItem(QQuickItem *parent/* = Q_NULLPTR*/)
	:QQuickItem(parent)
{
	m_pRoot = nullptr;
	m_pCamera = nullptr;
	m_pViewer = nullptr;
	m_pQtFBO = nullptr;
	m_pQtTexture = nullptr;
	m_pQtTextureNode = nullptr;
	setFlag(QQuickItem::ItemHasContents, true);
	initOSG();
}


OSGItem::~OSGItem()
{
	delete m_pViewer;
}

void OSGItem::slotExe()
{

}

void OSGItem::timerEvent(QTimerEvent *event)
{
	if (m_pViewer->isRealized())
	{
		m_pViewer->realize();
	}
	m_pViewer->frame();
	update();
}

QSGNode * OSGItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *uppData)
{
	updateFBO();
	if (oldNode && oldNode != m_pQtTextureNode) {
		delete oldNode;
	}
	Q_UNUSED(uppData);

	return m_pQtTextureNode;
}

void OSGItem::initOSG()
{
	m_pViewer = new osgViewer::Viewer;
	m_pRoot = new osg::Group;
	m_pViewer->setSceneData(m_pRoot);
	m_pViewer->setUpViewerAsEmbeddedInWindow(0, 0, 640, 480);
	m_pCamera = m_pViewer->getCamera();
	auto context = m_pViewer->getCamera()->getGraphicsContext();
	m_pGraphicsWindow = dynamic_cast<osgViewer::GraphicsWindow*>(context);
	
	startTimer(10);
}

void OSGItem::updateFBO()
{
	QRectF rect = mapRectToItem(0, boundingRect());
	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	QSize size(rect.size().toSize());
	m_pQtFBO = new QOpenGLFramebufferObject(size, format);
	m_pQtTexture = window()->createTextureFromId(m_pQtFBO->texture(), size);
	m_pQtTextureNode = new QSGSimpleTextureNode();
	m_pQtTextureNode->setRect(0, height(), width(), -height());
	m_pQtTextureNode->setTexture(m_pQtTexture);
	
}
