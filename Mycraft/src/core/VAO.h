#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    // Reference ID of the Vertex Array Object
    GLuint ID;
    // Constructor
    VAO();
    // Link VBO to VAO
    void LinkAttrib(VBO& VBO, 
				  GLuint index, 
				  GLint size, 
				  GLenum type, 
				  GLsizeiptr stride, 
				  void* offset);
    // Bind and unbinding
    void Bind();
    void Unbind();
    // Deletes the Vertex Array Object
    void Delete();
};

#endif