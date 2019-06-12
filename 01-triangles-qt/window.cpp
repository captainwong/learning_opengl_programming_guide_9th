#include "window.h"
#include <qopenglshaderprogram.h>
#include <qdebug.h>
#include <qstring.h>

Window::Window(QWidget* parent)
	: QOpenGLWidget(parent)
{

}

Window::~Window()
{

}

void Window::initializeGL()
{
	initializeOpenGLFunctions();

	auto vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	constexpr auto vsrc = R"(

#version 400 core

layout( location = 0 ) in vec4 vPosition;

void
main()
{
    gl_Position = vPosition;
}

)";
	vshader->compileSourceCode(vsrc);

	auto fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	constexpr auto fsrc = R"(
#version 450 core

out vec4 fColor;

void main()
{
    fColor = vec4(0.5, 0.4, 0.8, 1.0);
}

)";
	fshader->compileSourceCode(fsrc);

	program = new QOpenGLShaderProgram;
	program->addShader(vshader);
	program->addShader(fshader);
	program->link();
	program->bind();
}

#define USE_BUFFER

void Window::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	constexpr GLuint NumVertices = 6;

	GLfloat vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, { 0.85f, -0.90f }, { -0.90f, 0.85f }, // Triangle 1
		{ 0.90f, -0.85f, }, { 0.90f, 0.90f }, { -0.85f, 0.90f }, // Triangle 2
	};

	GLuint vPosition = program->attributeLocation("vPosition");

#ifdef USE_BUFFER
	buffer.create();
	buffer.bind();
	buffer.allocate(vertices, sizeof(vertices));
	program->setAttributeBuffer(vPosition, GL_FLOAT, 0, 2);
#else	
	program->setAttributeArray(vPosition, GL_FLOAT, vertices, 2);
#endif

	program->enableAttributeArray(vPosition);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void Window::resizeGL(int w, int h)
{
}
