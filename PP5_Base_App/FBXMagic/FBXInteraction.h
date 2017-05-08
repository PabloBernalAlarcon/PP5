#pragma once
#ifdef FBXMAGIC_EXPORTS  
#define FBXINTERACTION_API __declspec(dllexport)   
#else  
#define FBXINTERACTION_API __declspec(dllimport)   
#endif

//#include <fbxsdk.h>
#include <vector>

//namespace fbxsdk
//{
	//class FBXManager;
//}

namespace FBXinteracts
{
	struct vert {
		float Position[4];
	};

	
	using namespace std;
	// This class is exported from the MathLibrary.dll  
	class Functions
	{
	private:
		
	public:
		
		// Returns a + b  
		static FBXINTERACTION_API int  GimmeSomething();

		static FBXINTERACTION_API void SetupFBX();

		static FBXINTERACTION_API  vector<float> getPositions();

		static FBXINTERACTION_API float getverts(unsigned int indicesVec, unsigned int indicesArr);

		static FBXINTERACTION_API unsigned int getvertsSize();

	};
}

