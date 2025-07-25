#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
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
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3native.h>

#define XR_USE_PLATFORM_WIN32
#define XR_USE_GRAPHICS_API_OPENGL
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#define CHECK_XR(call) \
    do { \
        XrResult result = call; \
        if (XR_FAILED(result)) { \
            printf("XR call failed: %s = %d\n", #call, result); \
        } \
    } while(0)


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
	size_t currentSize;

	buffer() : buff(0), currentSize(0) {
		glGenBuffers(1, &buff);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR || buff == 0) {
			printf("Failed to generate buffer: 0x%x\n", error);
		}
	}

	void loadbuff(void* buffr, int size) {
		if (buff == 0) {
			printf("Buffer not initialized\n");
			return;
		}

		if (buffr == nullptr) {
			printf("Null buffer data pointer\n");
			return;
		}

		if (size <= 0) {
			printf("Invalid buffer size: %d\n", size);
			return;
		}

		// Check if we have a valid OpenGL context
		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		if (glGetError() != GL_NO_ERROR) {
			printf("No valid OpenGL context for buffer operations\n");
			return;
		}

		glBindBuffer(GL_ARRAY_BUFFER, buff);
		GLenum bindError = glGetError();
		if (bindError != GL_NO_ERROR) {
			printf("Error binding buffer: 0x%x\n", bindError);
			return;
		}

		// Use GL_STREAM_DRAW for frequently updated data
		glBufferData(GL_ARRAY_BUFFER, size, buffr, GL_DYNAMIC_DRAW);
		GLenum dataError = glGetError();
		if (dataError != GL_NO_ERROR) {
			printf("Error loading buffer data (size: %d): 0x%x\n", size, dataError);

			// Try to diagnose the error
			if (dataError == GL_OUT_OF_MEMORY) {
				printf("  - Out of GPU memory\n");
			}
			else if (dataError == GL_INVALID_VALUE) {
				printf("  - Invalid buffer size or usage\n");
			}
			return;
		}

		currentSize = size;

		// Unbind buffer to avoid state pollution
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool isValid() const {
		return buff != 0;
	}

	size_t getSize() const {
		return currentSize;
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

static const XrPosef XR_POSE_IDENTITY = {
  {0,0,0,1},   // orientation
  {0,0,0}      // position
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

	XrInstance                           xrInstance{ XR_NULL_HANDLE };
	XrSession                            xrSession{ XR_NULL_HANDLE };
	XrSystemId                           xrSystemId{ XR_NULL_SYSTEM_ID };
	XrSpace                              xrAppSpace{ XR_NULL_HANDLE };
	std::vector<XrViewConfigurationView> viewConfigViews;
	std::vector<XrSwapchain>             swapchains;
	std::vector<std::vector<XrSwapchainImageOpenGLKHR>> swapchainImages;
	std::vector<std::vector<GLuint>> swapchainDepthBuffers;
	std::vector<std::vector<GLuint>>     swapchainFramebuffers;
	std::vector<XrView>                  xrViews;
	std::vector<XrCompositionLayerProjectionView> projLayerViews;
	XrFrameState                         frameState{ XR_TYPE_FRAME_STATE };
	float                                nearZ{ 0.1f }, farZ{ 1000.0f };
	int                                  eyeWidth{ 0 }, eyeHeight{ 0 };
	glm::mat4                            projMat[2], viewMat[2];

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

		// --- 1) Create XR Instance ---
		XrInstanceCreateInfo instCI{ XR_TYPE_INSTANCE_CREATE_INFO };
		strcpy(instCI.applicationInfo.applicationName, "OpenXR GLFW Example");
		instCI.applicationInfo.applicationVersion = 1;
		strcpy(instCI.applicationInfo.engineName, "CustomEngine");
		instCI.applicationInfo.engineVersion = 1;
		instCI.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
		const char* exts[] = { XR_KHR_OPENGL_ENABLE_EXTENSION_NAME };
		instCI.enabledExtensionCount = 1;
		instCI.enabledExtensionNames = exts;
		xrCreateInstance(&instCI, &xrInstance);

		// --- 2) Get SystemId for HMD ---
		XrSystemGetInfo sysGetInfo{ XR_TYPE_SYSTEM_GET_INFO };
		sysGetInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
		xrGetSystem(xrInstance, &sysGetInfo, &xrSystemId);

		// --- 3) Check GL requirements ---
		PFN_xrGetOpenGLGraphicsRequirementsKHR pfnReq = nullptr;
		xrGetInstanceProcAddr(xrInstance,
			"xrGetOpenGLGraphicsRequirementsKHR",
			(PFN_xrVoidFunction*)&pfnReq);
		XrGraphicsRequirementsOpenGLKHR glReq{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };
		pfnReq(xrInstance, xrSystemId, &glReq);
		// (optional) check_opengl_version(&glReq);

		// --- 4) Create Session with graphics binding ---
		XrGraphicsBindingOpenGLWin32KHR gfxBinding{
		  XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR
		};
		gfxBinding.hDC = GetDC(glfwGetWin32Window(window));
		gfxBinding.hGLRC = glfwGetWGLContext(window);

		XrSessionCreateInfo sessCI{ XR_TYPE_SESSION_CREATE_INFO };
		sessCI.next = &gfxBinding;
		sessCI.systemId = xrSystemId;
		xrCreateSession(xrInstance, &sessCI, &xrSession);

		// --- 5) Create a local reference space ---
		XrReferenceSpaceCreateInfo spaceCI{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
		spaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
		spaceCI.poseInReferenceSpace = XR_POSE_IDENTITY;
		xrCreateReferenceSpace(xrSession, &spaceCI, &xrAppSpace);

		// --- 6) Enumerate stereo view configuration ---
		uint32_t viewCount = 0;
		xrEnumerateViewConfigurationViews(
			xrInstance, xrSystemId,
			XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
			0, &viewCount, nullptr);
		viewConfigViews.resize(viewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
		xrEnumerateViewConfigurationViews(
			xrInstance, xrSystemId,
			XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
			viewCount, &viewCount,
			viewConfigViews.data());

		eyeWidth = viewConfigViews[0].recommendedImageRectWidth;
		eyeHeight = viewConfigViews[0].recommendedImageRectHeight;

		// --- 7) Create one swapchain per eye ---
		swapchains.resize(viewCount);
		swapchainImages.resize(viewCount);
		swapchainFramebuffers.resize(viewCount);
		swapchainDepthBuffers.resize(viewCount);
		xrViews.resize(viewCount, { XR_TYPE_VIEW });
		projLayerViews.resize(viewCount, { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW });

		for (uint32_t i = 0; i < viewCount; i++) {
			// Pick preferred format
			uint32_t fmtCount = 0;
			xrEnumerateSwapchainFormats(xrSession, 0, &fmtCount, nullptr);
			std::vector<int64_t> fmts(fmtCount);
			xrEnumerateSwapchainFormats(xrSession, fmtCount, &fmtCount, fmts.data());

			int64_t chosenFmt = GL_SRGB8_ALPHA8;
			if (!std::count(fmts.begin(), fmts.end(), chosenFmt)) {
				// Try common fallback formats
				std::vector<int64_t> fallbacks = { GL_RGBA8, GL_RGB8, GL_RGBA16F };
				chosenFmt = fmts[0]; // Default fallback
				for (auto fallback : fallbacks) {
					if (std::count(fmts.begin(), fmts.end(), fallback)) {
						chosenFmt = fallback;
						break;
					}
				}
			}
			printf("Using color format: 0x%llx for eye %d\n", chosenFmt, i);

			XrSwapchainCreateInfo sci{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
			sci.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
			sci.format = chosenFmt;
			sci.sampleCount = viewConfigViews[i].recommendedSwapchainSampleCount;
			sci.width = eyeWidth;
			sci.height = eyeHeight;
			sci.faceCount = 1;
			sci.arraySize = 1;
			sci.mipCount = 1;

			XrResult createResult = xrCreateSwapchain(xrSession, &sci, &swapchains[i]);
			if (XR_FAILED(createResult)) {
				printf("Failed to create swapchain for eye %d: %d\n", i, createResult);
				continue;
			}

			// Get swapchain images
			uint32_t nImages = 0;
			xrEnumerateSwapchainImages(swapchains[i], 0, &nImages, nullptr);
			swapchainImages[i].resize(nImages, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR });
			xrEnumerateSwapchainImages(swapchains[i], nImages, &nImages,
				(XrSwapchainImageBaseHeader*)swapchainImages[i].data());

			printf("Eye %d: Created %d swapchain images\n", i, nImages);

			// Create framebuffers and depth buffers
			swapchainFramebuffers[i].resize(nImages);
			swapchainDepthBuffers[i].resize(nImages);

			glGenFramebuffers((GLsizei)nImages, swapchainFramebuffers[i].data());
			glGenRenderbuffers((GLsizei)nImages, swapchainDepthBuffers[i].data());

			// Check for OpenGL errors after generation
			GLenum genError = glGetError();
			if (genError != GL_NO_ERROR) {
				printf("OpenGL error after generating buffers for eye %d: 0x%x\n", i, genError);
			}

			// Setup each framebuffer
			for (size_t j = 0; j < nImages; j++) {
				// Verify swapchain image is valid
				if (swapchainImages[i][j].image == 0) {
					printf("Invalid swapchain image %zu for eye %d\n", j, i);
					continue;
				}

				// Setup depth buffer first
				glBindRenderbuffer(GL_RENDERBUFFER, swapchainDepthBuffers[i][j]);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, eyeWidth, eyeHeight);

				GLenum depthError = glGetError();
				if (depthError != GL_NO_ERROR) {
					printf("Error creating depth buffer %zu for eye %d: 0x%x\n", j, i, depthError);
				}

				// Setup framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, swapchainFramebuffers[i][j]);

				// Attach color texture
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_2D, swapchainImages[i][j].image, 0);

				GLenum colorError = glGetError();
				if (colorError != GL_NO_ERROR) {
					printf("Error attaching color texture %zu for eye %d: 0x%x\n", j, i, colorError);
				}

				// Attach depth buffer
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_RENDERBUFFER, swapchainDepthBuffers[i][j]);

				GLenum depthAttachError = glGetError();
				if (depthAttachError != GL_NO_ERROR) {
					printf("Error attaching depth buffer %zu for eye %d: 0x%x\n", j, i, depthAttachError);
				}

				// Check framebuffer completeness
				GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (status != GL_FRAMEBUFFER_COMPLETE) {
					printf("Framebuffer %zu for eye %d incomplete: 0x%x\n", j, i, status);

					// Debug incomplete framebuffer
					switch (status) {
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						printf("  - GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						printf("  - GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
						printf("  - GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
						printf("  - GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
						break;
					case GL_FRAMEBUFFER_UNSUPPORTED:
						printf("  - GL_FRAMEBUFFER_UNSUPPORTED\n");
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
						printf("  - GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
						printf("  - GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
						break;
					default:
						printf("  - Unknown framebuffer error: 0x%x\n", status);
						break;
					}

					// Try to get more info about attachments
					GLint colorAttachmentType, depthAttachmentType;
					glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
						GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &colorAttachmentType);
					glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &depthAttachmentType);

					printf("  - Color attachment type: 0x%x, Depth attachment type: 0x%x\n",
						colorAttachmentType, depthAttachmentType);
				}
				else {
					printf("Framebuffer %zu for eye %d setup successfully\n", j, i);
				}
			}

			// Cleanup - unbind everything
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			printf("Completed setup for eye %d with %zu framebuffers\n", i, nImages);
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
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {return ((oknogl*)(glfwGetWindowUserPointer(window)))->scroll_callback(window, xoffset, yoffset); });
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
	float* scrool = nullptr;
	float* scroolshift = nullptr;
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
		if (przyspmult > 10) {
			if(scroolshift) {
				*scroolshift *= 1 + yoffset / 100;
			}
			else {
				printf("scroll shift %f\n", yoffset);
			}
		}
		else {
		if (scrool) {
			*scrool *= 1 + yoffset / 100;
		}
		else {
			printf("scroll %f\n", yoffset);
		}
		}
	}
	void startframe(shader s) {

		printf("Frame %lld: shouldRender=%s, views=%zu\n",
			frameState.predictedDisplayTime,
			frameState.shouldRender ? "true" : "false",
			xrViews.size());

		glfwPollEvents();

		// 1) handle XR events (session start/stop)
		XrEventDataBuffer ev{ XR_TYPE_EVENT_DATA_BUFFER };
		while (xrPollEvent(xrInstance, &ev) == XR_SUCCESS) {
			if (ev.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED) {
				auto* ss = (XrEventDataSessionStateChanged*)&ev;
				if (ss->state == XR_SESSION_STATE_READY) {
					XrSessionBeginInfo sessionBeginInfo = {
						XR_TYPE_SESSION_BEGIN_INFO, // type  
						nullptr,                    // next  
						XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO // primary stereo view configuration  
					};
					xrBeginSession(xrSession, &sessionBeginInfo);
				}
				if (ss->state == XR_SESSION_STATE_STOPPING) {
					xrEndSession(xrSession);
				}
			}
			ev.type = XR_TYPE_EVENT_DATA_BUFFER;
		}

		// 2) xrWaitFrame + xrBeginFrame
		xrWaitFrame(xrSession, nullptr, &frameState);
		xrBeginFrame(xrSession, nullptr);

		// 3) locateViews → head‐tracked poses + FOVs
		XrViewLocateInfo vli{ XR_TYPE_VIEW_LOCATE_INFO };
		vli.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
		vli.displayTime = frameState.predictedDisplayTime;
		vli.space = xrAppSpace;

		XrViewState vs{ XR_TYPE_VIEW_STATE };
		uint32_t     actualCount = (uint32_t)xrViews.size();
		xrLocateViews(xrSession, &vli, &vs, actualCount, &actualCount,
			xrViews.data());


		// 4) build glm projection & view matrices
		for (uint32_t i = 0; i < actualCount; i++) {
			auto& xv = xrViews[i];
			float l = -xv.fov.angleLeft * nearZ;
			float r = xv.fov.angleRight * nearZ;
			float b = -xv.fov.angleDown * nearZ;
			float u = xv.fov.angleUp * nearZ;
			projMat[i] = glm::frustum(l, r, b, u, nearZ, farZ);

			// pose → mat4
			glm::quat q{
			  xv.pose.orientation.w,
			  xv.pose.orientation.x,
			  xv.pose.orientation.y,
			  xv.pose.orientation.z
			};
			glm::mat4 rot = glm::mat4_cast(q);
			glm::mat4 trans = glm::translate(
				glm::mat4(1.0f),
				glm::vec3(xv.pose.position.x,
					xv.pose.position.y,
					xv.pose.position.z));
			viewMat[i] = glm::inverse(trans * rot);
		}

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
	void renderTestPoints() {
		// Test if points render at all
		glPointSize(20.0f);
		glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -2.0f);
		glVertex3f(1.0f, 0.0f, -2.0f);
		glVertex3f(-1.0f, 0.0f, -2.0f);
		glEnd();
	}
	void draw(int ed,int type=GL_TRIANGLES,int st=0) {
		glDrawArrays(type, st, ed);
	}
	void drawXR(int ed, int type = GL_TRIANGLES, int st = 0) {
		// Early exit if no vertices
		if (ed <= 0) return;

		// Check if frameState indicates we should render
		if (!frameState.shouldRender) {
			printf("Frame state indicates we shouldn't render\n");
			return;
		}

		// Store current OpenGL state
		GLint oldFramebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFramebuffer);

		for (uint32_t eye = 0; eye < xrViews.size() && eye < swapchains.size(); eye++) {
			// 1) Acquire swapchain image
			uint32_t imgIndex = 0;
			XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
			XrResult acquireResult = xrAcquireSwapchainImage(swapchains[eye], &acquireInfo, &imgIndex);
			if (XR_FAILED(acquireResult)) {
				printf("Failed to acquire swapchain image for eye %d: %d\n", eye, acquireResult);
				continue;
			}

			// Check if image index is valid
			if (imgIndex >= swapchainFramebuffers[eye].size()) {
				printf("Invalid image index %d for eye %d (max: %zu)\n", imgIndex, eye, swapchainFramebuffers[eye].size());
				XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
				xrReleaseSwapchainImage(swapchains[eye], &releaseInfo);
				continue;
			}

			// 2) Wait for swapchain image
			XrSwapchainImageWaitInfo waitInfo{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
			waitInfo.timeout = 100000000; // 100ms timeout (reduced from 1s)
			XrResult waitResult = xrWaitSwapchainImage(swapchains[eye], &waitInfo);
			if (XR_FAILED(waitResult)) {
				printf("Failed to wait for swapchain image for eye %d: %d\n", eye, waitResult);
				XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
				xrReleaseSwapchainImage(swapchains[eye], &releaseInfo);
				continue;
			}

			// 3) Bind the pre-configured framebuffer (DON'T re-attach textures)
			GLuint fbo = swapchainFramebuffers[eye][imgIndex];
			if (fbo == 0) {
				printf("Invalid framebuffer for eye %d, image %d\n", eye, imgIndex);
				XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
				xrReleaseSwapchainImage(swapchains[eye], &releaseInfo);
				continue;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			// Quick framebuffer check (optional, remove if performance is critical)
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				printf("Framebuffer not complete for eye %d, image %d: 0x%x\n", eye, imgIndex, status);
				glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffer);
				XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
				xrReleaseSwapchainImage(swapchains[eye], &releaseInfo);
				continue;
			}

			// 4) Set viewport and clear
			glViewport(0, 0, eyeWidth, eyeHeight);

			// Clear with a subtle eye-specific tint for debugging
			glClearColor(eye * 0.05f, 0.0f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// 5) Set matrices and uniforms
			glm::mat4 MVP = projMat[eye] * viewMat[eye];
			glUniformMatrix4fv(uniMVP, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(uniV, 1, GL_FALSE, &viewMat[eye][0][0]);

			// Update per-eye uniforms
			glm::vec3 eyePos = glm::vec3(xrViews[eye].pose.position.x,
				xrViews[eye].pose.position.y,
				xrViews[eye].pose.position.z);
			glUniform3f(viewPos, eyePos.x, eyePos.y, eyePos.z);
			glUniform3f(unilpos, lightpos.x, lightpos.y, lightpos.z);
			glUniform3f(unilcol, lightcolor.x, lightcolor.y, lightcolor.z);
			glUniform1f(unilpow, lightpower);

			// 6) Set OpenGL state for rendering
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			if (type == GL_POINTS) {
				glEnable(GL_PROGRAM_POINT_SIZE);
			}

			// 7) Render
			glDrawArrays(type, st, ed);

			// Check for rendering errors
			GLenum renderError = glGetError();
			if (renderError != GL_NO_ERROR) {
				printf("OpenGL error during rendering eye %d: 0x%x\n", eye, renderError);
			}

			// 8) Unbind framebuffer before release
			glBindFramebuffer(GL_FRAMEBUFFER, oldFramebuffer);

			// 9) Release swapchain image
			XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
			XrResult releaseResult = xrReleaseSwapchainImage(swapchains[eye], &releaseInfo);
			if (XR_FAILED(releaseResult)) {
				printf("Failed to release swapchain image for eye %d: %d\n", eye, releaseResult);
			}

			// 10) Setup composition layer for this eye
			projLayerViews[eye].pose = xrViews[eye].pose;
			projLayerViews[eye].fov = xrViews[eye].fov;
			projLayerViews[eye].subImage.swapchain = swapchains[eye];
			projLayerViews[eye].subImage.imageArrayIndex = 0;
			projLayerViews[eye].subImage.imageRect.offset = { 0, 0 };
			projLayerViews[eye].subImage.imageRect.extent = { eyeWidth, eyeHeight };
		}
	}
	void wyswietl() {
		XrCompositionLayerProjection layer{
			XR_TYPE_COMPOSITION_LAYER_PROJECTION
		};
		layer.space = xrAppSpace;
		layer.viewCount = (uint32_t)projLayerViews.size();
		layer.views = projLayerViews.data();

		const XrCompositionLayerBaseHeader* layers[] = {
		  (const XrCompositionLayerBaseHeader*)&layer
		};
		XrFrameEndInfo xrfei{
			  XR_TYPE_FRAME_END_INFO,nullptr,
			  frameState.predictedDisplayTime, 
			  XR_ENVIRONMENT_BLEND_MODE_OPAQUE,
			  1, layers
		};
		xrEndFrame(xrSession,
			&xrfei);
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};



