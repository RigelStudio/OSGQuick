#pragma once
#include <QQuickFramebufferObject>
#include <osg/Group>
#include <osg/Camera>
#include <osgViewer/Viewer>
#include <QSGSimpleTextureNode>
#include <QSGDynamicTexture>
#include <QOpenGLFramebufferObject>

class OSGItem : public QQuickFramebufferObject
{
	Q_OBJECT
public:
    OSGItem(QQuickItem *parent = Q_NULLPTR);
    virtual ~OSGItem();
public:
	osg::Camera* getCamera();

    osgViewer::Viewer* getViewer();

    osg::Group* getSceneData();

public slots:
	Q_INVOKABLE void slotHome();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *uppData);
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

    /***事件处理***/
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void timerEvent(QTimerEvent *event);

	Renderer* createRenderer() const;

private:
	void initOSG();

private:
	osg::Group* m_pRoot;
	osg::Camera* m_pCamera;
	osgViewer::Viewer* m_pViewer;
	osgViewer::GraphicsWindow* m_pGraphWindow;
};
