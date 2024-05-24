#include "output_window.hpp"

#include "main_window.hpp"
#include "shaders/orbit_shaders.hpp"
#include "shaders/xyz_gizmo_shaders.hpp"
#include "simulation/satellite.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

#include <iostream>
#include <cmath>
#include <chrono>


OutputWindow::OutputWindow(QWidget *parent)
	: QOpenGLWidget(parent) {

	this->data_f = nullptr;
	this->points = 0;

    auto main_ui = static_cast<MainWindow *>(parent->parent());
    connect(main_ui, &MainWindow::new_data,
            this, &OutputWindow::set_data);
}

OutputWindow::~OutputWindow() {
    if (this->data_f) delete[] this->data_f;

	makeCurrent();
	
    this->orbit_VAO.destroy();
    this->orbit_VBO.destroy();
	if(this->orbit_shader_program) delete this->orbit_shader_program;

	this->xyz_gizmo_VAO.destroy();
	this->xyz_gizmo_VBO.destroy();
	this->xyz_gizmo_EBO.destroy();
	if(this->xyz_gizmo_shader_program) delete this->xyz_gizmo_shader_program;
}

void OutputWindow::set_data(orbsim::SimData data) {

	if (this->data_f) delete[] this->data_f;
	this->points = 3 * data.steps;
	this->data_f = new float[this->points];

    for (int i = 0; i < data.steps; i++) {
		// y and z are swapped because OpenGL has the z axis pointing up
		this->data_f[3*i + 0] = 0.6 * data.pos_arr[i].x / 10000;		// temporary
		this->data_f[3*i + 1] = 0.6 * data.pos_arr[i].z / 10000;		// can't use norm tho
		this->data_f[3*i + 2] = 0.6 * data.pos_arr[i].y / 10000;
		// std::cout << data_f[3*i + 0] << " " << data_f[3*i + 1] << " " << data_f[3*i + 2] << "\n";
    }

	this->orbit_VBO.bind();
	glBufferData(GL_ARRAY_BUFFER, this->points * sizeof(float), this->data_f, GL_STATIC_DRAW);
	this->orbit_VBO.release();

	update();
}

void OutputWindow::initializeGL() {

    initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);

	float xyz_gizmo_vertices[] = {
		// vertices		// colors
		0, 0, 0,		1, 1, 1,
		0.1, 0, 0,		1.0, 0, 0,
		0, 0.1, 0,		0, 1.0, 0,
		0, 0, 0.1,		0, 0, 1.0
	};

	unsigned int xyz_gizmo_indices[] {
		0, 1,
		0, 2,
		0, 3
	};

	// Orbit

    this->orbit_VAO.create();
    this->orbit_VAO.bind();

    this->orbit_VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->orbit_VBO.create();
    this->orbit_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->orbit_VBO.bind();
    this->orbit_VBO.allocate(this->data_f, this->points * sizeof(float));

	// this->orbit_VAO.release();
    // this->orbit_VAO.release();

	this->orbit_shader_program = new QOpenGLShaderProgram();
	this->orbit_shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, orbit_vert_src);
	this->orbit_shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, orbit_frag_src);
	this->orbit_shader_program->link();

    this->orbit_shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    this->orbit_shader_program->enableAttributeArray(0);

	// XYZ Gizmo

    this->xyz_gizmo_VAO.create();
    this->xyz_gizmo_VAO.bind();

    this->xyz_gizmo_VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->xyz_gizmo_VBO.create();
    this->xyz_gizmo_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->xyz_gizmo_VBO.bind();
    this->xyz_gizmo_VBO.allocate(xyz_gizmo_vertices, sizeof(xyz_gizmo_vertices));

    this->xyz_gizmo_EBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this->xyz_gizmo_EBO.create();
    this->xyz_gizmo_EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->xyz_gizmo_EBO.bind();
    this->xyz_gizmo_EBO.allocate(xyz_gizmo_indices, sizeof(xyz_gizmo_indices));

	// this->xyz_gizmo_VAO.release();
    // this->xyz_gizmo_VAO.release();

	this->xyz_gizmo_shader_program = new QOpenGLShaderProgram();
	this->xyz_gizmo_shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, xyz_gizmo_vert_src);
	this->xyz_gizmo_shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, xyz_gizmo_frag_src);
	this->xyz_gizmo_shader_program->link();

    this->xyz_gizmo_shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    this->xyz_gizmo_shader_program->enableAttributeArray(0);

    this->xyz_gizmo_shader_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    this->xyz_gizmo_shader_program->enableAttributeArray(1);
}

void OutputWindow::paintGL() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QMatrix4x4 model;

	QMatrix4x4 view;
	float r = 3;
	using namespace std::chrono;
	double t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	view.lookAt(QVector3D(std::sin(t/1000) * r, 1, std::cos(t/1000) * r), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

	QMatrix4x4 projection;
	projection.perspective(45, 4/3, 0.1, 100);

    this->xyz_gizmo_shader_program->bind();
	glUniformMatrix4fv(glGetUniformLocation(this->xyz_gizmo_shader_program->programId(), "model"), 1, GL_FALSE, model.data());
	glUniformMatrix4fv(glGetUniformLocation(this->xyz_gizmo_shader_program->programId(), "view"), 1, GL_FALSE, view.data());
	glUniformMatrix4fv(glGetUniformLocation(this->xyz_gizmo_shader_program->programId(), "projection"), 1, GL_FALSE, projection.data());
	this->xyz_gizmo_VAO.bind();
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, nullptr);
	this->xyz_gizmo_VAO.release();
	this->xyz_gizmo_shader_program->release();


    this->orbit_shader_program->bind();
	glUniform3f(glGetUniformLocation(this->orbit_shader_program->programId(), "color"), 1, 1, 1);
	glUniformMatrix4fv(glGetUniformLocation(this->orbit_shader_program->programId(), "model"), 1, GL_FALSE, model.data());
	glUniformMatrix4fv(glGetUniformLocation(this->orbit_shader_program->programId(), "view"), 1, GL_FALSE, view.data());
	glUniformMatrix4fv(glGetUniformLocation(this->orbit_shader_program->programId(), "projection"), 1, GL_FALSE, projection.data());
	// this->shader_program->setUniformValue(0, QColor(0,1,0));
    this->orbit_VAO.bind();
    glDrawArrays(GL_POINTS, 0, this->points);
	this->orbit_VAO.release();
	this->orbit_shader_program->release();

	update();
}

void OutputWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
}
