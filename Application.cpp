#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void setupGlfw();
void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void setupTriangle(unsigned int VAO, unsigned int VBO, int vsize, float vertices[]);

const char *vertextShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(0.941f, 0.219f, 0.219f, 1.0f);\n"
"}\0";

const int SCR_WIDTH = 700;
const int SCR_HEIGHT = 600;

int main()
{
	setupGlfw();

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangles", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "GLFW has failed to create the window!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load OpenGL function pointers!\n";
		return -1;
	}

	auto vertextShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertextShader, 1, &vertextShaderSource, NULL);
	glCompileShader(vertextShader);

	int success{ 0 };
	char infoLog[512];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);
		std::cout << "failed to compile vertext shader. info: " << infoLog << "\n";
	}

	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "failed to compile fragment shader. info: " << infoLog << "\n";
	}

	auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertextShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "failed to link program. info: " << infoLog << "\n";
	}

	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);

	float firstTriangle[] = {
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};
	float secondTriangle[] = {
		-0.5f, -0.4f, 0.0f,
		 0.0f,  0.6f, 0.0f,
		 0.5f, -0.4f, 0.0f
	};
	float thirdTriangle[] = {
		-0.2f, -0.6f, 0.0f,
		 0.0f, -0.2f, 0.0f,
		 0.2f, -0.6f, 0.0f
	};

	unsigned int VBOs[3], VAOs[3];
	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);

	setupTriangle(VAOs[0], VBOs[0], sizeof(firstTriangle), firstTriangle);
	setupTriangle(VAOs[1], VBOs[1], sizeof(secondTriangle), secondTriangle);
	setupTriangle(VAOs[2], VBOs[2], sizeof(thirdTriangle), thirdTriangle);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.109f, 0.109f, 0.109f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}

void setupGlfw()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void setupTriangle(unsigned int VAO, unsigned int VBO, int vsize, float vertices[])
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}