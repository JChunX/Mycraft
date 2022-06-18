#ifndef SKYBOX_H
#define SKYBOX_H

class SKybox
{
public:// TODO
    SKybox(Camera& camera);
    ~SKybox();
    void Render(Scene& scene);
private:
    Shader m_shader;
    Camera& m_camera;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_texture;
};

#endif // SKYBOX_H