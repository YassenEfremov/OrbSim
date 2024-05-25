#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "vis_obj.hpp"
#include "simulation/satellite.hpp"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <cstddef>


class Orbit : public VisObj {

public:	
	Orbit();
	~Orbit() override;

	void create() override;
	void render() override;

	void update_points(orbsim::SimData sim_data);

private:
	float *data;
	std::size_t points;

	QOpenGLVertexArrayObject VAO;
	QOpenGLBuffer VBO;
	QOpenGLShaderProgram *shader_program;
};

#endif	// ORBIT_HPP
