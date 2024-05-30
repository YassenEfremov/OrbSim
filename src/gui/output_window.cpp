#include "output_window.hpp"

#include "main_window.hpp"
#include "central_body.hpp"
#include "xyz_gizmo.hpp"
#include "orbit.hpp"
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

}

OutputWindow::~OutputWindow() {

	// makeCurrent();
}

void OutputWindow::update_sim_data(orbsim::SimData new_data) {

	this->orbit.update_points(new_data);

	update();
}

void OutputWindow::initializeGL() {

    initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);

	this->central_body.create();
	this->xyz_gizmo.create();
	this->orbit.create();
}

void OutputWindow::paintGL() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// QMatrix4x4 model;

	// QMatrix4x4 view;
	// float r = 3;
	// using namespace std::chrono;
	// double t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	// view.lookAt(QVector3D(std::sin(t/1000) * r, 1, std::cos(t/1000) * r), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

	// QMatrix4x4 projection;
	// projection.perspective(45, 4/3, 0.1, 100);

	this->central_body.render();
	this->xyz_gizmo.render();
	this->orbit.render();

	update();
}

void OutputWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
}
