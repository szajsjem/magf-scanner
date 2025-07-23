#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>


#define GLGL

class gliniter {
public:
	gliniter() {
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
		}
	}
};
gliniter globalglinit;

template<class T>
void rot(T& a, T& b, T sinx, T cosx) {
	T ta = cosx * a - sinx * b;
	b = cosx * b + sinx * a;
	a = ta;
}




class shader {
public:
	GLuint ProgramID;
	shader(std::string vertexshaderpath,std::string fragmentshaderpath) {
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertexshaderpath, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << VertexShaderStream.rdbuf();
			VertexShaderCode = sstr.str();
			VertexShaderStream.close();
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragmentshaderpath, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << FragmentShaderStream.rdbuf();
			FragmentShaderCode = sstr.str();
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}



		// Compile Fragment Shader
		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}



		// Link the program
		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
	}
	shader(std::string vertexshaderpath,std::string fragmentshaderpath,std::string geometryshaderpath) {
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertexshaderpath, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << VertexShaderStream.rdbuf();
			VertexShaderCode = sstr.str();
			VertexShaderStream.close();
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragmentshaderpath, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << FragmentShaderStream.rdbuf();
			FragmentShaderCode = sstr.str();
			FragmentShaderStream.close();
		}

		std::string geometryShaderCode;
		std::ifstream geometryShaderStream(geometryshaderpath, std::ios::in);
		if (geometryShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << geometryShaderStream.rdbuf();
			geometryShaderCode = sstr.str();
			geometryShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;


		// Compile Vertex Shader
		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}



		// Compile geometry Shader
		char const* geometrySourcePointer = geometryShaderCode.c_str();
		glShaderSource(geometryShaderID, 1, &geometrySourcePointer, NULL);
		glCompileShader(geometryShaderID);

		// Check geometry Shader
		glGetShaderiv(geometryShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(geometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> geometryShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(geometryShaderID, InfoLogLength, NULL, &geometryShaderErrorMessage[0]);
			printf("%s\n", &geometryShaderErrorMessage[0]);
		}

		// Compile Fragment Shader
		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}

		// Link the program
		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, geometryShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);
		glDetachShader(ProgramID, geometryShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(geometryShaderID);
		glDeleteShader(FragmentShaderID);
	}
	shader(std::string computeshaderpath) {//?? working?
		GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);


		std::string ComputeShaderCode;
		std::ifstream ComputeShaderStream(computeshaderpath, std::ios::in);
		if (ComputeShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << ComputeShaderStream.rdbuf();
			ComputeShaderCode = sstr.str();
			ComputeShaderStream.close();
		}

		char const* ComputeSourcePointer = ComputeShaderCode.c_str();
		glShaderSource(ComputeShaderID, 1, &ComputeSourcePointer, NULL);
		glCompileShader(ComputeShaderID);

		ProgramID = glCreateProgram();
		glAttachShader(ProgramID, ComputeShaderID);
		glLinkProgram(ProgramID);

		glDetachShader(ProgramID, ComputeShaderID);

		glDeleteShader(ComputeShaderID);
	}
};

class buffer {
public:
	GLuint buff;
	buffer() {
		glGenBuffers(1, &buff);
	}
	void loadbuff(void *buffr,int size) {
		glBindBuffer(GL_ARRAY_BUFFER, buff);
		glBufferData(GL_ARRAY_BUFFER, size, buffr, GL_DYNAMIC_DRAW);
	}
};

class uniformbuffer {
public:
	GLuint buff;
	uniformbuffer(shader s,std::string name) {
		buff = glGetUniformLocation(s.ProgramID, name.data());
	}
};

class compsh {//?? working?

public:
	void start(shader s) {
		glUseProgram(s.ProgramID);
	}
	void setarg(int argnum, buffer b, int size, int type = GL_FLOAT) {
		glEnableVertexAttribArray(argnum);
		glBindBuffer(GL_ARRAY_BUFFER, b.buff);
		glVertexAttribPointer(
			argnum,
			size,
			type,
			GL_FALSE,
			0,
			(void*)0
		);
	}
	void compute(int xsize,int ysize,int zsize) {
		glDispatchCompute(xsize, ysize, zsize);
	}
};

class oknogl {
	double xold, yold;
	int wys, szer;
	glm::vec3 przysp;
	double przyspmult = 1;
	glm::vec3 predkosc;

	glm::mat4 projection;
	glm::vec3 headpos;
public:
	GLFWwindow* window;
	glm::vec3 playerpos;
	glm::vec3 viewkier;
	GLuint uniMVP;
	GLuint uniV;
	GLuint unilpos;
	glm::vec3 lightpos = glm::vec3(0, 10, 0);
	GLuint unilcol;
	GLuint viewPos;
	glm::vec3 lightcolor = glm::vec3(1, 1, 1);
	GLuint unilpow;
	float lightpower=1;
	GLuint uboTriangulation;
	oknogl() {
		playerpos = glm::vec3(0, 0, 0);
		viewkier = glm::vec3(1, 0, 0);
		headpos = glm::vec3(0, 1, 0);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		this->wys = mode->height;
		this->szer = mode->width;
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		window = glfwCreateWindow(szer, wys, "gl tste",monitor, NULL);

		if (window == NULL) {
			exit(1);
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
		}
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClearColor(0.0f, 0.0f, 0.02f, 0.0f);
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		glfwSetWindowUserPointer(window, this);
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window, &xold, &yold);
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {return ((oknogl*)(glfwGetWindowUserPointer(window)))->keycb(window, key, scancode, action, mods); });
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {return ((oknogl*)(glfwGetWindowUserPointer(window)))->mousecb(window, xpos,ypos); });


		glGenBuffers(1, &uboTriangulation);
		glBindBuffer(GL_UNIFORM_BUFFER, uboTriangulation);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(triangulation), triangulation, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void keycb(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_W) {
			if (action == GLFW_PRESS)przysp.r += 0.01;
			if (action == GLFW_RELEASE)przysp.r -= 0.01;
		}
		if (key == GLFW_KEY_S) { 
			if (action == GLFW_PRESS)przysp.r -= 0.01;
			if (action == GLFW_RELEASE)przysp.r += 0.01;
		}
		if (key == GLFW_KEY_A) { 
			if (action == GLFW_PRESS)przysp.b -= 0.01;
			if (action == GLFW_RELEASE)przysp.b += 0.01;
		}
		if (key == GLFW_KEY_D) { 
			if (action == GLFW_PRESS)przysp.b += 0.01;
			if (action == GLFW_RELEASE)przysp.b -= 0.01;
		}
		if (key == GLFW_KEY_LEFT_SHIFT) { 
			if (action == GLFW_PRESS)przysp.g -= 0.01;
			if (action == GLFW_RELEASE)przysp.g += 0.01;
		}
		if (key == GLFW_KEY_SPACE) { 
			if (action == GLFW_PRESS)przysp.g += 0.01;
			if (action == GLFW_RELEASE)przysp.g -= 0.01;
		}
		if (key == GLFW_KEY_LEFT_CONTROL) {
			if (action == GLFW_PRESS)przyspmult = 12;
			if (action == GLFW_RELEASE)przyspmult = 3;
		}
	}
	void mousecb(GLFWwindow* window, double xpos, double ypos) {
		double dy = (ypos - yold) / 1000;
		yold = ypos;
		ypos = viewkier.g;
		viewkier.g -= dy;
		if (viewkier.g >= 1)viewkier.g = 0.99999;
		if (viewkier.g <= -1)viewkier.g = -0.99999;
		dy = abs(viewkier.g) - abs(ypos);
		rot(viewkier.r, viewkier.b, (float)sin((xpos - xold) / 1000), (float)cos((xpos - xold) / 1000));
		double t = sqrt(viewkier.r * viewkier.r + viewkier.b * viewkier.b);
		viewkier.r -= viewkier.r * dy / t;
		viewkier.b -= viewkier.b * dy / t;
		xold = xpos;
	}
	void startframe(shader s) {
		glUseProgram(s.ProgramID);
		uniMVP = glGetUniformLocation(s.ProgramID, "MVP");
		uniV = glGetUniformLocation(s.ProgramID, "V");
		unilpos = glGetUniformLocation(s.ProgramID, "LightPosition_worldspace");
		viewPos = glGetUniformLocation(s.ProgramID, "viewPos");
		unilcol = glGetUniformLocation(s.ProgramID, "LightColor");
		unilpow = glGetUniformLocation(s.ProgramID, "LightPower");
		projection = glm::perspective(glm::radians(55.0f), szer / (float)wys, 0.1f, 10000.0f);
		glm::vec3 przyspt = przysp ;
		float t = sqrt(viewkier.r * viewkier.r + viewkier.b * viewkier.b);
		rot(przyspt.r, przyspt.b, viewkier.b/t, viewkier.r/t);
		przyspt *= przyspmult;
		predkosc += przyspt;
		predkosc *= 0.8;
		playerpos += predkosc;
		glm::mat4 View = glm::lookAt(
			playerpos,
			playerpos+viewkier,
			headpos 
		);
		glm::mat4 MVP = projection * View;
		lightpos = playerpos;
		glUniformMatrix4fv(uniMVP, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(uniV, 1, GL_FALSE, &View[0][0]);
		glUniform3f(unilpos, lightpos.x, lightpos.y, lightpos.z);
		glUniform3f(viewPos, playerpos.x, playerpos.y, playerpos.z);
		glUniform3f(unilcol, lightcolor.x, lightcolor.y, lightcolor.z);
		glUniform1f(unilpow, lightpower);
		{
			GLuint block_index = glGetUniformBlockIndex(s.ProgramID, "TriangulationTable");
			glUniformBlockBinding(s.ProgramID, block_index, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboTriangulation);
		}
	}
	void setarg(int argnum, buffer b,int size,int type=GL_FLOAT) {
		glEnableVertexAttribArray(argnum);
		glBindBuffer(GL_ARRAY_BUFFER, b.buff);
		glVertexAttribPointer(
			argnum,                 
			size,                  
			type,           
			GL_FALSE,           
			0,                  
			(void*)0            
		);
	}
	void draw(int ed,int type=GL_TRIANGLES,int st=0) {
		glDrawArrays(type, st, ed);
	}
	void wyswietl() {
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};



