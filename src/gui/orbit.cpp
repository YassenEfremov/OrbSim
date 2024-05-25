#include "orbit.hpp"

#include "shaders/orbit_shaders.hpp"
#include "simulation/satellite.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

#include <cstddef>


Orbit::Orbit() : data(nullptr), points(0), shader_program(nullptr) {}

Orbit::~Orbit() {
    if (this->data) delete[] this->data;

	this->VAO.destroy();
	this->VBO.destroy();
	if(this->shader_program) delete this->shader_program;
}

void Orbit::create() {
    this->VAO.create();
    this->VAO.bind();

    this->VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->VBO.create();
    this->VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->VBO.bind();
    this->VBO.allocate(this->data, this->points * sizeof(float));

	this->shader_program = new QOpenGLShaderProgram();
	this->shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, orbit_vert_src);
	this->shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, orbit_frag_src);
	this->shader_program->link();

    this->shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    this->shader_program->enableAttributeArray(0);
}

void Orbit::render() {
	QMatrix4x4 model;

	QMatrix4x4 view;
	float r = 3;
	using namespace std::chrono;
	double t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	view.lookAt(QVector3D(std::sin(t/1000) * r, 1, std::cos(t/1000) * r), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

	QMatrix4x4 projection;
	projection.perspective(45, 4/3, 0.1, 100);

    this->shader_program->bind();
	int color_loc = this->shader_program->uniformLocation("color");
	int model_loc = this->shader_program->uniformLocation("model");
	int view_loc = this->shader_program->uniformLocation("view");
	int proj_loc = this->shader_program->uniformLocation("projection");
	this->shader_program->setUniformValue(color_loc, QVector3D(1, 1, 1));
	this->shader_program->setUniformValue(model_loc, model);
	this->shader_program->setUniformValue(view_loc, view);
	this->shader_program->setUniformValue(proj_loc, projection);
    this->VAO.bind();
	QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glDrawArrays(GL_POINTS, 0, this->points);
	this->VAO.release();
	this->shader_program->release();
}

void Orbit::update_points(orbsim::SimData sim_data) {

	if (this->data) delete[] this->data;
	this->points = 3 * sim_data.steps;
	this->data = new float[this->points];

    for (int i = 0; i < sim_data.steps; i++) {
		// y and z are swapped because OpenGL has the z axis pointing up
		this->data[3*i + 0] = 0.6 * sim_data.pos_arr[i].x / 10000;		// temporary
		this->data[3*i + 1] = 0.6 * sim_data.pos_arr[i].z / 10000;		// can't use norm tho
		this->data[3*i + 2] = 0.6 * sim_data.pos_arr[i].y / 10000;
		// std::cout << data_f[3*i + 0] << " " << data_f[3*i + 1] << " " << data_f[3*i + 2] << "\n";
    }

	QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
	this->VBO.bind();
	glFuncs->glBufferData(GL_ARRAY_BUFFER, points * sizeof(float), data, GL_STATIC_DRAW);
	this->VBO.release();
}
