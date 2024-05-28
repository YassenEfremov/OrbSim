#ifndef XYZ_GIZMO_HPP
#define XYZ_GIZMO_HPP

#include "vis_obj.hpp"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class XYZGizmo : public VisObj {

public:	
	XYZGizmo();
	~XYZGizmo() override;

	void create() override;
	void render() override;

private:
	float vertices[4 * 3 * 2];		// 4 vertices * 3 floats * 2 attributes
	unsigned int indices[3 * 2];	// 3 lines * 2 vertices

	QOpenGLVertexArrayObject VAO;
	QOpenGLBuffer VBO;
	QOpenGLBuffer EBO;
	QOpenGLShaderProgram *shader_program;
};


#endif	// XYZ_GIZMO_HPP
