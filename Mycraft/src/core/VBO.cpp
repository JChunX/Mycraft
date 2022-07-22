#include"VBO.h"
#include<iostream>

// Constructor that generates a Vertex Buffer Object and links it to vertices
// Note: use size as input because sizeof(vertices) not the same after vertices passed into function as pointer

VBO::VBO()
{

}

void VBO::Buffer(GLfloat* vertices, GLsizeiptr size)
{
	if (!has_buffer)
	{
		glGenBuffers(1, &ID);
		has_buffer = true;
	}
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{

	glDeleteBuffers(1, &ID);
}