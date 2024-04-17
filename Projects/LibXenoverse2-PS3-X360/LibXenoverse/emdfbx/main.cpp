#include "LibXenoverse.h"





bool exportFbxCameraAnimation(std::vector<string> &arguments, bool exportAscii = false)
{
	string filename = "";
	string extension = "";
	string basefilename = "";

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		filename = arguments.at(i);
		extension = LibXenoverse::extensionFromFilename(filename, true);
		basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		
		if (extension == "ean")
		{
			LibXenoverse::EAN ean;
			if((ean.load(filename))&&(ean.getType()==LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA))
			{
				arguments.erase(arguments.begin() + i);
				break;
			}
		}
		filename = "";
	}

	if (!filename.size())
		return false;


	printf("------- Camera Animation detected\n");


	
	FbxManager *sdk_manager = FbxManager::Create();							// Create FBX Manager
	FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	sdk_manager->SetIOSettings(ios);
	FbxScene *scene = FbxScene::Create(sdk_manager, "EMDFBXScene_camAnimation");			// Create Scene

	scene->GetGlobalSettings().SetCustomFrameRate(60.0);				//specify the frameRate, the number of image by second. (not working well in blender fbx importer. so, I had to modify default value into blender fbx importer).
	
	FbxTime::EProtocol protocol = FbxTime::GetGlobalTimeProtocol();
	FbxTime::SetGlobalTimeProtocol(FbxTime::EProtocol::eDefaultProtocol);
	protocol = FbxTime::GetGlobalTimeProtocol();

	scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eOpenGL);	//eOpenGL,			!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
	scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::m);

	FbxNode *lRootNode = scene->GetRootNode();

	
	LibXenoverse::EAN* animation = new LibXenoverse::EAN();
	if (!animation->load(filename))
	{
		printf("Error on loading %s.\n", filename.c_str());
		LibXenoverse::notifyError();
		return false;
	}


	vector<LibXenoverse::ESK::FbxBonesInstance_DBxv> global_fbx_bones;
	if (animation->getSkeleton())
		animation->getSkeleton()->exportFBXBone(scene, global_fbx_bones);

	FbxCamera* camera = FbxCamera::Create(scene, "Camera");
	camera->ProjectionType = FbxCamera::EProjectionType::ePerspective;
	camera->SetFormat(FbxCamera::eHD);
	camera->FieldOfView = 40.0;
	camera->SetNearPlane(0.1);							//to avoid to not see the character (or somethimng else)

	if (global_fbx_bones.size())
		global_fbx_bones.at(0).mNode->SetNodeAttribute(camera);



	printf("Build FBX Animation\n");
	vector<FbxAnimCurveNode *> global_fbx_animation;
	if (animation)
	{
		LibXenoverse::EANAnimation* eanAnim;

		FbxAnimStack* lAnimStack;
		FbxAnimLayer* lAnimLayer;
		std::vector<FbxAnimStack *> list_AnimStack;


		size_t nbAnims = animation->getAnimations().size();
		for (size_t j = 0; j < nbAnims; j++)													//we create only one stack and one layer by animation. each will animate all bones of all skeleton.
		{
			eanAnim = &(animation->getAnimations().at(j));

			lAnimStack = FbxAnimStack::Create(scene, eanAnim->getName().c_str());
			lAnimLayer = FbxAnimLayer::Create(scene, (eanAnim->getName() + "_Layer0").c_str());
			lAnimStack->AddMember(lAnimLayer);

			list_AnimStack.push_back(lAnimStack);
		}


		vector<FbxAnimCurveNode *> fbx_anim;
		size_t nbBones = global_fbx_bones.size();
		for (size_t j = 0; j<nbBones; j++)
		{
			fbx_anim = animation->exportFBXAnimations(scene, list_AnimStack, &global_fbx_bones.at(j));

			for (size_t j = 0; j < fbx_anim.size(); j++)
				global_fbx_animation.push_back(fbx_anim[j]);
		}
	}


	printf("Export (Camera Animation) FBX\n");
	{
		int lFileFormat = sdk_manager->GetIOPluginRegistry()->GetNativeWriterFormat();
		FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
		bool lExportStatus = lExporter->Initialize((filename + ".fbx").c_str(), lFileFormat, sdk_manager->GetIOSettings());
		if (!lExportStatus)
		{
			printf("Call to FbxExporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
			LibXenoverse::notifyError();
			return false;
		}
		lExporter->Export(scene);												// Export scene
		lExporter->Destroy();



		if(exportAscii)												//test to have the Ascci version, that could help.
		{
			int lFileFormat_Ascii = sdk_manager->GetIOPluginRegistry()->FindWriterIDByDescription("FBX ascii (*.fbx)");
			
			FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
			bool lExportStatus = lExporter->Initialize((filename + "_Ascii.fbx").c_str(), lFileFormat_Ascii, sdk_manager->GetIOSettings());
			if (lExportStatus)
			{
				lExporter->Export(scene);												// Export scene
				lExporter->Destroy();
			}
		}
	}



	printf("------- \n");
	return true;
}




int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for convert the files of Dbxv2 into fbx for edition into Blender or 3dsmax.\n\
 Usage: 'emdfbx.exe [options] file.ext file2.ext ... [output.fbx]'\n\
 Files could be mesh (.emd), skeleton (.esk) or animation (.ean).\n\
 IMPORTANT: from this version, xxx.cam.ean will be converted as a xxxxx.cam.ean.fbx, independently of others files.\n\
 Options : '-NoTexture', '-ExportAscii', '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'.\n\
 'NoTexture' is for having lighter fbx and fast to build.\n\
 'ExportAscii' is for also export a ascii verion of the fbx, it's for debug informations.\n\
 Notice: by doing a shortcut, you could use another option and keep drag and drop of files.\n\
 Notice: \"path With Spaces\" allowed now. \n\
*******************************************************************\n");

	std::vector<string> arguments = LibXenoverse::initApplication(argc, argv);
	


	if (arguments.size()==0)
	{
		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}

	//options
	bool wantNoTexture = false;
	bool exportAscii = false;															//help to debug
	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		if (arguments.at(i) == "-NoTexture")
		{
			arguments.erase(arguments.begin() + i);
			nbArg--;
			i--;
			wantNoTexture = true;
			continue;
		}

		if (arguments.at(i) == "-ExportAscii")
		{
			arguments.erase(arguments.begin() + i);
			nbArg--;
			i--;
			exportAscii = true;
			continue;
		}
	}


	bool cameraAnimationExported = exportFbxCameraAnimation(arguments, exportAscii);										//test for special case Camera Animation (create a special case);
	
	if (arguments.size() == 0)
	{
		if (cameraAnimationExported)
		{
			LibXenoverse::waitOnEnd();
			return 0;
		}

		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}



	printf("Notice: There is all bones declaration in Ean file. In an ESK file, there is less bones than in EAN files.\n");
	printf("Converter Emd to FBX Started. please wait ...\n");

	vector<string> model_filenames;
	vector<string> skeleton_filenames;
	vector<string> animation_filenames;
	string export_filename = "";

	nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		string parameter = arguments.at(i);
		string extension = LibXenoverse::extensionFromFilename(parameter, true);

		if (extension == "emd")
			model_filenames.push_back(parameter);

		if (extension == "esk")
			skeleton_filenames.push_back(parameter);

		if (extension == "nsk")
			skeleton_filenames.push_back(parameter);

		if (extension == "ean")
			animation_filenames.push_back(parameter);

		if (extension == "fbx")
			export_filename = parameter;
	}

	if (!export_filename.size())
	{
		if (model_filenames.size())
			export_filename = model_filenames[0] + ".fbx";
		else if (skeleton_filenames.size())
			export_filename = skeleton_filenames[0] + ".fbx";
		else
			export_filename = "Out.fbx";
	}







	// Create FBX Manager
	FbxManager *sdk_manager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	//ios->SetBoolProp(EXP_ASCIIFBX, true);										//test to have debug on fbx. Todo comment
	sdk_manager->SetIOSettings(ios);




	// Create Scene
	FbxScene *scene = FbxScene::Create(sdk_manager, "EMDFBXScene");

	

	
	scene->GetGlobalSettings().SetCustomFrameRate(60.0);				//specify the frameRate, the number of image by second. (not working well in blender fbx importer. so, I had to modify default value into blender fbx importer).
	//FbxTime::SetGlobalTimeMode(FbxTime::EMode::eFrames60);
	//FbxTime::EMode emode = FbxTime::GetGlobalTimeMode();

	
	FbxTime::EProtocol protocol = FbxTime::GetGlobalTimeProtocol();
	FbxTime::SetGlobalTimeProtocol(FbxTime::EProtocol::eDefaultProtocol);
	protocol = FbxTime::GetGlobalTimeProtocol();


	scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eOpenGL);	//eOpenGL,			!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
	scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::m);


	FbxNode *lRootNode = scene->GetRootNode();



	// Load Shaders and convert it to fx file (will be use by fbx => break in some case, depend of importer).
	vector<string> shader_names;
	shader_names.push_back("adam_shader/shader_age_vs.emb");								//must specified vs folloxed by ps shaders
	shader_names.push_back("adam_shader/shader_age_ps.emb");
	shader_names.push_back("adam_shader/shader_default_vs.emb");
	shader_names.push_back("adam_shader/shader_default_ps.emb");

	/*
	bool needs_install_shaders = false;
	for (size_t i = 0; i < shader_names.size(); i++)
	{
		if (!LibXenoverse::fileCheck(shader_names[i]))
		{
			needs_install_shaders = true;
			break;
		}
	}


	//TODO use it again (for now I desactive Material and shader export for testing fbx import on blender)
	if (needs_install_shaders) {
		printf("Shaders not found. Please use Xenoviewer to prepare shaders in bin folder.\n");
		notifyError();
		return -1;
	}

	for (size_t i = 0; i+1 < shader_names.size(); i+=2) {

		LibXenoverse::EMB *shader_pack_vs = new LibXenoverse::EMB();
		LibXenoverse::EMB *shader_pack_ps = new LibXenoverse::EMB();

		if (!shader_pack_vs->load(shader_names[i])) {
			delete shader_pack_vs;
			printf("Couldn't load Shader Pack %s. File is either missing, open by another application, or corrupt.\n", shader_names[i].c_str());
			notifyError();
			continue;
		}
		if (!shader_pack_ps->load(shader_names[i+1])) {
			delete shader_pack_vs;
			delete shader_pack_ps;
			printf("Couldn't load Shader Pack %s. File is either missing, open by another application, or corrupt.\n", shader_names[i].c_str());
			notifyError();
			continue;
		}

		shader_pack_vs->exportShadersToFx(shader_pack_vs, shader_pack_ps);					//convert all shaders in fx file with defaults program parameters (like Ogre's version).
	}
	*/





	//first things is to open all Ean Files to get full skeleton declaration (miss some of bones in ESK file).
	//Esk file will update matrix transformations
	//we will add custom data on fbx bones or file to notice all bones in Skeleton File (to make esk file in inverse conversion)
	//last notice : the root bone have different name in ESK and EAN file.






	printf("Load EAN files and merge bones declaration\n");

	LibXenoverse::EAN *allAnimations = NULL;		//will have the principal skeleton , result of merge of ean and esk files.
	for (size_t i = 0; i < animation_filenames.size(); i++)
	{
		string filename = animation_filenames.at(i);
		string ean_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);

		LibXenoverse::EAN *animation = new LibXenoverse::EAN();
		if (animation->load(filename))
		{
			//animation->save(filename + ".resave.ean");		//test

			if (allAnimations)
			{
				allAnimations->merge(animation);
				delete animation;
				animation = NULL;
			} else {
				allAnimations = animation;
			}
		} else {
			delete animation;
			animation = NULL;
		}
	}



	if (allAnimations)
		allAnimations->addTPoseAnimation();


	printf("Load ESK files and merge bones declaration\n");
	LibXenoverse::ESK *esk_Principalskeleton;
	if ((allAnimations) && (allAnimations->getSkeleton()))
		esk_Principalskeleton = allAnimations->getSkeleton();
	else
		esk_Principalskeleton = new LibXenoverse::ESK();

	size_t inc = 0;
	for (size_t i = 0; i < skeleton_filenames.size(); i++)
	{
		LibXenoverse::ESK *esk_skeleton = new LibXenoverse::ESK();
		if (esk_skeleton->load(skeleton_filenames[i]))
		{
			//esk_skeleton->save(skeleton_filenames[i] + ".resave.esk");		//Test

			esk_Principalskeleton->merge(esk_skeleton);			//will add new bones (normally allready in ean file), or update transformation matrix (witch isn't in ean file)
		}
		delete esk_skeleton;
	}

	if (esk_Principalskeleton->getBones().size() == 0)
	{
		delete esk_Principalskeleton;
		esk_Principalskeleton = NULL;
	}



	printf("Build FBX skeleton\n");
	vector<LibXenoverse::ESK::FbxBonesInstance_DBxv> global_fbx_bones;
	if(esk_Principalskeleton)
		esk_Principalskeleton->exportFBXBone(scene, global_fbx_bones);

	//if(esk_Principalskeleton)	esk_Principalskeleton->save("esk_principalSkeleton.resave.esk");		//Test
	//if (allAnimations) allAnimations->save("allAnimations.resave.ean");	//test









	printf("Build FBX Animation\n");
	vector<FbxAnimCurveNode *> global_fbx_animation;
	if (allAnimations)
	{
		LibXenoverse::EANAnimation* eanAnim;

		FbxAnimStack* lAnimStack;
		FbxAnimLayer* lAnimLayer;
		std::vector<FbxAnimStack *> list_AnimStack;


		size_t nbAnims = allAnimations->getAnimations().size();
		for (size_t j = 0; j < nbAnims; j++)													//we create only one stack and one layer by animation. each will animate all bones of all skeleton.
		{
			eanAnim = &(allAnimations->getAnimations().at(j));

			lAnimStack = FbxAnimStack::Create(scene, eanAnim->getName().c_str());
			lAnimLayer = FbxAnimLayer::Create(scene, (eanAnim->getName() + "_Layer0").c_str());
			lAnimStack->AddMember(lAnimLayer);

			list_AnimStack.push_back(lAnimStack);
		}


		vector<FbxAnimCurveNode *> fbx_anim;
		size_t nbBones = global_fbx_bones.size();
		for (size_t j=0;j<nbBones;j++)
		{			
			fbx_anim = allAnimations->exportFBXAnimations(scene, list_AnimStack, &global_fbx_bones.at(j) );

			for (size_t j = 0; j < fbx_anim.size(); j++)
				global_fbx_animation.push_back(fbx_anim[j]);
		}
	}




	printf("Build FBX geometry\n");
	string path, node_name, baseFileName;
	LibXenoverse::EMD* emd_model;
	LibXenoverse::EMM* emmMaterial;
	std::vector<LibXenoverse::EMB*> mListEmb;
	LibXenoverse::EMB* emb;
	

	for (size_t i = 0; i < model_filenames.size(); i++)
	{
		node_name = LibXenoverse::nameFromFilenameNoExtension(model_filenames.at(i));
		path = model_filenames.at(i).substr(0, model_filenames.at(i).size() - LibXenoverse::nameFromFilename(model_filenames.at(i)).size());
		baseFileName = path + node_name;

		emd_model = new LibXenoverse::EMD();
		emd_model->load(model_filenames.at(i));

		//emd_model->save(model_filenames.at(i) + ".resave.emd");		//Test



		// Materials/TexturePack parts
		emmMaterial = NULL;
		if (!LibXenoverse::fileCheck(baseFileName + ".emm"))
		{
			printf("No EMM Pack with the name %s found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application !\n", (baseFileName + ".emm").c_str());
			LibXenoverse::notifyWarning();
		}else {
			emmMaterial = new LibXenoverse::EMM();
			if (!emmMaterial->load(baseFileName + ".emm"))
			{
				printf("Invalid EMM Material Pack. Is  %s valid ? \n", (baseFileName + ".emm").c_str());
				LibXenoverse::notifyWarning();
				delete emmMaterial;
				emmMaterial = NULL;
			}
		}

		mListEmb.clear();
		emb = NULL;
		if (!wantNoTexture)
		{
			if (!LibXenoverse::fileCheck(baseFileName + ".emb"))
			{
				printf("No EMB Pack with the name %s found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application !\n", (baseFileName + ".emb").c_str());
				LibXenoverse::notifyWarning();
			}else {
				emb = new LibXenoverse::EMB();
				if (!emb->load(baseFileName + ".emb"))
				{
					printf("Invalid EMB Texture Pack. Is  %s valid ? \n", (baseFileName + ".emb").c_str());
					LibXenoverse::notifyWarning();
					delete emb;
					emb = NULL;
				}else {
					CreateDirectory(emb->getName().c_str(), NULL);
					emb->extract(emb->getName() + "/");					//extract textures (to create fbx textures after)
				}

				if (emb)
					mListEmb.push_back(emb);
			}


			if (!LibXenoverse::fileCheck(baseFileName + ".dyt.emb"))
			{
				printf("Warning : No EMB DYT Pack with the name %s found. we will try to use %s instead \n", (baseFileName + ".dyt.emb").c_str(), (baseFileName + ".emb").c_str());
				LibXenoverse::notifyWarning();
				if (emb)
					mListEmb.push_back(emb->clone());
			}else {
				emb = new LibXenoverse::EMB();
				if (!emb->load(baseFileName + ".dyt.emb"))
				{
					printf("Invalid EMB DYT Texture Pack. Is  %s valid ? \n", (baseFileName + ".dyt.emb").c_str());
					LibXenoverse::notifyWarning();
					delete emb;
					emb = NULL;
				}else {
					CreateDirectory(emb->getName().c_str(), NULL);
					emb->extract(emb->getName() + "/");			//extract textures (to create fbx textures after)
				}

				if (emb)
					mListEmb.push_back(emb);
			}
		}




		

		// Fbx part
		emd_model->exportFBX(scene, global_fbx_bones, mListEmb, emmMaterial, wantNoTexture);



		//	clean
		size_t nbEmb = mListEmb.size();
		for (size_t j = 0; j < nbEmb; j++)
			delete mListEmb.at(j);
		mListEmb.clear();

		if (emmMaterial)
			delete emmMaterial;
		emmMaterial = NULL;
		
		delete emd_model;
		emd_model = NULL;
	}




	printf("Export FBX\n");
	{
		int lFileFormat = sdk_manager->GetIOPluginRegistry()->GetNativeWriterFormat();
		FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
		bool lExportStatus = lExporter->Initialize(export_filename.c_str(), lFileFormat, sdk_manager->GetIOSettings());
		if (!lExportStatus)
		{
			printf("Call to FbxExporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
			return 1;
		}
		lExporter->Export(scene);												// Export scene
		lExporter->Destroy();


		if (exportAscii)												//test to have the Ascci version, that could help.
		{
			int lFileFormat_Ascii = sdk_manager->GetIOPluginRegistry()->FindWriterIDByDescription("FBX ascii (*.fbx)");

			FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
			bool lExportStatus = lExporter->Initialize((export_filename.substr(0, export_filename.length()-4) + "_Ascii.fbx").c_str(), lFileFormat_Ascii, sdk_manager->GetIOSettings());
			if (lExportStatus)
			{
				lExporter->Export(scene);												// Export scene
				lExporter->Destroy();
			}
		}
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}