#include <iostream>
#include <GL/glew.h>
#include <gl/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include <iostream>
#include <soil.h>
#include "shader_s.h"
#define M_PI 3.1415926535897932384626433832795


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

int stacks = 50;
int slices = 50;
float radius = 1.0f;
float height = 2.0f;


Camera camera(glm::vec3(2.0f, 1.0f, 2.0f));
float lastX = SCR_WIDTH / 1.4f;
float lastY = SCR_HEIGHT / 1.4f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.5f, 1.0f, -1.8f);// sin(glfwGetTime()) * 2.0f (0.5f, 0.5f, -1.8f)

//glm::vec3 lightPos(glfwGetTime() * 2.0f, 1.0f, glfwGetTime() * 2.0f);

void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int stacks, int slices) {
	for (int i = 0; i <= stacks; ++i) {
		float theta = i * M_PI / stacks;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int j = 0; j <= slices; ++j) {
			float phi = j * 2 * M_PI / slices;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			float x = cosPhi * sinTheta;
			float y = cosTheta;
			float z = sinPhi * sinTheta;

			vertices.push_back(radius * x);
			vertices.push_back(radius * y);
			vertices.push_back(radius * z);
		}
	}

	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			unsigned int first = (i * (slices + 1)) + j;
			unsigned int second = first + slices + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}
}



int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// glfw: создание окна
	GLFWwindow* window = glfwCreateWindow(2560, 1440, "Hello", nullptr, 0);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("basic_lighting.vs", "basic_lighting.fs");
	Shader lampShader("lamp.vs", "lamp.fs");

	float vertices[] = {


								// my solution
								//
								//0.0f, 0.0f, -0.05f, 0.0f, 1.0f,
								//-0.1f, 0.0f, -0.1f, 0.0f, 0.0f, 
								//-0.1f, -0.6f, -0.1f, 1.0f, 1.0f, 

								//-0.1f, -0.6f, -0.1f, 1.0f, 1.0f,
								//0.0f, -0.6f, -0.05f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.05f, 0.0f, 1.0f,


								//0.0f, 0.0f, -0.05f, 0.0f, 1.0f,
								//0.1f, 0.0f, -0.1f, 0.0f, 0.0f, 
								//0.1f, -0.6f, -0.1f, 1.0f, 1.0f, 

								//0.1f, -0.6f, -0.1f, 1.0f, 1.0f,
								//0.0f, -0.6f, -0.05f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.05f, 0.0f, 1.0f,
								//

								//0.1f, 0.0f, -0.1f, 0.0f, 1.0f,
								//0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 
								//0.05f, -0.6f, 0.0f, 1.0f, 1.0f, 

								//0.05f, -0.6f, 0.0f, 1.0f, 1.0f,
								//0.1f, -0.6f, -0.1f, 1.0f, 0.0f,
								//0.1f, 0.0f, -0.1f, 0.0f, 1.0f,


								//0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
								//0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 
								//0.1f, -0.6f, 0.1f, 1.0f, 1.0f, 

								//0.1f, -0.6f, 0.1f, 1.0f, 1.0f,
								//0.05f, -0.6f, 0.0f, 1.0f, 0.0f,
								//0.05f, 0.0f, 0.0f, 0.0f, 1.0f,


								//0.1f, 0.0f, 0.1f, 0.0f, 1.0f,
								//0.0f, 0.0f, 0.05f, 0.0f, 0.0f, 
								//0.0f, -0.6f, 0.05f, 1.0f, 1.0f, 

								//0.0f, -0.6f, 0.05f, 1.0f, 1.0f,
								//0.1f, -0.6f, 0.1f, 1.0f, 0.0f,
								//0.1f, 0.0f, 0.1f, 0.0f, 1.0f,


								//0.0f, 0.0f, 0.05f, 0.0f, 1.0f,
								//-0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 
								//-0.1f, -0.6f, 0.1f, 1.0f, 1.0f, 

								//-0.1f, -0.6f, 0.1f, 1.0f, 1.0f,
								//0.0f, -0.6f, 0.05f, 1.0f, 0.0f,
								//0.0f, 0.0f, 0.05f, 0.0f, 1.0f,


								//-0.1f, 0.0f, 0.1f, 0.0f, 1.0f,
								//-0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 
								//-0.05f, -0.6f, 0.0f, 1.0f, 1.0f, 

								//-0.05f, -0.6f, 0.0f, 1.0f, 1.0f,
								//-0.1f, -0.6f, 0.1f, 1.0f, 0.0f,
								//-0.1f, 0.0f, 0.1f, 0.0f, 1.0f,


								//-0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
								//-0.1f, 0.0f, -0.1f, 0.0f, 0.0f, 
								//-0.1f, -0.6f, -0.1f, 1.0f, 1.0f, 

								//-0.1f, -0.6f, -0.1f, 1.0f, 1.0f,
								//-0.05f, -0.6f, 0.0f, 1.0f, 0.0f,
								//-0.05f, 0.0f, 0.0f, 0.0f, 1.0f, // star trunk
								////'''''''''''''''''''''''''''''
								//0.0f, 0.0f, -0.1f, 0.0f, 1.0f,
								//-0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 
								//-0.1f, -0.6f, 0.0f, 1.0f, 1.0f, 

								//-0.1f, -0.6f, 0.0f, 1.0f, 1.0f,
								//0.0f, -0.6f, -0.1f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.1f, 0.0f, 1.0f,


								//-0.1f, 0.0f, 0.0f, 0.0f, 1.0f,
								//0.0f, 0.0f, 0.1f, 0.0f, 0.0f,
								//0.0f, -0.6f, 0.1f, 1.0f, 1.0f,

								//0.0f, -0.6f, 0.1f, 1.0f, 1.0f,
								//-0.1f, -0.6f, 0.0f, 1.0f, 0.0f,
								//-0.1f, 0.0f, 0.0f, 0.0f, 1.0f,


								//0.0f, 0.0f, 0.1f, 0.0f, 1.0f,
								//0.1f, 0.0f, 0.0f, 0.0f, 0.0f,
								//0.1f, -0.6f, 0.0f, 1.0f, 1.0f,

								//0.1f, -0.6f, 0.0f, 1.0f, 1.0f,
								//0.0f, -0.6f, 0.1f, 1.0f, 0.0f,
								//0.0f, 0.0f, 0.1f, 0.0f, 1.0f,


								//0.1f, 0.0f, 0.0f, 0.0f, 1.0f,
								//0.0f, 0.0f, -0.1f, 0.0f, 0.0f,
								//0.0f, -0.6f, -0.1f, 1.0f, 1.0f,

								//0.0f, -0.6f, -0.1f, 1.0f, 1.0f,
								//0.1f, -0.6f, 0.0f, 1.0f, 0.0f,
								//0.1f, 0.0f, 0.0f, 0.0f, 1.0f,
								
								// ниже уменьшил в 2 раза

								//0.0f, 0.0f, -0.025f, 0.0f, 1.0f,
								//-0.05f, 0.0f, -0.05f, 0.0f, 0.0f,
								//-0.05f, -0.6f, -0.05f, 1.0f, 1.0f,

								//-0.05f, -0.6f, -0.05f, 1.0f, 1.0f,
								//0.0f, -0.6f, -0.025f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.025f, 0.0f, 1.0f,


								//0.0f, 0.0f, -0.025f, 0.0f, 1.0f,
								//0.05f, 0.0f, -0.05f, 0.0f, 0.0f,
								//0.05f, -0.6f, -0.05f, 1.0f, 1.0f,

								//0.05f, -0.6f, -0.05f, 1.0f, 1.0f,
								//0.0f, -0.6f, -0.025f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.025f, 0.0f, 1.0f,


								//0.05f, 0.0f, -0.05f, 0.0f, 1.0f,
								//0.025f, 0.0f, 0.0f, 0.0f, 0.0f,
								//0.025f, -0.6f, 0.0f, 1.0f, 1.0f,

								//0.025f, -0.6f, 0.0f, 1.0f, 1.0f,
								//0.05f, -0.6f, -0.05f, 1.0f, 0.0f,
								//0.05f, 0.0f, -0.05f, 0.0f, 1.0f,


								//0.025f, 0.0f, 0.0f, 0.0f, 1.0f,
								//0.05f, 0.0f, 0.05f, 0.0f, 0.0f,
								//0.05f, -0.6f, 0.05f, 1.0f, 1.0f,

								//0.05f, -0.6f, 0.05f, 1.0f, 1.0f,
								//0.025f, -0.6f, 0.0f, 1.0f, 0.0f,
								//0.025f, 0.0f, 0.0f, 0.0f, 1.0f,


								//0.05f, 0.0f, 0.05f, 0.0f, 1.0f,
								//0.0f, 0.0f, 0.025f, 0.0f, 0.0f,
								//0.0f, -0.6f, 0.025f, 1.0f, 1.0f,

								//0.0f, -0.6f, 0.025f, 1.0f, 1.0f,
								//0.05f, -0.6f, 0.05f, 1.0f, 0.0f,
								//0.05f, 0.0f, 0.05f, 0.0f, 1.0f,


								//0.0f, 0.0f, 0.025f, 0.0f, 1.0f,
								//-0.05f, 0.0f, 0.05f, 0.0f, 0.0f,
								//-0.05f, -0.6f, 0.05f, 1.0f, 1.0f,

								//-0.05f, -0.6f, 0.05f, 1.0f, 1.0f,
								//0.0f, -0.6f, 0.025f, 1.0f, 0.0f,
								//0.0f, 0.0f, 0.025f, 0.0f, 1.0f,


								//-0.05f, 0.0f, 0.05f, 0.0f, 1.0f,
								//-0.025f, 0.0f, 0.0f, 0.0f, 0.0f,
								//-0.025f, -0.6f, 0.0f, 1.0f, 1.0f,

								//-0.025f, -0.6f, 0.0f, 1.0f, 1.0f,
								//-0.05f, -0.6f, 0.05f, 1.0f, 0.0f,
								//-0.05f, 0.0f, 0.05f, 0.0f, 1.0f,


								//-0.025f, 0.0f, 0.0f, 0.0f, 1.0f,
								//-0.05f, 0.0f, -0.05f, 0.0f, 0.0f,
								//-0.05f, -0.6f, -0.05f, 1.0f, 1.0f,

								//-0.05f, -0.6f, -0.05f, 1.0f, 1.0f,
								//-0.025f, -0.6f, 0.0f, 1.0f, 0.0f,
								//-0.025f, 0.0f, 0.0f, 0.0f, 1.0f, // ствол звезды
								////'''''''''''''''''''''''''''''
								//0.0f, 0.0f, -0.05f, 0.0f, 1.0f,
								//-0.05f, 0.0f, 0.0f, 0.0f, 0.0f,
								//-0.05f, -0.6f, 0.0f, 1.0f, 1.0f,

								//-0.05f, -0.6f, 0.0f, 1.0f, 1.0f,
								//0.0f, -0.6f, -0.05f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.05f, 0.0f, 1.0f,


								//-0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
								//0.0f, 0.0f, 0.05f, 0.0f, 0.0f,
								//0.0f, -0.6f, 0.05f, 1.0f, 1.0f,

								//0.0f, -0.6f, 0.05f, 1.0f, 1.0f,
								//-0.05f, -0.6f, 0.0f, 1.0f, 0.0f,
								//-0.05f, 0.0f, 0.0f, 0.0f, 1.0f,


								//0.0f, 0.0f, 0.05f, 0.0f, 1.0f,
								//0.05f, 0.0f, 0.0f, 0.0f, 0.0f,
								//0.05f, -0.6f, 0.0f, 1.0f, 1.0f,

								//0.05f, -0.6f, 0.0f, 1.0f, 1.0f,
								//0.0f, -0.6f, 0.05f, 1.0f, 0.0f,
								//0.0f, 0.0f, 0.05f, 0.0f, 1.0f,


								//0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
								//0.0f, 0.0f, -0.05f, 0.0f, 0.0f,
								//0.0f, -0.6f, -0.05f, 1.0f, 1.0f,

								//0.0f, -0.6f, -0.05f, 1.0f, 1.0f,
								//0.05f, -0.6f, 0.0f, 1.0f, 0.0f,
								//0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
								
								//romb
								//'''''''''''''''''''''''''''
								//0.0f, 0.0f, -0.1f, 0.0f, 1.0f,
								//-0.1f, 0.0f, 0.0f, 0.0f, 0.0f,
								//0.0f, 0.0f, 0.1f, 1.0f, 1.0f,

								//0.0f, 0.0f, 0.1f, 1.0f, 1.0f,
								//0.1f, 0.0f, 0.0f, 1.0f, 0.0f,
								//0.0f, 0.0f, -0.1f, 0.0f, 1.0f,//zaglyshka
								0.0f, 0.0f, -0.0125f, 0.0f, 1.0f,
								-0.025f, 0.0f, -0.025f, 0.0f, 0.0f,
								-0.025f, -0.6f, -0.025f, 1.0f, 1.0f,

								-0.025f, -0.6f, -0.025f, 1.0f, 1.0f,
								0.0f, -0.6f, -0.0125f, 1.0f, 0.0f,
								0.0f, 0.0f, -0.0125f, 0.0f, 1.0f,


								0.0f, 0.0f, -0.0125f, 0.0f, 1.0f,
								0.025f, 0.0f, -0.025f, 0.0f, 0.0f,
								0.025f, -0.6f, -0.025f, 1.0f, 1.0f,

								0.025f, -0.6f, -0.025f, 1.0f, 1.0f,
								0.0f, -0.6f, -0.0125f, 1.0f, 0.0f,
								0.0f, 0.0f, -0.0125f, 0.0f, 1.0f,


								0.025f, 0.0f, -0.025f, 0.0f, 1.0f,
								0.0125f, 0.0f, 0.0f, 0.0f, 0.0f,
								0.0125f, -0.6f, 0.0f, 1.0f, 1.0f,

								0.0125f, -0.6f, 0.0f, 1.0f, 1.0f,
								0.025f, -0.6f, -0.025f, 1.0f, 0.0f,
								0.025f, 0.0f, -0.025f, 0.0f, 1.0f,


								0.0125f, 0.0f, 0.0f, 0.0f, 1.0f,
								0.025f, 0.0f, 0.025f, 0.0f, 0.0f,
								0.025f, -0.6f, 0.025f, 1.0f, 1.0f,

								0.025f, -0.6f, 0.025f, 1.0f, 1.0f,
								0.0125f, -0.6f, 0.0f, 1.0f, 0.0f,
								0.0125f, 0.0f, 0.0f, 0.0f, 1.0f,


								0.025f, 0.0f, 0.025f, 0.0f, 1.0f,
								0.0f, 0.0f, 0.0125f, 0.0f, 0.0f,
								0.0f, -0.6f, 0.0125f, 1.0f, 1.0f,

								0.0f, -0.6f, 0.0125f, 1.0f, 1.0f,
								0.025f, -0.6f, 0.025f, 1.0f, 0.0f,
								0.025f, 0.0f, 0.025f, 0.0f, 1.0f,


								0.0f, 0.0f, 0.0125f, 0.0f, 1.0f,
								-0.025f, 0.0f, 0.025f, 0.0f, 0.0f,
								-0.025f, -0.6f, 0.025f, 1.0f, 1.0f,

								-0.025f, -0.6f, 0.025f, 1.0f, 1.0f,
								0.0f, -0.6f, 0.0125f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0125f, 0.0f, 1.0f,


								-0.025f, 0.0f, 0.025f, 0.0f, 1.0f,
								-0.0125f, 0.0f, 0.0f, 0.0f, 0.0f,
								-0.0125f, -0.6f, 0.0f, 1.0f, 1.0f,

								-0.0125f, -0.6f, 0.0f, 1.0f, 1.0f,
								-0.025f, -0.6f, 0.025f, 1.0f, 0.0f,
								-0.025f, 0.0f, 0.025f, 0.0f, 1.0f,


								-0.0125f, 0.0f, 0.0f, 0.0f, 1.0f,
								-0.025f, 0.0f, -0.025f, 0.0f, 0.0f,
								-0.025f, -0.6f, -0.025f, 1.0f, 1.0f,

								-0.025f, -0.6f, -0.025f, 1.0f, 1.0f,
								-0.0125f, -0.6f, 0.0f, 1.0f, 0.0f,
								-0.0125f, 0.0f, 0.0f, 0.0f, 1.0f, // star trunk
								//'''''''''''''''''''''''''''''
								0.0f, 0.0f, -0.025f, 0.0f, 1.0f,
								-0.025f, 0.0f, 0.0f, 0.0f, 0.0f,
								-0.025f, -0.6f, 0.0f, 1.0f, 1.0f,

								-0.025f, -0.6f, 0.0f, 1.0f, 1.0f,
								0.0f, -0.6f, -0.025f, 1.0f, 0.0f,
								0.0f, 0.0f, -0.025f, 0.0f, 1.0f,


								-0.025f, 0.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 0.0f, 0.025f, 0.0f, 0.0f,
								0.0f, -0.6f, 0.025f, 1.0f, 1.0f,

								0.0f, -0.6f, 0.025f, 1.0f, 1.0f,
								-0.025f, -0.6f, 0.0f, 1.0f, 0.0f,
								-0.025f, 0.0f, 0.0f, 0.0f, 1.0f,


								0.0f, 0.0f, 0.025f, 0.0f, 1.0f,
								0.025f, 0.0f, 0.0f, 0.0f, 0.0f,
								0.025f, -0.6f, 0.0f, 1.0f, 1.0f,

								0.025f, -0.6f, 0.0f, 1.0f, 1.0f,
								0.0f, -0.6f, 0.025f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.025f, 0.0f, 1.0f,


								0.025f, 0.0f, 0.0f, 0.0f, 1.0f,
								0.0f, 0.0f, -0.025f, 0.0f, 0.0f,
								0.0f, -0.6f, -0.025f, 1.0f, 1.0f,

								0.0f, -0.6f, -0.025f, 1.0f, 1.0f,
								0.025f, -0.6f, 0.0f, 1.0f, 0.0f,
								0.025f, 0.0f, 0.0f, 0.0f, 1.0f,


								0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
								0.1f, 0.2f, -0.2f, 0.0f, 0.0f,
								0.2f, 0.2f, -0.1f, 1.0f, 1.0f,

								0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
								0.1f, 0.1f, 0.2f, 1.0f, 0.0f,
								0.2f, 0.1f, 0.1f, 0.0f, 1.0f,

								0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
								-0.2f, 0.2f, 0.1f, 1.0f, 0.0f,
								-0.1f, 0.2f, 0.2f, 0.0f, 1.0f,

								0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
								-0.2f, 0.15f, -0.1f, 1.0f, 0.0f,
								-0.1f, 0.15f, -0.2f, 0.0f, 1.0f,

								// first half-leafs triangles 
								0.1f, 0.2f, -0.2f, 0.0f, 1.0f,
								0.2f, 0.2f, -0.1f, 1.0f, 0.0f,
								0.4f, -0.1f, -0.4f, 1.0f, 1.0f,

								0.1f, 0.1f, 0.2f, 1.0f, 1.0f,
								0.2f, 0.1f, 0.1f, 0.0f, 0.0f,
								0.4f, -0.1f, 0.4f, 0.0f, 1.0f,

								-0.2f, 0.2f, 0.1f, 1.0f, 1.0f,
								-0.1f, 0.2f, 0.2f, 0.0f, 0.0f,
								-0.4f, -0.1f, 0.4f, 0.0f, 1.0f,

								-0.2f, 0.15f, -0.1f, 1.0f, 1.0f,
								-0.1f, 0.15f, -0.2f, 0.0f, 0.0f,
								-0.4f, -0.1f, -0.4f, 0.0f, 1.0f,
								// second

								0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
								-0.05f, 0.2f, -0.2f, 0.0f, 0.0f,
								0.05f, 0.2f, -0.2f, 0.0f, 1.0f,

								0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
								0.2f, 0.2f, -0.05f, 0.0f, 0.0f,
								0.2f, 0.2f, 0.05f, 0.0f, 1.0f,

								0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
								-0.05f, 0.15f, 0.2f, 0.0f, 0.0f,
								0.05f, 0.15f, 0.2f, 0.0f, 1.0f,
								
								0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
								-0.2f, 0.1f, -0.05f, 0.0f, 0.0f,
								-0.2f, 0.1f, 0.05f, 0.0f, 1.0f,
								//2d row first tri
								
								-0.05f, 0.2f, -0.2f, 0.0f, 1.0f,
								0.05f, 0.2f, -0.2f, 0.0f, 0.0f,
								0.0f, -0.1f, -0.5f, 0.0f, 1.0f,

								0.2f, 0.2f, -0.05f, 0.0f, 1.0f,
								0.2f, 0.2f, 0.05f, 0.0f, 0.0f,
								0.3f, -0.1f, 0.0f, 0.0f, 1.0f,

								-0.05f, 0.15f, 0.2f, 0.0f, 1.0f,
								0.05f, 0.15f, 0.2f, 0.0f, 0.0f,
								0.0f, -0.1f, 0.5f, 0.0f, 1.0f,

								-0.2f, 0.1f, -0.05f, 0.0f, 1.0f,
								-0.2f, 0.1f, 0.05f, 0.0f, 0.0f,
								-0.3f, -0.1f, 0.0f, 0.0f, 0.0f,
								
								


							
	};


	float floorVertices[] = {

			-2.0f, 0.4f, -2.0f, 0.0f, 0.0f,
			2.0f, 0.4f, -2.0f, 1.0f, 0.0f,
			2.0f, 0.4f, 2.0f, 1.0f, 1.0f,
			2.0f, 0.4f, 2.0f, 1.0f, 1.0f,
			-2.0f, 0.4f, 2.0f, 0.0f, 1.0f,
			-2.0f, 0.4f, -2.0f, 0.0f, 0.0f,

			-2.0f, 0.4f, -2.0f, 0.0f, 0.0f,
			-2.0f, 1.6f, -2.0f, 1.0f, 0.0f,
			2.0f, 1.6f, -2.0f, 1.0f, 1.0f,
			2.0f, 1.6f, -2.0f, 1.0f, 1.0f,
			2.0f, 0.4f, -2.0f, 0.0f, 1.0f,
			-2.0f, 0.4f, -2.0f, 0.0f, 0.0f,

			-2.0f, 0.4f, 2.0f, 0.0f, 0.0f,
			-2.0f, 1.6f, 2.0f, 1.0f, 0.0f,
			2.0f, 1.6f, 2.0f, 1.0f, 1.0f,
			2.0f, 1.6f, 2.0f, 1.0f, 1.0f,
			2.0f, 0.4f, 2.0f, 0.0f, 1.0f,
			-2.0f, 0.4f, 2.0f, 0.0f, 0.0f,

			-2.0f, 0.4f, -2.0f, 0.0f, 0.0f,
			-2.0f, 0.4f, 2.0f, 1.0f, 0.0f,
			-2.0f, 1.6f, 2.0f, 1.0f, 1.0f,
			-2.0f, 1.6f, 2.0f, 1.0f, 1.0f,
			-2.0f, 1.6f, -2.0f, 0.0f, 1.0f,
			-2.0f, 0.4f, -2.0f, 0.0f, 0.0f,

			2.0f, 0.4f, -2.0f, 0.0f, 0.0f,
			2.0f, 0.4f, 2.0f, 1.0f, 0.0f,
			2.0f, 1.6f, 2.0f, 1.0f, 1.0f,
			2.0f, 1.6f, 2.0f, 1.0f, 1.0f,
			2.0f, 1.6f, -2.0f, 0.0f, 1.0f,
			2.0f, 0.4f, -2.0f, 0.0f, 0.0f

	};
	unsigned int indices[] = {
	0,1,2,
	1,2,3

	};
	//
	std::vector<float> sphereVertices;
	std::vector<unsigned int> sphereIndices;
	generateSphere(sphereVertices, sphereIndices, 1.0f, 20, 20); // Радиус 1.0f, 20 стэков, 20 слайсов

	unsigned int sphereVAO, sphereVBO, sphereEBO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	//
	unsigned int VBO_marsh, VAO_marsh, EBO_marsh;   //буфер вершин, массив вершин, буфер индексный
	glGenVertexArrays(1, &VAO_marsh);
	glGenBuffers(1, &VBO_marsh);
	glGenBuffers(1, &EBO_marsh);

	glBindVertexArray(VAO_marsh); //загружаем данные

	glBindBuffer(GL_ARRAY_BUFFER, VBO_marsh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_marsh);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //устанавливаем указатель
	glEnableVertexAttribArray(0);
	// Цветовые атрибуты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // указатель на цвет
	glEnableVertexAttribArray(1);
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Цветовые атрибуты
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int texture1;

	// Текстура
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Загрузка изображения, создание текстуры и генерирование мипмап-уровней
	int width, height, nrChannels;
	unsigned char* data = SOIL_load_image("green.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	unsigned int texture2;
	// Текстура 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data2 = SOIL_load_image("beach.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Указываем OpenGL, какой сэмплер к какому текстурному блоку принадлежит (это нужно сделать единожды)
	lightingShader.use();
	lightingShader.setInt("texture", 0);
	lightingShader.setInt("texture2", 1);



	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom + 50), 0.4f / 0.3f, 1.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// Рендеринг комнаты
		lightingShader.use();
		glBindTexture(GL_TEXTURE_2D, texture2);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 1000);

		// Рендеринг фигуры
		glBindTexture(GL_TEXTURE_2D, texture1);
		lightingShader.setVec3("lightColor", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setInt("texture_diffuse1", 0);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_marsh);
		glBindVertexArray(VAO_marsh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_marsh);
		glDrawArrays(GL_TRIANGLES, 0, 1000);

		// Рендеринг источника свет
		//lampShader.use();
		//lampShader.setMat4("projection", projection);
		//lampShader.setMat4("view", view);
		//model = glm::mat4(0.4f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.1f));
		//lampShader.setMat4("model", model);
		//glBindVertexArray(lightVAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Добавьте эту строку
		//glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
		
		// experemental
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f)); // Масштабируем источник света для визуализации
		lampShader.setMat4("model", model);

		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
		//

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO_marsh);
	glDeleteBuffers(1, &VBO_marsh);

	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP))
		height += 0.025f;
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		height -= 0.025f;
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна.
	// Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}


// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
