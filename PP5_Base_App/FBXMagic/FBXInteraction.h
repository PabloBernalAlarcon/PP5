#pragma once
#ifdef FBXMAGIC_EXPORTS  
#define FBXINTERACTION_API __declspec(dllexport)   
#else  
#define FBXINTERACTION_API __declspec(dllimport)   
#endif

//#include <fbxsdk.h>
#include <vector>
#include <string>
//namespace fbxsdk
//{
	//class FBXManager;
//}

namespace FBXinteracts
{
	using namespace std;

	struct vert {
		float Position[4];
		float Color[4];
		int BoneId[4];
		float Weights[4];
	};
	struct KeyFrame {

		double time;
		float Matrix[16];
	};

	struct AnimClip {

		double lenght;
		std::vector<KeyFrame> keys;
	};
	struct Bone {
		float verts[8];
		std::string name;
		int ID;
		float Matrix[16];
		Bone * parent = nullptr;
		std::vector<Bone*> children;
		
		vector<AnimClip> animations;
	};


	struct Model{
		
		vector<vert> vertices;
		vector<uint32_t> indices;
		Bone * root;
		vector<Bone *> bones;

		Bone * getBone(string name) {
			for each(Bone * b in bones) {
				if (name.compare(b->name) == 0)
					return b;
			}
				 return nullptr;	
		}
	};
	AnimClip animation;
	//using namespace std;
	// This class is exported from the MathLibrary.dll  
	class Functions
	{
	private:
		
	public:
		
		// Returns a + b  
		 FBXINTERACTION_API int  GimmeSomething();

		 FBXINTERACTION_API void SetupFBX(const char* lFilename);

		 FBXINTERACTION_API float getverts(unsigned int indicesVec, unsigned int indicesArr);

		 FBXINTERACTION_API unsigned int getvertsSize();

		 FBXINTERACTION_API unsigned int getPositionsSize();

		FBXINTERACTION_API std::vector<uint32_t> getIndices();

		FBXINTERACTION_API void LoadModel(Model * model);

		 FBXINTERACTION_API float getPositions(unsigned int indicesVec);

		 FBXINTERACTION_API AnimClip getAnimation() { return animation; }

	};
}

