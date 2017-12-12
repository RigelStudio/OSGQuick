#include "OSGRender.h"
#include <osg/Camera>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>

#include <iostream>

OSGRender::OSGRender(osgViewer::Viewer* viewer)
	: m_pViewer(viewer)
{
	std::cout << "OSGRender::OSGRender()" << std::endl;
}


OSGRender::~OSGRender()
{
}

QOpenGLFramebufferObject * OSGRender::createFramebufferObject(const QSize &size)
{
	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	format.setSamples(4);
	
	return new QOpenGLFramebufferObject(size, format);
}

void OSGRender::render()
{
	std::cout << "render" << std::endl;
	QOpenGLContext::currentContext()->functions()->glUseProgram(0);
	if (m_pViewer->isRealized())
	{
		m_pViewer->realize();
	}
}

void OSGRender::synchronize(QQuickFramebufferObject *item)
{
	std::cout << "synchronize" << std::endl;
	m_pViewer->frame();
}
