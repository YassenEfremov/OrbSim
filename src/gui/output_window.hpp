#ifndef OUTPUT_WINDOW_HPP
#define OUTPUT_WINDOW_HPP

#include "simulation/satellite.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>


class OutputWindow : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	explicit OutputWindow(QWidget *parent = nullptr);
	~OutputWindow();

	void set_data(orbsim::SimData data);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // GLuint m_posAttr;
    // GLuint m_colAttr;
    // GLuint m_matrixUniform;

private:
    QOpenGLVertexArrayObject orbit_VAO;
    QOpenGLVertexArrayObject xyz_gizmo_VAO;
    QOpenGLBuffer orbit_VBO;
    QOpenGLBuffer xyz_gizmo_VBO;
    QOpenGLBuffer xyz_gizmo_EBO;
    QOpenGLShaderProgram *orbit_shader_program;
    QOpenGLShaderProgram *xyz_gizmo_shader_program;

	// orbsim::SimData our_data;
    float *data_f;
    int points;
};

#endif	// OUTPUT_WINDOW_HPP
