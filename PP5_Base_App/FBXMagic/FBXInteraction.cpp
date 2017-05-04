#include "stdafx.h"
#include "FBXInteraction.h"
using namespace fbxsdk;

namespace FBXinteracts {

	int Functions::GimmeSomething() 
	{
		 
		return FBXSDK_INT_MAX;
	}

	void Functions::SetupFBX() 
	{
		//// Change the following filename to a suitable filename value.
		const char* lFilename = "file.fbx";

		//// Initialize the SDK manager. This object handles memory management.
		FbxManager* lSdkManager = FbxManager::Create();
	}
}