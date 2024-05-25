#ifndef OUTPUT_WINDOW_HPP
#define OUTPUT_WINDOW_HPP

#include "xyz_gizmo.hpp"
#include "orbit.hpp"
#include "simulation/satellite.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

#include <cstddef>


class OutputWindow : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	explicit OutputWindow(QWidget *parent = nullptr);
	~OutputWindow();

	void update_sim_data(orbsim::SimData new_data);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    XYZGizmo xyz_gizmo;
    Orbit orbit;
};

#endif	// OUTPUT_WINDOW_HPP
