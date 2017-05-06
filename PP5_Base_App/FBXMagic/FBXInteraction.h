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



//		static FBXINTERACTION_API void PrintNode(FbxNode* pNode);

		//static FBXINTERACTION_API void PrintTabs();

		//static FBXINTERACTION_API  void PrintAttribute(FbxNodeAttribute* pAttribute);

//		static FBXINTERACTION_API FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	};
}

