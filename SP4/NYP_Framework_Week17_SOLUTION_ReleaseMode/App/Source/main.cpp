/**
 main.cpp
 @brief This file contains the main function for this project
 */
#include "Application.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

/**
 Boolean macro to determine if the program is in debug mode
	0=Non-debug mode, 1=Debug mode
 Note: You can use the debug macro inside Visual Studio project properties.
 */
#define _DEBUGMODE 1

/**
Boolean macro to determine if the program is in 3D or 2D mode
0=2D mode, 1=3D mode
*/
#define _3DMODE 1

/**
 @brief This function is the main function which is called by the operating system when you run the executables
 @return This function returns the error codes
 */
int main(void)
{
	Application* pApp = Application::GetInstance();
	// if the application is initialised properly, then run it
	if (pApp->Init() == true)
	{
		// Run the Application instance
		pApp->Run();

		// Destroy the Application instance, just to be sure
		pApp->Destroy();

		// Set the Application handler to NULL
		pApp = NULL;

		// Return 0 since the application successfully ran
		return 0;
	}

	// Return 1 if the application failed to run
	return 1;
}
