/**
 Singleton template
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

template <typename T>
class CSingletonTemplate
{
public:
	// Get the Singleton instance
	static T* GetInstance(void)
	{
		// If the instance does not exists...
		if (s_mInstance == nullptr)
		{
			// Create a new instance
			s_mInstance = new T();
		}

		// Return the instance
		return s_mInstance;
	}

	// Destroy this singleton instance
	static void Destroy()
	{
		// If the instance exists...
		if (s_mInstance)
		{
			// Delete the singleton instance
			delete s_mInstance;
			// Set the handler to nullptr
			s_mInstance = nullptr;
		}
	}

protected:
	// Constructor
	CSingletonTemplate(void)
	{
	};
	// Destructor
	virtual ~CSingletonTemplate(void)
	{
	};

private:
	static T* s_mInstance;
};

template <typename T>
T* CSingletonTemplate<T>::s_mInstance = nullptr;
