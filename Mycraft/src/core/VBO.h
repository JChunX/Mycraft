#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO
{
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    bool has_buffer = false;
    VBO();
    
    void Buffer(GLfloat* vertices, GLsizeiptr size);
    // Bind and unbinding
    void Bind();
    void Unbind();
    // Deletes the Vertex Buffer Object
    void Delete();
};

#endif