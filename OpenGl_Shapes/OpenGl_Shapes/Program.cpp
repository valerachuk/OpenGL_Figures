#include "Program.h"

Program::Program() : 
	moveAxis(glm::vec2(0.0f)),
	moveSpeed(0.02f)
{
	glfwInit();
	glewInit();

	Window::getInstance().setKeyCallback(onKeyCallback); //first
	RenderSystem::getInstance().setShader(std::unique_ptr<Shader>(new Shader("../../shader/vertexShader.glsl", "../../shader/fragmentShader.glsl")));
}

void Program::onKeyCallback(KeyCode code, Action action, Modifier modif)
{
	glm::vec2& axis = Program::getInstance().moveAxis;
	if (action == Action::Press)
	{
		switch (code)
		{
		case KeyCode::W:
			axis.y = 1.0f;
			break;
		case KeyCode::A:
			axis.x = -1.0f;
			break;
		case KeyCode::S:
			axis.y = -1.0f;
			break;
		case KeyCode::D:
			axis.x = 1.0f;
			break;
		}
	}
	else if (action == Action::Release) {
		switch (code)
		{
		case KeyCode::W:
		case KeyCode::S:
			axis.y = 0.0f;
			break;
		case KeyCode::A:
		case KeyCode::D:
			axis.x = 0.0f;
			break;
		}
	}

	if (glm::length(axis) > 0)
		axis = glm::normalize(axis);

}

Program& Program::getInstance()
{
	static Program inst;
	return inst;
}

void Program::Start()
{
	ShapeUnion shapeUnion;
	Shape* shape1 = new Shape(std::shared_ptr<Buffer>(new Buffer(ShapeData::getTriangle())));
	shape1->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	shape1->setScale(glm::vec2(0.5f));
	shape1->setPos(glm::vec2(0.6f, 0.6f));
	shapeUnion.Add(std::unique_ptr<ICanvasComponent>(shape1));

	Shape* shape2 = new Shape(std::shared_ptr<Buffer>(new Buffer(ShapeData::getTriangle())));
	shapeUnion.Add(std::unique_ptr<ICanvasComponent>(shape2));


	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(Window::getInstance().getGLFWHandle()))
	{
		glfwPollEvents();

		RenderSystem::clearDisplay(1, 1, 1);
		shapeUnion.draw();
		shapeUnion.translate(moveAxis * moveSpeed);
		shapeUnion.clampCanvasFit();

		glfwSwapBuffers(Window::getInstance().getGLFWHandle());
	}
}
