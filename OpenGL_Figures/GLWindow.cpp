#include "GLWindow.h"

GLWindow::GLWindow(const std::string& title, uint32_t width, uint32_t height) : 
	handle(nullptr),
	keyCallback(nullptr)
{
	lastMouse.xPos = 0;
	lastMouse.yPos = 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

	if (handle == nullptr)
	{
		throw new std::runtime_error("Failed to create window");
	}
	glfwMakeContextCurrent(handle);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw new std::runtime_error("Failed to initialize GLEW");
	}

	glfwSetWindowUserPointer(handle, this);
	glfwSetKeyCallback(handle, internalKeyCallback);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(handle);
}

void GLWindow::internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto handle = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
	if (handle->keyCallback != nullptr)
	{
		handle->keyCallback((KeyCode)key, (Action)action, (Modifier)mods);
	}
}

uint32_t GLWindow::getWidth() const
{
	int width, height;
	glfwGetFramebufferSize(handle, &width, &height);
	return width;
}

uint32_t GLWindow::getHeigth() const
{
	int width, height;
	glfwGetFramebufferSize(handle, &width, &height);
	return height;
}

void GLWindow::setKeyCallback(const KeyCallback& _keyCallback)
{
	keyCallback = _keyCallback;
}

GLFWwindow* GLWindow::getGLFWHandle()
{
	return handle;
}

void GLWindow::hideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void GLWindow::showConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

void GLWindow::setFullScreen()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_FALSE);
	glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	glfwSwapInterval(1);
}

void GLWindow::setHalfScreenWindowed()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowMonitor(handle, nullptr, mode->width / 4, mode->height / 4, mode->width / 2, mode->height / 2, mode->refreshRate);
	glfwSwapInterval(1);
}

bool GLWindow::requestFilePath(char* path)
{
	OPENFILENAMEA ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Ascii Stereolithography (.stl)\0*.stl";
	ofn.lpstrFile = path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = "Privet, let me open your file...";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	return GetOpenFileNameA(&ofn);
}
