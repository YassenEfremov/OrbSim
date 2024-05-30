#ifndef CENTRAL_BODY_HPP
#define CENTRAL_BODY_HPP

#include "vis_obj.hpp"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <vector>


class CentralBody : public VisObj {

public:	
	CentralBody();
	~CentralBody() override;

	void create() override;
	void render() override;

private:
	void create_sphere(double radius, int sectorCount, int stackCount);

	std::vector<float> vertices;
	std::vector<int> indices;

	QOpenGLVertexArrayObject VAO;
	QOpenGLBuffer VBO;
	QOpenGLBuffer EBO;
	QOpenGLShaderProgram *shader_program;
};


#endif	// CENTRAL_BODY_HPP
