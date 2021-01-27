#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 300.0f, 800.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
bool playPuerta = false;
bool playAzul = false;
bool playEstufa = false;

float range = 0.0f;
float rotAzul = 0.0f;
float rot = 0.0f;
float rotPuerta = 0.0f;
float rotEstufa = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

//Animaci�n del mouse
float movMouseX = 0.0; //Eje Y
float movMouseZ = 0.0; //Eje X

//Estados del Muse
bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;

//Animaci�n del Libro
float movBookX = 0.0; //Eje Y
float movBookY = 0.0; //Eje X

//Estados del Muse
bool circuitoBook = false;
bool recorridoB1 = true;
bool recorridoB2 = false;
bool recorridoB3 = false;
bool recorridoB4 = false;
bool recorridoB5 = false;
bool recorridoB6 = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");


	//Modelos Estaticos
	Model Cosas((char*)"Models/MueblesEstaticos.obj");
	Model Casa((char*)"Models/Casa.obj");

	//Modelos Animados
	Model CompuMouse((char*)"Models/CompuMouse.obj");
	Model DescansoAzul((char*)"Models/DescansoAzul.obj");
	Model Libro((char*)"Models/Libro.obj");
	Model Puerta((char*)"Models/Puerta.obj");
	Model PuertaEstufa((char*)"Models/PuertaEstufa.obj");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	//Vista que tiene la c�mara, que tanto puede ver
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.5f, 2500.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(1.0f, 0.1f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 600.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, -1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1 - Foco 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.79f, 0.83f, 0.00f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.79f, 0.83f, 0.00f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0002f);

		// Point light 2 - Foco 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.04f, 0.85f, 0.85f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.04f, 0.85f, 0.85f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0002f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, 0.0f, 0.0);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.79f, 0.83f, 0.00f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.79f, 0.83f, 0.00f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0002f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 600.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glm::mat4 tmp1 = glm::mat4(1.0f); //Pivote1 - Puerta
		glm::mat4 tmp2 = glm::mat4(1.0f); //Pivote2 - SillaAzul
		glm::mat4 tmp3 = glm::mat4(1.0f); //Pivote3 - EstufaPuerta
		glm::mat4 tmp4 = glm::mat4(1.0f); //Pivote4 - Mouse
		glm::mat4 tmp5 = glm::mat4(1.0f); //Pivote5 - Libro

		//Cargar modelos estaticos
		//Casa
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		tmp1 = tmp2 = tmp3 = tmp4 = tmp5 = model = glm::translate(model, glm::vec3(0, 0, -500));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Casa.Draw(lightingShader);

		//Muebles
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cosas.Draw(lightingShader);


		//-------------Objetos con mivimiento
		//Simples
		//DescansoAzul
		view = camera.GetViewMatrix();
		model = glm::translate(tmp1, glm::vec3(-170.0f, 42.0f, 267.5f));
		model = glm::rotate(model, glm::radians(-rotAzul), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DescansoAzul.Draw(lightingShader);

		//Puerta
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp2, glm::vec3(104.5f, 2.0f, 698.0f));
		model = glm::rotate(model, glm::radians(-rotPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		//PuertaEstufa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp3, glm::vec3(314.0f, 18.0f, 527.0f));
		model = glm::rotate(model, glm::radians(-rotEstufa), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PuertaEstufa.Draw(lightingShader);


		//Complejos
		//Mouse
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp4, glm::vec3(100.0f + movMouseX, 81.5f, -257.0f + movMouseZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CompuMouse.Draw(lightingShader);

		//Libro
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp5, glm::vec3(100.0f + movBookX, 170.0f + movBookY, -560.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Libro.Draw(lightingShader);
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}


void animacion()
{
	//Puerta
	if (playPuerta) {
		if (rotPuerta <= 90) {
			rotPuerta += 1.0f;
		}
		else if (rotPuerta == 90){
			playPuerta = false;
		}
	}
	else {
	    if (rotPuerta >= 0) {
			rotPuerta -= 1.0f;
		}
		else if (rotPuerta == 0){
			playPuerta = true;
		}
	}

	//Sillon Azul
	if (playAzul) {
		if (rotAzul <= 90) {
			rotAzul += 1.0f;
		}
		else if (rotAzul == 90) {
			playAzul = false;
		}
	}
	else {
		if (rotAzul >= 0) {
			rotAzul -= 1.0f;
		}
		else if (rotAzul == 0) {
			playAzul = true;
		}
	}

	//Estufa
	if (playEstufa) {
		if (rotEstufa <= 70) {
			rotEstufa += 1.0f;
		}
		else if (rotEstufa == 70) {
			playEstufa = false;
		}
	}
	else {
		if (rotEstufa >= 0) {
			rotEstufa -= 1.0f;
		}
		else if (rotEstufa == 0) {
			playEstufa = true;
		}
	}

	//Movimiento del Mouse
	if (circuito)
	{
		if (recorrido1){
			movMouseX += 0.4f;
			if (movMouseX > 25){
				recorrido1 = false;
				recorrido4 = true;
			}
		}
		if (recorrido2){
			movMouseZ -= 0.4f;
			movMouseX += 0.4;
			if (movMouseZ < 9){
				recorrido2 = false;
				recorrido5 = true;
	
			}
		}
	
		if (recorrido3){
			movMouseX -= 0.4f;
			if (movMouseX < 0){
				recorrido3 = false;
				recorrido2 = true;
			}
		}
	
		if (recorrido4){
			movMouseZ += 0.4f;
			if (movMouseZ > 30){
				recorrido4 = false;
				recorrido3 = true;
			}
		}
		if (recorrido5){
			movMouseX -= 0.4f;
			if (movMouseX < 2){
				recorrido5 = false;
				recorrido1 = true;
			}
		}
	}

	//Libro
	if (circuitoBook)
	{
		if (recorridoB1) {
			movBookX -= 0.4f;
			if (movBookX < -40) {
				recorridoB1 = false;
				recorridoB2 = true;
			}
		}
		if (recorridoB2) {
			movBookY -= 0.4f;
			if (movBookY < -60) {
				recorridoB2 = false;
				recorridoB3 = true;
			}
		}
		if (recorridoB3) {
			movBookX += 0.4f;
			if (movBookX > 5) {
				recorridoB3 = false;
				recorridoB4 = true;
			}
		}

		if (recorridoB4) {
			movBookX -= 0.4f;
			if (movBookX < -40) {
				recorridoB4 = false;
				recorridoB5 = true;
			}
		}
		if (recorridoB5) {
			movBookY += 0.4f;
			if (movBookY > 0) {
				recorridoB5 = false;
				recorridoB6 = true;
			}
		}
		if (recorridoB6) {
			movBookX += 0.4f;
			if (movBookX > 0) {
				recorridoB6 = false;
				recorridoB1 = true;
			}
		}
	}
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//Puerta
	if (keys[GLFW_KEY_1]) {
		if (playPuerta == false) {
			rotPuerta = 0.0f;
			playPuerta = true;
		}
		else if (playPuerta == true) {
			rotPuerta = 90.0f;
			playPuerta = false;
		}
	}

	//Sillon Azul
	if (keys[GLFW_KEY_2]) {
		if (playAzul == false) {
			rotAzul = 0.0f;
			playAzul = true;
		}
		else if (playAzul == true) {
			rotAzul = 90.0f;
			playAzul = false;
		}
	}


	//Estufa
	if (keys[GLFW_KEY_3]) {
		if (playEstufa == false) {
			rotEstufa = 0.0f;
			playEstufa = true;
		}
		else if (playEstufa == true) {
			rotEstufa = 80.0f;
			playEstufa = false;
		}
	}

	
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Activacion Mouse
	if (keys[GLFW_KEY_6])
	{
		circuito = true;
	}

	if (keys[GLFW_KEY_7])
	{
		circuito = false;
	}

	//Activacion Libro
	if (keys[GLFW_KEY_8])
	{
		circuitoBook = true;
	}

	if (keys[GLFW_KEY_9])
	{
		circuitoBook = false;
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime*50);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime*50);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime*50);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime*50);
	}

}