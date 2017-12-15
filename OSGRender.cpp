#include "OSGRender.h"
#include <osg/Camera>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>

#include <iostream>
#include "OSGView.h"

OSGRender::OSGRender(const OSGView* view)
	: m_pViewer(view->m_pViewer)
{
	m_pOSGView = view;
	std::cout << "OSGRender::OSGRender()" << std::endl;
}


OSGRender::~OSGRender()
{
}

QOpenGLFramebufferObject * OSGRender::createFramebufferObject(const QSize &size)
{
	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	//format.setSamples(8);
	return new QOpenGLFramebufferObject(size, format);
}

void OSGRender::render()
{
	QOpenGLContext::currentContext()->functions()->glUseProgram(0);
	osgGA::EventQueue::Events events;
	m_pOSGView->m_pEventQueue->takeEvents(events);
	if (events.size() > 0)
	{
		m_pOSGView->m_pViewer->getEventQueue()->appendEvents(events);
	}
	m_pOSGView->m_pViewer->frame();
	update();
}

