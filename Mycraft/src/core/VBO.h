#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO
{
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    // Constructor that build the Vertex Buffer Object from data
    VBO(GLfloat* vertices, GLsizeiptr size);
    // Bind and unbinding
    void Bind();
    void Unbind();
    // Deletes the Vertex Buffer Object
    void Delete();
};

#endif