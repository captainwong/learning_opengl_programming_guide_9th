#pragma once

#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglbuffer.h>

class QOpenGLShaderProgram;

class Window : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit Window(QWidget* parent = nullptr);
	virtual ~Window();

protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;

private:
	QOpenGLShaderProgram* program = nullptr;
	QOpenGLBuffer buffer = {};
};
