#include "tess.h"

// Shader Programs
GLuint shaderProgram;

// Textures
GLuint screen;

// Variables used to determine framerate
double previousUpdateTime = 0.0;
double currentUpdateTime = 0.0;
double timeDeltaFramerate = 0.0;
int framesCounter = 0;
int avgCounter = 0;
double fpsTotal = 0.0;
double msTotal = 0.0;

std::vector<GLfloat> genNonSymPlaneUniform(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int div)
{
	std::vector<GLfloat> plane;

	glm::vec3 dir03 = (v3 - v0) / float(div);
	glm::vec3 dir12 = (v2 - v1) / float(div);

	// dir2 and dir3
	for (int i = 0; i < div + 1; i++)
	{
		// dir1
		for (int j = 0; j < div + 1; j++)
		{
			glm::vec3 acrossj = ((v1 + i * dir12) - (v0 + i * dir03)) / float(div);
			glm::vec3 crntVec = v0 + i * dir03 + j * acrossj;
			// Position
			plane.push_back(crntVec.x);
			plane.push_back(crntVec.y);
			plane.push_back(crntVec.z);
			// Tex UV
			plane.push_back(float(j) / div);
			plane.push_back(float(i) / div);
		}
	}

	return plane;
}

// Plane indices for triangle patches
std::vector<GLuint> genPlaneInd(int div)
{
	std::vector<GLuint> indices;

	for (int row = 0; row < div; row++)
	{
		for (int col = 0; col < div; col++)
		{
			int index = row * (div + 1) + col;
			// Top triangle								   ____
			indices.push_back(index);					// |  /
			indices.push_back(index + (div + 1) + 1);	// | /
			indices.push_back(index + (div + 1));		// |/
			// Bot triangle
			indices.push_back(index);					//   /|
			indices.push_back(index + 1);				//	/ |
			indices.push_back(index + (div + 1) + 1);	// /__|
		}
	}

	return indices;
}

// Plane indices for quad patches
std::vector<GLuint> genPlaneIndTes(int div)
{
	std::vector<GLuint> indices;

	for (int row = 0; row < div; row++)
	{
		for (int col = 0; col < div; col++)
		{
			int index = row * (div + 1) + col;			// 3 , 2
			indices.push_back(index);					//  
			indices.push_back(index + 1);				//  
			indices.push_back(index + (div + 1) + 1);	// 
			indices.push_back(index + (div + 1));		// 0 , 1
		}
	}

	return indices;
}

// Screen mesh
GLuint VAO, VBO, EBO;

// Quads
int divisions = 8;
std::vector<GLfloat> vertices = genNonSymPlaneUniform(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(-0.5f, 0.5f, 0.0f), divisions);
std::vector<GLuint> indices = genPlaneIndTes(divisions);

std::string readText(const char* textFile)
{
	string linesOfTheFile;
	ofBuffer buffer = ofBufferFromFile(textFile);
	for (auto line : buffer.getLines()) {
		linesOfTheFile.append(line);
		linesOfTheFile.append("\n");
	}
	return linesOfTheFile;
}

void TesselatorUtil::setup() {
	std::string vertexCode = readText("shaders/tessellation.vert");
	std::string fragmentCode = readText("shaders/tessellation.frag");
	std::string tesControlCode = readText("shaders/tessellation.tesc");
	std::string tesEvalCode = readText("shaders/tessellation.tese");

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	const char* tesControlSource = tesControlCode.c_str();
	const char* tesEvalSource = tesEvalCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLuint tesControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tesControlShader, 1, &tesControlSource, NULL);
	glCompileShader(tesControlShader);
	GLuint tesEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tesEvalShader, 1, &tesEvalSource, NULL);
	glCompileShader(tesEvalShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, tesControlShader);
	glAttachShader(shaderProgram, tesEvalShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(tesControlShader);
	glDeleteShader(tesEvalShader);


	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glNamedBufferData(VBO, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glNamedBufferData(EBO, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayAttribBinding(VAO, 1, 0);
	glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(GLfloat));
	glVertexArrayElementBuffer(VAO, EBO);

	glCreateTextures(GL_TEXTURE_2D, 1, &screen);
	glTextureParameteri(screen, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screen, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screen, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screen, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screen, 1, GL_RGBA32F, ofGetScreenWidth(), ofGetScreenHeight());
	glBindImageTexture(0, screen, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glPatchParameteri(GL_PATCH_VERTICES, 4); // Quads

	//glPatchParameteri(GL_PATCH_VERTICES, 3); // Triangles

	//glViewport(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
}

void TesselatorUtil::draw() {
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	glPatchParameteri(GL_PATCH_VERTICES, 4); // Quads

	glUseProgram(shaderProgram);
	glProgramUniform2f(shaderProgram, 0, (float)ofGetMouseX() / (float)ofGetWindowWidth(),
		1.0 - ((float)ofGetMouseY() / (float)ofGetWindowHeight()));

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, 0);
}