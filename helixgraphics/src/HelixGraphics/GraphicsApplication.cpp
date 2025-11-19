#include "GraphicsApplication.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HelixCore/CoreEngine.h>

hxInt32 main(hxInt32 argc, hxChar** argv)
{
	if (!glfwInit()) 
	{
		hxAssert(false, "GLFW failed to initialize");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);   
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* window = glfwCreateWindow(800, 800, "Helix Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		hxAssert(false, "Window failed to create");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	// Charger les fonctions OpenGL avec GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		hxAssert(false, "Failed to initialize GLAD");
	}

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.21, 0.26, 0.40, 1.0);

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}