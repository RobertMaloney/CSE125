#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <iostream>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>
#include <gtx\string_cast.hpp>


class Renderable
{
private:
	enum RenderMode {
		VERTICES = 0,
		ELEMENTS
	};
	GLuint		m_vao, m_vbo, m_ebo, m_texId;
	GLint		m_model;
	RenderMode	m_renderMode;
	GLint		m_numElems, m_numVerts;
	GLboolean	m_valid;
	GLenum		m_drawType;
	bool        isSkybox = false;
	int			textureUnit;

protected:
	glm::mat4 m_matrix;

	// This bufferObject is used for element drawing
	// TODO Allow object geometry updates OR prevent them
	// TODO Handle normals
	void bufferObject(GLenum elementType, GLfloat* vertexArray, int vertexLength, GLuint *elementArray, int elementLength){
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		
		m_valid = false;
		if (vertexLength == 0 || elementLength == 0) return;
		m_valid = true;

		m_numElems = elementLength;
		m_numVerts = vertexLength;
		m_drawType = elementType;
		m_renderMode = ELEMENTS;

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexLength * sizeof(*vertexArray), vertexArray, GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementLength * sizeof(*elementArray), elementArray, GL_STATIC_DRAW);

		GLint shaderProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

		m_model = glGetUniformLocation(shaderProgram, "model");

		// Vert in attributes
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

		GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		textureUnit = 0;
	}

	// This bufferObject handles vertices only
	void bufferObject(GLenum elementType, GLfloat* vertexArray, int vertexLength) {
		m_renderMode = VERTICES;
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_valid = false;
		if (vertexLength == 0) return;
		m_valid = true;

		m_numVerts = vertexLength;
		m_drawType = elementType;
		m_texId = 0;

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexLength * sizeof(*vertexArray), vertexArray, GL_STATIC_DRAW);

		GLint shaderProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

		m_model = glGetUniformLocation(shaderProgram, "model");

		// Vert in attributes
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), 0);

		GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));

		GLint normalAttrib = glGetAttribLocation(shaderProgram, "normal");
		glEnableVertexAttribArray(normalAttrib);
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));

		GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));

		glBindVertexArray(0);
	}

	~Renderable() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_ebo);
	}

public:
	void render(glm::mat4* matrix) {
		if (m_valid) {
			glBindVertexArray(m_vao);
			if (m_texId != 0) {
				//std::cout << m_texId << std::endl;
				if (this->isSkybox){
					glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId);
				}
				else{
					glBindTexture(GL_TEXTURE_2D, m_texId);
				}
			}
			glUniformMatrix4fv(m_model, 1, GL_FALSE, glm::value_ptr(*matrix));
			switch (m_renderMode) {
				case ELEMENTS: glDrawElements(m_drawType, m_numElems, GL_UNSIGNED_INT, 0); break;
				case VERTICES: glDrawArrays(m_drawType, 0, m_numVerts / 9); break; // 3 vec3 per triangle
			}
			glBindVertexArray(0);
		}
	}

	void setTextureId(GLuint id) {
		m_texId = id;
	}


	GLuint getTextureId() {
		return m_texId;
	}


	void setIsSkybox(bool v){
		isSkybox = v;
	}
};
#endif