#include "stdafx.h"
#include "FBXInteraction.h"
#include <queue>
using namespace fbxsdk;

namespace FBXinteracts {

	FbxNode * root;
	FbxScene* lScene;
	int Functions::GimmeSomething() 
	{
		 
		return FBXSDK_INT_MAX;
	}

	/* Tab character ("\t") counter */
	int numTabs = 0;
	vector<float> Positions;
	vector<vert> becky;
	vector<int> Indices;
	/**
	* Print the required number of tabs.
	*/
	void /*Functions::*/PrintTabs() {
		for (int i = 0; i < numTabs; i++)
			printf("\t");
	}

	/**
	* Return a string-based representation based on the attribute type.
	*/
	FbxString /*Functions::*/GetAttributeTypeName(FbxNodeAttribute::EType type) {
		switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
		}
	}

	/**
	* Print an attribute.
	*/
	void /*Functions::*/PrintAttribute(FbxNodeAttribute* pAttribute) {
		if (!pAttribute) return;

		FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
		FbxString attrName = pAttribute->GetName();
		PrintTabs();
		// Note: to retrieve the character array of a FbxString, use its Buffer() method.
		printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
	}
	FbxPose * BindPose;
	struct my_fbx_joint { FbxNode* node; int parent_index; };

	void LoadBone(FbxNode * pNode, FbxDouble3 StoredP) {


	/*	for (int i = 0; i < lScene->GetPoseCount(); i++)
		{
			if (lScene->GetPose(i)->IsBindPose())
			{
				BindPose = lScene->GetPose(i);
				break;
			}
		}
		
		for (int i = 0; i < BindPose->GetCount(); i++)
		{
			FbxSkeleton * skelet = BindPose->GetNode(i)->GetSkeleton();
			if (skelet != nullptr)
			{
				if (skelet->IsSkeletonRoot())
				{
					pNode = BindPose->GetNode(i);
					break;
				}
			}
		}

		std::vector<my_fbx_joint> joints;
		std::queue<my_fbx_joint> nodes;
		my_fbx_joint temp;
		temp.parent_index = -1;
		temp.node = pNode;
		nodes.push(temp);

		while (!nodes.empty())
		{
			my_fbx_joint curr = nodes.front();
			nodes.pop();
			joints.push_back(curr);

			for (int i = 0; i < curr.node->GetChildCount(); i++)
			{
				my_fbx_joint temp2;
				temp2.node = curr.node->GetChild(i);
				temp2.parent_index = joints.size() - 1;
				nodes.push(temp2);
			}
		}

		for (int i = 0; i < joints.size(); i++)
		{
			FbxDouble3 translation = joints[i].node->LclTranslation.Get();
			Positions.push_back(translation[0]);
			Positions.push_back(translation[1]);
			Positions.push_back(translation[2]);
			Positions.push_back(1.0f);

		}*/
		for (int i = 0; i < pNode->GetChildCount(); i++)
		{
			//FbxDouble3 translation = pNode->LclTranslation.Get();
			FbxDouble3 translationChild = pNode->GetChild(i)->LclTranslation.Get();
			Positions.push_back(StoredP[0]);
			Positions.push_back(StoredP[1]);
			Positions.push_back(StoredP[2]);
			Positions.push_back(1.0f);

			Positions.push_back(translationChild[0]+ StoredP[0]);
			Positions.push_back(translationChild[1] + StoredP[1]);
			Positions.push_back(translationChild[2] + StoredP[2]);
			Positions.push_back(1.0f);

			FbxDouble3 P;
			P[0] = translationChild[0] + StoredP[0];
			P[1] = translationChild[1] + StoredP[1];
			P[2] = translationChild[2] + StoredP[2];
			LoadBone(pNode->GetChild(i), P);
			//me, child, call func on child
		}
	}



	void LoadNode(FbxNode * pNode) {
		
		FbxMesh * leMesh = pNode->GetMesh();
	
		
		for (int i = 0; i < leMesh->GetPolygonCount(); i++)
		{
		
			Indices.push_back(leMesh->GetPolygonVertex(i, 0));
			Indices.push_back(leMesh->GetPolygonVertex(i, 1));
			Indices.push_back(leMesh->GetPolygonVertex(i, 2));

		}
		
		for (int i = 0; i < Indices.size(); i++)
		{
			FbxVector4 holden = leMesh->GetControlPointAt(Indices[i]);

			vert ben; 
			ben.Position[0] = holden.mData[0];
			ben.Position[1] = holden.mData[1];
			ben.Position[2] = holden.mData[2];
			ben.Position[3] = holden.mData[3];

			becky.push_back(ben);
		}
	}
	/**
	* Print a node, its attributes, and all its children recursively.
	*/
	void /*Functions::*/PrintNode(FbxNode* pNode) {

		
		PrintTabs();
		const char* nodeName = pNode->GetName();
		const char* attribute = pNode->GetNodeAttribute()->GetTypeName();
		const char* data = "skeleton";
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();
		FbxString att = GetAttributeTypeName(pNode->GetNodeAttribute()->GetAttributeType());
		/*if (Positions.size() == 0)
		{*/
		printf("fjkj %s \n", att);
			if (att == "skeleton")
			{
				printf("<node name='%s' node att='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
					nodeName, attribute,
					translation[0], translation[1], translation[2],
					rotation[0], rotation[1], rotation[2],
					scaling[0], scaling[1], scaling[2]
				);
				
					/*FbxDouble3 translationChild = pNode->LclTranslation.Get();
					Positions.push_back(translationChild[0]);
					Positions.push_back(translationChild[1]);
					Positions.push_back(translationChild[2]);
					Positions.push_back(1.0f);*/
				if (root == nullptr)
				{
					root = pNode;
					
					LoadBone(pNode, pNode->LclTranslation.Get());
				}


			}

		//}
		
			if (att == "mesh")
			{
				// Print the contents of the node.
				LoadNode(pNode);
				
					printf("<node name='%s' node att='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
						nodeName, attribute,
						translation[0], translation[1], translation[2],
						rotation[0], rotation[1], rotation[2],
						scaling[0], scaling[1], scaling[2]
					);
			}
		
		numTabs++;

		// Print the node's attributes.
		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
			PrintAttribute(pNode->GetNodeAttributeByIndex(i));
		
		// Recursively print the children.
		for (int j = 0; j < pNode->GetChildCount(); j++)
			PrintNode(pNode->GetChild(j));

		numTabs--;
		PrintTabs();
		printf("</node>\n");
	}
	void Functions::SetupFBX() 
	{
		//// Change the following filename to a suitable filename value.
		const char* lFilename = "Teddy_Idle.fbx";

		//// Initialize the SDK manager. This object handles memory management.
		FbxManager* lSdkManager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			exit(-1);
		}

		//Create a new scene so that it can be populated by the imported file.
		lScene = FbxScene::Create(lSdkManager, "myScene");

		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		// The file is imported, so get rid of the importer.
		lImporter->Destroy();

		// Print the nodes of the scene and their attributes recursively.
		// Note that we are not printing the root node because it should
		// not contain any attributes.
		FbxNode* lRootNode = lScene->GetRootNode();
		if (lRootNode) {
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				PrintNode(lRootNode->GetChild(i));

		}
		// Destroy the SDK manager and all the other objects it was handling.
		lSdkManager->Destroy();
	}

	float Functions::getPositions(unsigned int indicesVec) {
		return Positions[indicesVec];
	}

	unsigned int Functions::getPositionsSize() {
		return Positions.size();
	}

	float Functions::getverts(unsigned int indicesVec, unsigned int indicesArr ) {
		return becky[indicesVec].Position[indicesArr];
	}

	unsigned int Functions::getvertsSize() {
		return becky.size();
	}
}