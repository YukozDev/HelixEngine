#include "GraphicsApplication.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HelixCore/CoreEngine.h>
#include <malloc.h>
#include <iostream>

GLuint CreateShader(GLenum type, const char* source);
GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

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

	// Triangle setup
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	GLfloat positionVertices[15] = {
		 0.0f,  0.1f, 1.0f, 0.0f, 0.0f, // Vertex 0: Red Top
		-0.1f, -0.1f, 0.0f, 1.0f, 0.0f, // Vertex 1: Green Bottom Left
		 0.1f, -0.1f, 0.0f, 0.0f, 1.0f  // Vertex 2: Blue Bottom Right
	};

	GLuint positionIndices[] = {
		0, 1, 2
	};

	// Send vertex data to GPU
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), positionVertices, GL_STATIC_DRAW);

	// Send index data to GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), positionIndices, GL_STATIC_DRAW);

	// Define vertex attribute layout
	glEnableVertexAttribArray(0);  // Position attribute
	glEnableVertexAttribArray(1);  // Color attribute


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	const char* vertexShaderSource = R"(
		#version 460 core

		layout(location = 0) in vec2 position;
		layout(location = 1) in vec3 color;

		out vec3 fragColor;
		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
			fragColor = color;
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 460 core

		in vec3 fragColor;
		out vec4 finalColor;
   
		void main()
		{
			finalColor = vec4(fragColor, 1.0);
		}
	)";

	GLuint vertexShaderId = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	GLuint programShader = CreateProgram(vertexShaderId, fragmentShaderId);
	glUseProgram(programShader);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.21, 0.26, 0.40, 1.0);

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLuint CreateShader(GLenum type, const char* source)
{
	GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	GLint shaderCompiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);

	if (shaderCompiled != GL_TRUE)
	{
		GLint msgLenght;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLenght);

		GLchar message = (GLchar)alloca(msgLenght);
		glGetShaderInfoLog(shaderId, msgLenght, 0, &message);

		// TODO: LOG
		std::cout << message << "\n";

		glDeleteShader(shaderId);
		hxAssert(false, message);
	}

	return shaderId;
}

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	GLint programLinked;
	glGetProgramiv(programId, GL_LINK_STATUS, &programLinked);

	if (programLinked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(programId, 1024, &log_length, message);
		
		// TODO: LOG
		std::cout << message << "\n";

		glDeleteProgram(programId);

		hxAssert(false, message);
	}

	glValidateProgram(programId);

	return programId;
}