#include"MainRenderer.h"

MainRenderer::MainRenderer(Camera& camera) : m_camera(camera)
{
   
}

void MainRenderer::Render()
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

MainRenderer::~MainRenderer()
{

}