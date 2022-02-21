/******************************************************************************/
/*!
\file	MatrixStack.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Matrix Stack to replace openGL math function
*/
/******************************************************************************/
#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>
/******************************************************************************/
/*!
		Class MS:
\brief	Matrix Stack class
*/
/******************************************************************************/
class MS {
	std::stack<glm::mat4> ms;
public:
	MS();
	~MS();
	const glm::mat4& Top() const;
	void PopMatrix();
	void PushMatrix();
	void Clear();
	void LoadIdentity();
	void LoadMatrix(const glm::mat4&matrix);
	void MultMatrix(const glm::mat4&matrix);
	void Rotate(float degrees, float axisX, float axisY, float axisZ);
	void Scale(float scaleX, float scaleY, float scaleZ);
	void Translate(float translateX, float translateY, float translateZ);
	void Frustum(double left, double right, double	bottom, double top, double near, double far);
	void LookAt(double eyeX, double eyeY, double eyeZ,
				double centerX, double centerY, double centerZ,
				double upX, double upY, double upZ);
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
};

#endif
