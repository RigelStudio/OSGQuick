#pragma once
#include <QQuickItem>
#include <osg/Group>
#include <osg/Camera>
#include <osgViewer/Viewer>
#include <QSGSimpleTextureNode>
#include <QSGDynamicTexture>
#include <QOpenGLFramebufferObject>

class OSGItem : public QQuickItem
{
	Q_OBJECT
public:
	OSGItem(QQuickItem *parent = Q_NULLPTR);
	virtual ~OSGItem();

public slots:
	Q_INVOKABLE  void slotExe();

protected:
	void timerEvent(QTimerEvent *event);


	virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *uppData);

private:
	void initOSG();

	void updateFBO();

private:
	osg::Group* m_pRoot;
	osg::Camera* m_pCamera;
	osgViewer::Viewer* m_pViewer;
	osgViewer::GraphicsWindow* m_pGraphicsWindow;	//osg的图形上下文

	QSGTexture* m_pQtTexture;
	QOpenGLFramebufferObject* m_pQtFBO;
	QSGSimpleTextureNode* m_pQtTextureNode;
};

