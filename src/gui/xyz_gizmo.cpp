#include "xyz_gizmo.hpp"

#include "shaders/xyz_gizmo_shaders.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>


XYZGizmo::XYZGizmo()
	: vertices{
		// vertices		// colors
		0, 0, 0,		1, 1, 1,
		0.1, 0, 0,		1.0, 0, 0,
		0, 0.1, 0,		0, 1.0, 0,
		0, 0, 0.1,		0, 0, 1.0
	},
	indices{
		0, 1,
		0, 2,
		0, 3
	}, shader_program(nullptr) {}

XYZGizmo::~XYZGizmo() {
	this->VAO.destroy();
	this->VBO.destroy();
	this->EBO.destroy();
	if(this->shader_program) delete this->shader_program;
}

void XYZGizmo::create() {
    this->VAO.create();
    this->VAO.bind();

    this->VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->VBO.create();
    this->VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->VBO.bind();
    this->VBO.allocate(this->vertices, sizeof(this->vertices));

    this->EBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this->EBO.create();
    this->EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->EBO.bind();
    this->EBO.allocate(this->indices, sizeof(this->indices));

	this->shader_program = new QOpenGLShaderProgram();
	this->shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, xyz_gizmo_vert_src);
	this->shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, xyz_gizmo_frag_src);
	this->shader_program->link();

    this->shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    this->shader_program->enableAttributeArray(0);

    this->shader_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    this->shader_program->enableAttributeArray(1);
}

void XYZGizmo::render() {
	QMatrix4x4 model;

	QMatrix4x4 view;
	float r = 3;
	using namespace std::chrono;
	double t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	view.lookAt(QVector3D(std::sin(t/1000) * r, 1, std::cos(t/1000) * r), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

	QMatrix4x4 projection;
	projection.perspective(45, 4/3, 0.1, 100);

    this->shader_program->bind();
	int model_loc = this->shader_program->uniformLocation("model");
	int view_loc = this->shader_program->uniformLocation("view");
	int proj_loc = this->shader_program->uniformLocation("projection");
	this->shader_program->setUniformValue(model_loc, model);
	this->shader_program->setUniformValue(view_loc, view);
	this->shader_program->setUniformValue(proj_loc, projection);
	this->VAO.bind();
	QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
	glFuncs->glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, nullptr);
	this->VAO.release();
	this->shader_program->release();
}
