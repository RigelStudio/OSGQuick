#pragma once
#include <osg/observer_ptr>
#include <QQuickFramebufferObject>
#include <osgViewer/Viewer>

class OSGRender : public QQuickFramebufferObject::Renderer
{
public:
	OSGRender(osgViewer::Viewer* viewer);
	virtual ~OSGRender();

	virtual QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

	virtual void render();

private:
	osg::observer_ptr<osgViewer::Viewer> m_pViewer;
};
