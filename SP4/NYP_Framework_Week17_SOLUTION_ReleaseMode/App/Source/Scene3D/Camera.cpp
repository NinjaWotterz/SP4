/**
 CCamera
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Camera.h"
#include "CameraEffects/CameraEffectsManager.h"
#include <iostream>
using namespace std;


/**
 @brief Default Constructor
 */
CCamera::CCamera(void)
	: vec3Position(glm::vec3(0.0f, 0.0f, 0.0f))
	, vec3Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 0.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Offset(glm::vec3(0.0f, 0.0f, 0.0f))
	, fYaw(YAW)
	, fPitch(PITCH)
	, offSetFPitch(0.f)
	, fMovementSpeed(SPEED)
	, fMouseSensitivity(SENSITIVITY)
	, fZoom(ZOOM)
	, ZoomState(false)
{
	UpdateCameraVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CCamera::CCamera(	const glm::vec3 position,
					const glm::vec3 up,
					const float yaw,
					const float pitch)
	: vec3Position(position)
	, vec3Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 0.0f, 0.0f))
	, vec3WorldUp(up)
	, vec3Offset(glm::vec3(0.0f, 0.0f, 0.0f))
	, fYaw(yaw)
	, fPitch(pitch)
	, offSetFPitch(0.f)
	, fMovementSpeed(SPEED)
	, fMouseSensitivity(SENSITIVITY)
	, fZoom(ZOOM)
{
	UpdateCameraVectors();
}

/**
 @brief Constructor with scalar values
 @param posX A const float variable which contains the x position of the camera
 @param posY A const float variable which contains the y position of the camera
 @param posZ A const float variable which contains the z position of the camera
 @param upX A const float variable which contains the x up direction of the camera
 @param upY A const float variable which contains the y up direction of the camera
 @param upZ A const float variable which contains the z up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CCamera::CCamera(	const float posX, const float posY, const float posZ,
					const float upX, const float upY, const float upZ,
					const float yaw, const float pitch)
	: vec3Position(glm::vec3(posX, posY, posZ))
	, vec3Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 0.0f, 0.0f))
	, vec3WorldUp(glm::vec3(upX, upY, upZ))
	, vec3Offset(glm::vec3(0.0f, 0.0f, 0.0f))
	, fYaw(yaw)
	, fPitch(pitch)
	, offSetFPitch(0.f)
	, fMovementSpeed(SPEED)
	, fMouseSensitivity(SENSITIVITY)
	, fZoom(ZOOM)
{
	UpdateCameraVectors();
}

/**
 @brief Destructor
 */
CCamera::~CCamera(void)
{
}

/**
 @brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
 @return A glm::mat4 variable which contains the view matrix
 */
glm::mat4 CCamera::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
@brief Returns the Perspective Matrix
@return A glm::mat4 variable which contains the Perspective matrix
*/
glm::mat4 CCamera::GetPerspectiveMatrix(void) const
{
	return glm::mat4(1.0f);
}


/**
 @brief Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Camera_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CCamera::ProcessKeyboard(const CAMERAMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime;
	if (direction == CAMERAMOVEMENT::FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == CAMERAMOVEMENT::BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == CAMERAMOVEMENT::LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == CAMERAMOVEMENT::RIGHT)
		vec3Position += vec3Right * velocity;
}

/**
 @brief Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CCamera::ProcessMouseMovement(float xoffset, float yoffset, const bool constrainPitch)
{
	xoffset *= fMouseSensitivity;
	yoffset *= fMouseSensitivity;

	fYaw += xoffset;
	fPitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (fPitch > 89.0f)
			fPitch = 89.0f;
		if (fPitch < -89.0f)
			fPitch = -89.0f;
	}

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

/**
 @brief Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
 @param yoffset A const float variable which contains the y axis of the mouse movement
 */
void CCamera::ProcessMouseScroll(const float yoffset)
{
	if (fZoom >= 1.0f && fZoom <= 45.0f)
		fZoom -= yoffset;
	if (fZoom <= 1.0f)
		fZoom = 1.0f;
	if (fZoom >= 45.0f)
		fZoom = 45.0f;
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CCamera::UpdateCameraVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch + offSetFPitch));
	front.y = sin(glm::radians(fPitch + offSetFPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch + offSetFPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// Update the camera's position with the offsets due to camera shakes
	vec3Position += vec3Offset;
}

/**
 @brief PrintSelf
 */
void CCamera::PrintSelf(void)
{
	cout << "CCamera::PrintSelf()" << endl;

	cout << "Camera Attributes:" << endl;
	cout << "\tvec3Position =" << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << endl;
	cout << "\tvec3Front =" << vec3Front.x << ", " << vec3Front.y << ", " << vec3Front.z << endl;
	cout << "\tvec3Up =" << vec3Up.x << ", " << vec3Up.y << ", " << vec3Up.z << endl;
	cout << "\tvec3Right =" << vec3Right.x << ", " << vec3Right.y << ", " << vec3Right.z << endl;
	cout << "\tvec3WorldUp =" << vec3WorldUp.x << ", " << vec3WorldUp.y << ", " << vec3WorldUp.z << endl;
	cout << "Euler Angles:" << endl;
	cout << "\tfYaw =" << fYaw << endl;
	cout << "\tfPitch =" << fPitch << endl;
	cout << "Camera options:" << endl;
	cout << "\tfMovementSpeed =" << fMovementSpeed << endl;
	cout << "\tfMouseSensitivity =" << fMouseSensitivity << endl;
	cout << "\tfZoom =" << fZoom << endl;
	cout << "End of CCamera::PrintSelf()" << endl << endl;
}

void CCamera::SetZoomState(bool zoomstate)
{
	ZoomState = zoomstate;
	if (zoomstate) {
		fZoom = 20.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
	}
	else {
		fZoom = 45.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
	}
	
}

bool CCamera::GetZoomState()
{
	return ZoomState;
}