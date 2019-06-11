// 01-triangles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <LoadShaders.h>

enum VertexArrayObject_IDs
{
	Triangles,
	NumVAOs,
};

enum Buffer_IDs
{
	ArrayBuffer,
	NumBuffers,
};

enum Attrib_IDs
{
	vPosition = 0,
};

GLuint VAOs[VertexArrayObject_IDs::NumVAOs];
GLuint Buffers[NumBuffers];
constexpr GLuint NumVertices = 6;


void init()
{
	glCreateVertexArrays(NumVAOs, VAOs);
	printf("VertexArrayObjects[VertexArrayObject_IDs::Triangles]=%u\n", VAOs[Triangles]);

	glCreateBuffers(NumBuffers, Buffers);
	GLfloat vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, { 0.85f, -0.90f }, { -0.90f, 0.85f }, // vertex 1
		{ 0.90f, -0.85f, }, { 0.90f, 0.90f }, { -0.85f, 0.90f }, // vertex 2
	};
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, R"(
#version 400 core

layout( location = 0 ) in vec4 vPosition;

void
main()
{
    gl_Position = vPosition;
}
)" },
		{ GL_FRAGMENT_SHADER, R"(
#version 450 core

out vec4 fColor;

void main()
{
    fColor = vec4(0.5, 0.4, 0.8, 1.0);
}
)" },
		{ GL_NONE },
	};
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glBindVertexArray(VAOs[Triangles]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(vPosition);
}

void display()
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glBindVertexArray(VAOs[Triangles]);

	// 跟书上说的不一样，GL_PATCHES 是有显示的，与 GL_POINTS 效果一样
	glDrawArrays(GL_TRIANGLES /*GL_POINTS*/ /*GL_LINES*/ /*GL_PATCHES*/, 0, NumVertices);
}


int main()
{
	glfwInit();

	auto window = glfwCreateWindow(800, 600, "Triangles", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	gl3wInit();

	init();

	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
