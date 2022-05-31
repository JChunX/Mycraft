#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cmath>
#include<filesystem>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 720;


float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object 
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW Window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);  

	// Generates Shader objects
	Shader defaultShader("shaders/default.vert", "shaders/default.frag");

	// Generates Vertex Array Objects
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));

	// Links VBO attributes
	// 1. vertex attributes
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// 2. surface normal attributes
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*) (3*sizeof(float)));

	VAO VAOLight;
	VAOLight.Bind();
	VAOLight.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VAOLight.Unbind();
	VBO1.Unbind();

	// Shader needs to be activated before changing the value of a uniform
	defaultShader.Activate();
	Camera camera(SCR_WIDTH, SCR_HEIGHT, window, 100.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera.SetProjectionUniform(defaultShader, "projection");
	glm::vec3 lightPos = glm::vec3(1.2f, 2.0f, 2.0f);

	defaultShader.Activate();
	camera.SetProjectionUniform(defaultShader, "projection");

	InputBroadcaster::AddListener(camera);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Clear the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		defaultShader.Activate();
		// Set view
		InputBroadcaster::ReadInputs(window);
		camera.SetViewUniform(defaultShader, "view");
		// Set color uniforms
 		defaultShader.SetVec3("light.position", lightPos.x, lightPos.y, lightPos.z);

        // light properties
        glm::vec3 lightColor;
        lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
        lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
        lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
        defaultShader.SetVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
        defaultShader.SetVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
        defaultShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        defaultShader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        defaultShader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        defaultShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        defaultShader.SetFloat("material.shininess", 32.0f);

        
		VAO1.Bind();
		// draw 10000 cubes laid out on grid
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				// Set model
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 1.5f-100, 0.0f, j * 1.5f-100));
				// Spin the cube at a rate proportional to its position
				//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f) * (float)i, glm::vec3(0.5f, 1.0f, 0.0f));
				// Move cube up and down
				model = glm::translate(model, glm::vec3(0.0f, (float)sin(glfwGetTime() * 2.0f) * 0.5f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
				// Draw the cube
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// draw second square acting as lamp
		defaultShader.Activate();
		camera.SetViewUniform(defaultShader, "view");
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		VAOLight.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	defaultShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}