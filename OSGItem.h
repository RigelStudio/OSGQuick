#pragma once
#include <QQuickItem>
#include <osg/Group>

class OSGItem : public QQuickItem
{
	Q_OBJECT
public:
	OSGItem(QQuickItem *parent = Q_NULLPTR);
	virtual ~OSGItem();

public slots:
	void slotExe();
};

