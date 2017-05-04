#pragma once
#ifdef FBXMAGIC_EXPORTS  
#define FBXINTERACTION_API __declspec(dllexport)   
#else  
#define FBXINTERACTION_API __declspec(dllimport)   
#endif

//#include <fbxsdk.h>

//namespace fbxsdk
//{
	//class FBXManager;
//}

namespace FBXinteracts
{
	// This class is exported from the MathLibrary.dll  
	class Functions
	{
	private:
	
	public:

		// Returns a + b  
		static FBXINTERACTION_API int  GimmeSomething();

		static FBXINTERACTION_API void SetupFBX();

	};
}

