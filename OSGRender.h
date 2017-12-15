#pragma once
#include <osg/observer_ptr>
#include <QQuickFramebufferObject>
#include <osgViewer/Viewer>
class OSGView;

class OSGRender : public QQuickFramebufferObject::Renderer
{
public:
	OSGRender(const OSGView* view);
	virtual ~OSGRender();

	virtual QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

	virtual void render();

private:
	const OSGView* m_pOSGView;
	osg::observer_ptr<osgViewer::Viewer> m_pViewer;
};
