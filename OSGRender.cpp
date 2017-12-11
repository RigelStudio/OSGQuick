#include "OSGRender.h"
#include <osg/Camera>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>

OSGRender::OSGRender(osgViewer::Viewer* viewer)
	: m_pViewer(viewer)
{
}


OSGRender::~OSGRender()
{
}

QOpenGLFramebufferObject * OSGRender::createFramebufferObject(const QSize &size)
{
	m_pViewer->getCamera()->setViewport(0, 0, size.width(), size.height());
	m_pViewer->getEventQueue()->windowResize(0, 0, size.width(), size.height());

	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	format.setSamples(4);
	return new QOpenGLFramebufferObject(size, format);
}

void OSGRender::render()
{
	QOpenGLContext::currentContext()->functions()->glUseProgram(0);
	if (m_pViewer->isRealized())
	{
		m_pViewer->realize();
	}
	m_pViewer->frame();
	update();
}

void OSGRender::synchronize(QQuickFramebufferObject *item)
{
	
}
