/**
 CCollisionManager
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

 // Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CCollisionManager
{
	public:
		// Sphere-Sphere Collision Test
		static bool SphereSphereCollision(	const glm::vec3 sphereCentreA, 
											const float sphereRadiusA,
											const glm::vec3 sphereCentreB, 
											const float sphereRadiusB);
		// Box-Box Collision Test
		static bool BoxBoxCollision(	const glm::vec3 aMin, 
										const glm::vec3 aMax,
										const glm::vec3 bMin, 
										const glm::vec3 bMax);
		// Ray-Sphere Collision Test
		static bool RaySphereCollision(	const glm::vec3 sphereCentre, 
										const float sphereRadius,
										const glm::vec3 rayOrigin, 
										const glm::vec3 rayDirection,
										float& t);
		// Ray-Box Collision Test
		static bool RayBoxCollision(	const glm::vec3 boxMin, 
										const glm::vec3 boxMax,
										const glm::vec3 rayOrigin, 
										const glm::vec3 rayDirection,
										float& t);
		// Ray-Box Collision Test : Overloaded
		static bool RayBoxCollision(	const glm::vec3 boxMin, 
										const glm::vec3 boxMax,
										const glm::vec3 rayStart,
										const glm::vec3 rayEnd);
};
