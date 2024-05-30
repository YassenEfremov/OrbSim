#include "central_body.hpp"

#include "shaders/central_body_shaders.hpp"
#include "simulation/math_obj.hpp"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

#include <chrono>
#include <vector>
#include <cmath>
#include <iostream>


CentralBody::CentralBody() {
	create_sphere(0.2, 20, 20);
}

CentralBody::~CentralBody() {
	this->VAO.destroy();
	this->VBO.destroy();
	this->EBO.destroy();
	if(this->shader_program) delete this->shader_program;
}

void CentralBody::create() {
    this->VAO.create();
    this->VAO.bind();

    this->VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->VBO.create();
    this->VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->VBO.bind();
    this->VBO.allocate(this->vertices.data(), this->vertices.size() * sizeof(float));

    this->EBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    this->EBO.create();
    this->EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->EBO.bind();
    this->EBO.allocate(this->indices.data(), this->indices.size() * sizeof(int));

	this->shader_program = new QOpenGLShaderProgram();
	this->shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, central_body_vert_src);
	this->shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, central_body_frag_src);
	this->shader_program->link();

    this->shader_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    this->shader_program->enableAttributeArray(0);

    this->shader_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    this->shader_program->enableAttributeArray(1);
}

void CentralBody::render() {
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
	glFuncs->glDrawElements(GL_TRIANGLES, this->vertices.size(), GL_UNSIGNED_INT, nullptr);
	this->VAO.release();
	this->shader_program->release();
}

void CentralBody::create_sphere(double radius, int sectorCount, int stackCount) {

	// Code taken from: http://www.songho.ca/opengl/gl_sphere.html

	// clear memory of prev arrays
	// std::vector<float>().swap(vertices);
	// std::vector<float>().swap(normals);
	// std::vector<float>().swap(texCoords);
	this->vertices.reserve((sectorCount+1)*(stackCount+1) * 3 * 2);
	this->indices.reserve(sectorCount*(stackCount-1) * 3 * 2);

	float x, y, z, xy;                              // vertex position
	// float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	// float s, t;                                     // vertex texCoord

	float sectorStep = 2 * orbsim::PI / sectorCount;
	float stackStep = orbsim::PI / stackCount;
	float sectorAngle, stackAngle;

	for(int i = 0; i <= stackCount; ++i)
	{
		stackAngle = orbsim::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// first and last vertices have same position and normal, but different tex coords
		for(int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(z);
			vertices.push_back(y);
			vertices.push_back(0.3);
			vertices.push_back(0.3);
			vertices.push_back(1);

			// normalized vertex normal (nx, ny, nz)
			// nx = x * lengthInv;
			// ny = y * lengthInv;
			// nz = z * lengthInv;
			// normals.push_back(nx);
			// normals.push_back(ny);
			// normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			// s = (float)j / sectorCount;
			// t = (float)i / stackCount;
			// texCoords.push_back(s);
			// texCoords.push_back(t);
		}
	}

	// generate CCW index list of sphere triangles
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1
	// std::vector<int> indices;
	// std::vector<int> lineIndices;
	int k1, k2;
	for(int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if(i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if(i != (stackCount-1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			// store indices for lines
			// vertical lines for all stacks, k1 => k2
			// lineIndices.push_back(k1);
			// lineIndices.push_back(k2);
			// if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
			// {
			// 	lineIndices.push_back(k1);
			// 	lineIndices.push_back(k1 + 1);
			// }
		}
	}

	// poles color example
	for (int i = 0; i <= sectorCount; i++) {
		this->vertices[3*2*i + 3] = 1;
		this->vertices[3*2*i + 4] = 1;
		this->vertices[3*2*i + 5] = 1;
	}
	
	// ground color example
	for (int i = 0; i <= 4; i++) {
		this->vertices[3*2*(109 + i) + 3] = 0.3;
		this->vertices[3*2*(109 + i) + 4] = 1;
		this->vertices[3*2*(109 + i) + 5] = 0.3;

		this->vertices[3*2*(141 + i) + 3] = 0.3;
		this->vertices[3*2*(141 + i) + 4] = 1;
		this->vertices[3*2*(141 + i) + 5] = 0.3;

		this->vertices[3*2*(180 + i) + 3] = 0.3;
		this->vertices[3*2*(180 + i) + 4] = 1;
		this->vertices[3*2*(180 + i) + 5] = 0.3;

		this->vertices[3*2*(215 + i) + 3] = 0.3;
		this->vertices[3*2*(215 + i) + 4] = 1;
		this->vertices[3*2*(215 + i) + 5] = 0.3;

		this->vertices[3*2*(260 + i) + 3] = 0.3;
		this->vertices[3*2*(260 + i) + 4] = 1;
		this->vertices[3*2*(260 + i) + 5] = 0.3;

		this->vertices[3*2*(315 + i) + 3] = 0.3;
		this->vertices[3*2*(315 + i) + 4] = 1;
		this->vertices[3*2*(315 + i) + 5] = 0.3;
	}
}
