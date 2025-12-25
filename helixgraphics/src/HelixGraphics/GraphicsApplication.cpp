#include "GraphicsApplication.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HelixCore/CoreEngine.h>
#include <HelixCore/File/FileBinStream.h>
#include <HelixCore/Logger/LogCategory.h>
#include <HelixCore/Logger/Logger.h>
#include <malloc.h>
#include <iostream>

GLuint CreateShader(GLenum type, const char* source);
GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

// TODO: LOG
void APIENTRY GLDebugMessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

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

	//hxLogDebug(LogGraphics, "Test");

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

	// Create OpenGL Context Debug
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDebugMessageCallback, nullptr);

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

	FileBinStream* vertexFile = new FileBinStream("ressource/Shader/VertexShader.shader", false, FileAccess::Read);
	size_t vertexSize = vertexFile->Size();

	hxChar* vertexShaderSource = (char*)alloca(vertexSize + 1);
	vertexFile->Read(vertexShaderSource, vertexSize);
	vertexShaderSource[vertexSize] = '\0';

	FileBinStream* fragmentFile = new FileBinStream("ressource/Shader/FragmentShader.shader", false, FileAccess::Read);
	size_t fragmentSize = fragmentFile->Size();

	hxChar* fragmentShaderSource = (char*)alloca(fragmentSize + 1);

	size_t test = sizeof(fragmentShaderSource);

	fragmentFile->Read(fragmentShaderSource, fragmentSize);
	fragmentShaderSource[fragmentSize] = '\0';

	delete vertexFile;
	delete fragmentFile;

	std::cout << vertexShaderSource << "\n";
	std::cout << fragmentShaderSource << "\n";

	GLuint vertexShaderId = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderId = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	GLuint programShader = CreateProgram(vertexShaderId, fragmentShaderId);
	glUseProgram(programShader);

	//Uniforme
	GLint colorLocation = glGetUniformLocation(programShader, "u_Color");
	hxAssert(colorLocation != -1);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.21, 0.26, 0.40, 1.0);

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0);

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

		glDeleteShader(shaderId);
		hxAssert(false);
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

		glDeleteProgram(programId);
		hxAssert(false);
	}

	glValidateProgram(programId);

	return programId;
}

void APIENTRY GLDebugMessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cout << message << "\n";
}