#include "EMMOgre.h"
#include "EMBOgre.h"
#include "EANOgre.h"

EMMOgre::EMMOgre()
{
	material_resources_created = false;
	texture_pack = NULL;
	texture_dyt_pack = NULL;
	texture_player_dyt_pack = NULL;
	emdPartVisible = true;

	animation_to_change = 0;
	animation_to_change2 = 0;
	ema_Material_Anim = ema_Material_Anim2 = 0;
	animationLoopEnable = true;

	zMaskReadWrite_defaultValue_for_transparent = true;
}


/*
//Dario's version, for only characters on Dbxv1.
Ogre::Material *EMMOgre::createOgreMaterial(EMMMaterial *emm_material)					
{
	string ogre_material_name = name + "_" + emm_material->getName();

	Ogre::Material *compile_material = Ogre::MaterialManager::getSingleton().create(ogre_material_name, XENOVIEWER_RESOURCE_GROUP).getPointer();
	if (!compile_material)
		return NULL;

	Ogre::Pass *pass = compile_material->getTechnique(0)->getPass(0);

	// Force it to use 16 total texture units and full ambient lighting
	for (size_t i = 0; i<16; i++)
		Ogre::TextureUnitState *texture_unit_state = pass->createTextureUnitState("Blank");
	pass->setAmbient(1.0, 1.0, 1.0);

	//pass->setCullingMode(Ogre::CullingMode::CULL_ANTICLOCKWISE);					//is about mesh inversion (symetry)

	pass->getTextureUnitState((size_t)1)->setTextureName(name + "_0");     //   ImageSampler1        s1       1
	pass->getTextureUnitState((size_t)4)->setTextureName(name + ".dyt_0"); //   SamplerToon          s4       1
	pass->getTextureUnitState((size_t)4)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
	pass->getTextureUnitState((size_t)14)->setTextureName(name + ".dyt_1");    //   ImageSamplerTemp14   s14      1

	string shader_name = emm_material->getShaderName();		
	string pixel_shader_name = shader_name + "_PS";
	string vertex_shader_name = shader_name + "_W_VS";

	if (shader_name == "T1_C_L_SM_W")		//TODO use sds (shader program) to know the real shader used
	{
		vertex_shader_name = "T1_C_L_W_VS";
		pixel_shader_name = "T1_C_L_SM_PS";
	}

	bool pixel_shader_exists = Ogre::GpuProgramManager::getSingleton().resourceExists(pixel_shader_name);
	if (pixel_shader_exists)
	{
		pass->setFragmentProgram(pixel_shader_name);

		Ogre::GpuProgramParametersSharedPtr fp_parameters = pass->getFragmentProgramParameters();
		fp_parameters->setTransposeMatrices(false);

		Ogre::Vector4 MatCol0 = Ogre::Vector4::ZERO;
		Ogre::Vector4 MatCol1 = Ogre::Vector4::ZERO;
		Ogre::Vector4 MatCol2 = Ogre::Vector4::ZERO;
		Ogre::Vector4 MatCol3 = Ogre::Vector4::ZERO;
		Ogre::Vector4 MatScale0 = Ogre::Vector4::ZERO;
		Ogre::Vector4 MatScale1 = Ogre::Vector4::ZERO;

		vector<EMMParameter *> &parameters = emm_material->getParameters();
		for (size_t i = 0; i < parameters.size(); i++)
		{
			string parameter_name = parameters[i]->name;
			if (parameter_name == "MatCol0R") MatCol0.x = parameters[i]->valueFloat();
			if (parameter_name == "MatCol0G") MatCol0.y = parameters[i]->valueFloat();
			if (parameter_name == "MatCol0B") MatCol0.z = parameters[i]->valueFloat();
			if (parameter_name == "MatCol0A") MatCol0.w = parameters[i]->valueFloat();

			if (parameter_name == "MatCol1R") MatCol1.x = parameters[i]->valueFloat();
			if (parameter_name == "MatCol1G") MatCol1.y = parameters[i]->valueFloat();
			if (parameter_name == "MatCol1B") MatCol1.z = parameters[i]->valueFloat();
			if (parameter_name == "MatCol1A") MatCol1.w = parameters[i]->valueFloat();

			if (parameter_name == "MatCol2R") MatCol2.x = parameters[i]->valueFloat();
			if (parameter_name == "MatCol2G") MatCol2.y = parameters[i]->valueFloat();
			if (parameter_name == "MatCol2B") MatCol2.z = parameters[i]->valueFloat();
			if (parameter_name == "MatCol2A") MatCol2.w = parameters[i]->valueFloat();

			if (parameter_name == "MatCol3R") MatCol3.x = parameters[i]->valueFloat();
			if (parameter_name == "MatCol3G") MatCol3.y = parameters[i]->valueFloat();
			if (parameter_name == "MatCol3B") MatCol3.z = parameters[i]->valueFloat();
			if (parameter_name == "MatCol3A") MatCol3.w = parameters[i]->valueFloat();

			if (parameter_name == "MatScale0X") MatScale0.x = parameters[i]->valueFloat();
			if (parameter_name == "MatScale0Y") MatScale0.y = parameters[i]->valueFloat();
			if (parameter_name == "MatScale0Z") MatScale0.z = parameters[i]->valueFloat();
			if (parameter_name == "MatScale0W") MatScale0.w = parameters[i]->valueFloat();

			if (parameter_name == "MatScale1X") MatScale1.x = parameters[i]->valueFloat();
			if (parameter_name == "MatScale1Y") MatScale1.y = parameters[i]->valueFloat();
			if (parameter_name == "MatScale1Z") MatScale1.z = parameters[i]->valueFloat();
			if (parameter_name == "MatScale1W") MatScale1.w = parameters[i]->valueFloat();
		}

		// Override Battle Damage
		MatCol3.x = 0.0; // Scratch Mark Multiplier
		MatCol3.y = 0.0; // Blood Mark Multiplier

		fp_parameters->setConstant(51, Ogre::Vector4(0.0));  //   g_vFadeMulti_PS      c51      1
		fp_parameters->setConstant(52, Ogre::Vector4(0.0));  //   g_vFadeRim_PS        c52      1
		fp_parameters->setConstant(53, Ogre::Vector4(0.0));  //   g_vFadeAdd_PS        c53      1
		fp_parameters->setAutoConstant(64, Ogre::GpuProgramParameters::ACT_LIGHT_POSITION, 0);  //   g_vLightVec0_PS      c64      1
		fp_parameters->setConstant(84, MatCol0);
		fp_parameters->setConstant(85, MatCol1);
		fp_parameters->setConstant(86, MatCol2);
		fp_parameters->setConstant(87, MatCol3);
		fp_parameters->setConstant(92, MatScale0);
		fp_parameters->setConstant(93, MatScale1);
		fp_parameters->setConstant(105, Ogre::Vector4(0.0));  //   g_vColor0_PS         c105     1
		fp_parameters->setConstant(106, Ogre::Vector4(0.0));  //   g_vColor1_PS         c106     1
		fp_parameters->setConstant(107, Ogre::Vector4(0.0));  //   g_vColor2_PS         c107     1
		fp_parameters->setConstant(117, Ogre::Vector4(0.0));  //   g_vColor12_PS        c117     1
		fp_parameters->setConstant(139, Ogre::Vector4(1.0));  //   Color Multiplier
		fp_parameters->setConstant(140, Ogre::Vector4(0.0));  //   g_vParam4_PS         c140     1
		fp_parameters->setConstant(141, Ogre::Vector4(0.0));  //   g_vParam5_PS         c141     1
		fp_parameters->setConstant(143, Ogre::Vector4(0.0, 23.2558, 0.04587, 0.0));  // Toon Detail Parameter
	}

	bool vertex_shader_exists = Ogre::GpuProgramManager::getSingleton().resourceExists(vertex_shader_name);
	if (vertex_shader_exists)
	{
		pass->setVertexProgram(vertex_shader_name);
		Ogre::GpuProgramParametersSharedPtr vp_parameters = pass->getVertexProgramParameters();
		vp_parameters->setTransposeMatrices(false);

		vp_parameters->setAutoConstant((size_t)0, Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		vp_parameters->setAutoConstant((size_t)72, Ogre::GpuProgramParameters::ACT_CAMERA_POSITION);
		vp_parameters->setAutoConstant((size_t)104, Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4);
	}

	if (shader_name == "T1_C_L_SM_W")					//TODO lire le SDS
	{
		bool pixel_shader_exists = Ogre::GpuProgramManager::getSingleton().resourceExists(pixel_shader_name);
		if (pixel_shader_exists)
		{
			pass->setFragmentProgram(pixel_shader_name);

			Ogre::GpuProgramParametersSharedPtr fp_parameters = pass->getFragmentProgramParameters();
			fp_parameters->setTransposeMatrices(false);

			Ogre::Vector4 MatCol0 = Ogre::Vector4::ZERO;
			Ogre::Vector4 MatCol1 = Ogre::Vector4::ZERO;
			Ogre::Vector4 MatCol2 = Ogre::Vector4::ZERO;
			Ogre::Vector4 MatCol3 = Ogre::Vector4::ZERO;
			Ogre::Vector4 MatScale0 = Ogre::Vector4::ZERO;
			Ogre::Vector4 MatScale1 = Ogre::Vector4::ZERO;

			vector<EMMParameter *> &parameters = emm_material->getParameters();
			for (size_t i = 0; i < parameters.size(); i++)
			{
				string parameter_name = parameters[i]->name;
				if (parameter_name == "MatCol0R") MatCol0.x = parameters[i]->valueFloat();
				if (parameter_name == "MatCol0G") MatCol0.y = parameters[i]->valueFloat();
				if (parameter_name == "MatCol0B") MatCol0.z = parameters[i]->valueFloat();
				if (parameter_name == "MatCol0A") MatCol0.w = parameters[i]->valueFloat();

				if (parameter_name == "MatCol1R") MatCol1.x = parameters[i]->valueFloat();
				if (parameter_name == "MatCol1G") MatCol1.y = parameters[i]->valueFloat();
				if (parameter_name == "MatCol1B") MatCol1.z = parameters[i]->valueFloat();
				if (parameter_name == "MatCol1A") MatCol1.w = parameters[i]->valueFloat();

				if (parameter_name == "MatCol2R") MatCol2.x = parameters[i]->valueFloat();
				if (parameter_name == "MatCol2G") MatCol2.y = parameters[i]->valueFloat();
				if (parameter_name == "MatCol2B") MatCol2.z = parameters[i]->valueFloat();
				if (parameter_name == "MatCol2A") MatCol2.w = parameters[i]->valueFloat();

				if (parameter_name == "MatCol3R") MatCol3.x = parameters[i]->valueFloat();
				if (parameter_name == "MatCol3G") MatCol3.y = parameters[i]->valueFloat();
				if (parameter_name == "MatCol3B") MatCol3.z = parameters[i]->valueFloat();
				if (parameter_name == "MatCol3A") MatCol3.w = parameters[i]->valueFloat();

				if (parameter_name == "MatScale0X") MatScale0.x = parameters[i]->valueFloat();
				if (parameter_name == "MatScale0Y") MatScale0.y = parameters[i]->valueFloat();
				if (parameter_name == "MatScale0Z") MatScale0.z = parameters[i]->valueFloat();
				if (parameter_name == "MatScale0W") MatScale0.w = parameters[i]->valueFloat();

				if (parameter_name == "MatScale1X") MatScale1.x = parameters[i]->valueFloat();
				if (parameter_name == "MatScale1Y") MatScale1.y = parameters[i]->valueFloat();
				if (parameter_name == "MatScale1Z") MatScale1.z = parameters[i]->valueFloat();
				if (parameter_name == "MatScale1W") MatScale1.w = parameters[i]->valueFloat();
			}

			// Override Battle Damage
			MatCol3.x = 0.0; // Scratch Mark Multiplier
			MatCol3.y = 0.0; // Blood Mark Multiplier

			fp_parameters->setConstant(51, Ogre::Vector4(0.0));  //   g_vFadeMulti_PS      c51      1
			fp_parameters->setConstant(52, Ogre::Vector4(0.0));  //   g_vFadeRim_PS        c52      1
			fp_parameters->setConstant(53, Ogre::Vector4(0.0));  //   g_vFadeAdd_PS        c53      1
			fp_parameters->setAutoConstant(64, Ogre::GpuProgramParameters::ACT_LIGHT_POSITION, 0);  //   g_vLightVec0_PS      c64      1
			fp_parameters->setConstant(84, MatCol0);
			fp_parameters->setConstant(85, MatCol1);
			fp_parameters->setConstant(86, MatCol2);
			fp_parameters->setConstant(87, MatCol3);
			fp_parameters->setConstant(92, MatScale0);
			fp_parameters->setConstant(93, MatScale1);
			fp_parameters->setConstant(105, Ogre::Vector4(0.0));  //   g_vColor0_PS         c105     1
			fp_parameters->setConstant(106, Ogre::Vector4(0.0));  //   g_vColor1_PS         c106     1
			fp_parameters->setConstant(107, Ogre::Vector4(0.0));  //   g_vColor2_PS         c107     1
			fp_parameters->setConstant(117, Ogre::Vector4(0.0));  //   g_vColor12_PS        c117     1
			fp_parameters->setConstant(139, Ogre::Vector4(1.0));  //   Color Multiplier
			fp_parameters->setConstant(140, Ogre::Vector4(0.0));  //   g_vParam4_PS         c140     1
			fp_parameters->setConstant(141, Ogre::Vector4(0.0));  //   g_vParam5_PS         c141     1
			fp_parameters->setConstant(143, Ogre::Vector4(0.0, 23.2558, 0.04587, 0.0));  // Toon Detail Parameter
		}

		bool vertex_shader_exists = Ogre::GpuProgramManager::getSingleton().resourceExists(vertex_shader_name);
		if (vertex_shader_exists)
		{
			pass->setVertexProgram(vertex_shader_name);
			Ogre::GpuProgramParametersSharedPtr vp_parameters = pass->getVertexProgramParameters();
			vp_parameters->setTransposeMatrices(false);

			vp_parameters->setAutoConstant((size_t)0, Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
			vp_parameters->setAutoConstant((size_t)72, Ogre::GpuProgramParameters::ACT_CAMERA_POSITION);
			vp_parameters->setAutoConstant((size_t)104, Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4);
		}
	}

	created_materials.push_back(ogre_material_name);
	return compile_material;
}
*/



Ogre::Material *EMMOgre::createOgreMaterial(EMMMaterial *emm_material, std::vector<SDS*> &sds_list, Ogre::Viewport* viewport)
{
	
	//on a lire les SDS et shaders pour savoir ou se trouve les textureUnit a utilser.
	string shader_name = emm_material->getShaderName();
	string sds_name = "";
	SDSShaderProgram *sdsShaderProgram = nullptr;

	size_t nbSdsfiles = sds_list.size();
	for (size_t i = 0; i < nbSdsfiles; i++)
	{
		std::vector<SDSShaderProgram *> &listShaderProg = sds_list.at(i)->GetshaderPrograms();
		size_t nbShaderProg = listShaderProg.size();
		for (size_t j = 0; j < nbShaderProg; j++)
		{
			if (listShaderProg.at(j)->getName() == shader_name)
			{
				sdsShaderProgram = listShaderProg.at(j);
				sds_name = sds_list.at(i)->getName();
				break;
			}
		}
		if (sdsShaderProgram != nullptr)
			break;
	}
	if (sdsShaderProgram == nullptr)
		return NULL;
	
	
	
	string ogre_material_name = name + "_" + emm_material->getName();
	Ogre::StringUtil::toLowerCase(ogre_material_name);

	Ogre::MaterialPtr compile_material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().create(ogre_material_name, XENOVIEWER_RESOURCE_GROUP);
	if (compile_material.isNull())
		return NULL;

	Ogre::Pass *pass = compile_material->getTechnique(0)->getPass(0);

	// Force it to use 16 total texture units and full ambient lighting
	for (size_t i = 0; i<16; i++)
		Ogre::TextureUnitState *texture_unit_state = pass->createTextureUnitState("Blank");

	


	Ogre::String name_tmp = Ogre::StringUtil::replaceAll(Ogre::StringUtil::replaceAll(sds_name, "technique_", ""), "_sds", "");	//personnalize shaders, to avoid shader with same name in many shader type (age of default)

	string vertex_shader_name = name_tmp +"_"+ sdsShaderProgram->getVertexShaderName();
	string pixel_shader_name = name_tmp +"_"+ sdsShaderProgram->getPixelShaderName();

	Ogre::LogManager::getSingleton().logMessage("MatName: " + name + " emmMaterial: " + emm_material->getName() + " VertexShader: " + vertex_shader_name + " pixel_shader_name:" + pixel_shader_name);
	
	std::vector<EmmMaterialParameter> materialParameters;

	std::vector<size_t> listRegForTextTile;
	bool vertex_shader_exists = Ogre::GpuProgramManager::getSingleton().resourceExists(vertex_shader_name);
	if (vertex_shader_exists)
	{
		pass->setVertexProgram(vertex_shader_name);
		Ogre::GpuProgramParametersSharedPtr vp_parameters = pass->getVertexProgramParameters();
		
		setUpMaterialParameters(vertex_shader_name, vp_parameters, pass, emm_material, materialParameters, "vertex", viewport);
	}


	bool pixel_shader_exists = Ogre::GpuProgramManager::getSingleton().resourceExists(pixel_shader_name);
	if (pixel_shader_exists)
	{
		pass->setFragmentProgram(pixel_shader_name);
		Ogre::GpuProgramParametersSharedPtr fp_parameters = pass->getFragmentProgramParameters();
		
		setUpMaterialParameters(pixel_shader_name, fp_parameters, pass, emm_material, materialParameters, "fragment", viewport);
	}


	

	created_materials.push_back(EmmMaterialCreated(ogre_material_name, emm_material, materialParameters));
	return compile_material.getPointer();
}





std::vector<size_t> EMMOgre::setUpMaterialParameters(string shader_name, Ogre::GpuProgramParametersSharedPtr fp_parameters, Ogre::Pass* pass, EMMMaterial *emm_material, std::vector<EmmMaterialParameter> &materialParameters, string shaderType, Ogre::Viewport* viewport)
{
	std::vector<size_t> listRegForTextTile;

	Ogre::GpuProgramPtr program = Ogre::GpuProgramManager::getSingletonPtr()->getByName(shader_name);


	// I search to have information about sampler register positions, but don't get it from pass->getFragmentProgramParameters() or GpuProgramPtr
	// so I will parse source code to find this information. if you find how to do better, please do it.
	Ogre::String source = program->getSource();
	std::vector<EMMMaterial::Sampler2D_shaderDefinition> &listSDS = emm_material->getListSampler2D();
	size_t nbSdsdef = listSDS.size();

	

	bool isParameters = false;
	bool isRegisters = false;
	string line = "";
	size_t nbParams = 0;
	Ogre::StringVector sv;
	Ogre::StringVector sv2;
	Ogre::String arg = "";

	Ogre::StringVector sampler2DListName;
	Ogre::StringVector float4ListName;
	Ogre::StringVector boolListName;
	Ogre::StringVector float4x3ListName;
	Ogre::StringVector float4x4ListName;

	Ogre::String paramName = "";
	Ogre::String regName = "";
	size_t size = 0;
	size_t reg = 0;



	//config of pass
	EMMParameter* params = emm_material->getParameter("BackFace");
	if ((params) && (params->uint_value == 1))
	{
		pass->setManualCullingMode(Ogre::MANUAL_CULL_NONE);				//culling software
		pass->setCullingMode(Ogre::CULL_NONE);
		//pass->setCullingMode(Ogre::CULL_CLOCKWISE);
	}
		
	params = emm_material->getParameter("TwoSidedRender");
	if ((params) && (params->uint_value == 1))
		pass->setCullingMode(Ogre::CULL_NONE);							//culling hardware
	
	bool is_transparent = false;
	params = emm_material->getParameter("AlphaBlend");
	if ((params) && (params->uint_value == 1))
	{
		pass->setTransparentSortingEnabled(true);
		is_transparent = true;


		params = emm_material->getParameter("AlphaBlendType");			// with the name of material, we could say : 0: nml (normal or multiply ?), 1: additive, 2: sub
		size_t value = 0;
		if (params)
			value = params->uint_value;

		switch (value)
		{
		case 0:										 //nml (normal or multiply ?)
			{
				pass->setSeparateSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);		//no premultiplied
				//pass->setSeparateSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA); //premultipli�	//Todo activate texture premultiplication on loading
			}
			break;

		case 1:
			//pass->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE);
			pass->setSeparateSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE);		//
			break;	//additive
		case 2:
			pass->setSceneBlendingOperation(Ogre::SceneBlendOperation::SBO_REVERSE_SUBTRACT);
			pass->setSeparateSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ZERO, Ogre::SBF_ONE);		//

			break;	//sub

		default:
			{
				int aa = 42;
				//assert(false);
			}
		}
		
	}else {
		pass->setSeparateSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ZERO, Ogre::SBF_ONE, Ogre::SBF_ZERO);			//full opaque by default
	}

	params = emm_material->getParameter("AlphaSortMask");
	if ((params) && (params->uint_value == 1))
		pass->setTransparentSortingForced(true);


	//todo : LowRez, AnimationChannel, MipMapLod0, CustomFlag


	
	params = emm_material->getParameter("ZWriteMask");
	pass->setDepthWriteEnabled((params) ? (params->uint_value == 1) : ((is_transparent) ? zMaskReadWrite_defaultValue_for_transparent : true ) );

	params = emm_material->getParameter("ZTestMask");
	pass->setDepthCheckEnabled( (params) ? (params->uint_value == 1) : ((is_transparent) ? zMaskReadWrite_defaultValue_for_transparent : true ) );






	//read shader source to know what it need.
	Ogre::StringVector lines = Ogre::StringUtil::split(source, "\n");

	size_t nbLines = lines.size();
	for (size_t i = 0; i < nbLines; i++)
	{
		line = lines.at(i);
		if (line.substr(0, 2) != "//")				//only keep commentaires.
			continue;

		if (line == "// Parameters:")
		{
			isParameters = true;
			isRegisters = false;
			continue;
		}

		if (line == "// Registers:")
		{
			isParameters = false;
			isRegisters = true;
			continue;
		}

		if ((line == "") || (line.substr(0, 12) == "// Generated") || (line == "//   Name                 Reg   Size") || (line == "//   -------------------- ----- ----"))
			continue;


		sv = Ogre::StringUtil::split(line, " ");
		nbParams = sv.size();
		sv2.clear();

		for (size_t j = 0; j < nbParams; j++)				//clean arg.
		{
			arg = sv.at(j);
			if ((arg == "") || (arg == "//"))
				continue;

			if (arg.substr(arg.length() - 1) == ";")
				arg = arg.substr(0, arg.length() - 1);

			if (arg.find("["))
				arg = arg.substr(0, arg.find("["));

			sv2.push_back(arg);
		}

		
		if ((isParameters) && (sv2.size() == 2))
		{
			if (sv2.at(1).substr(0, 1) == "$")					//for uniform parameters
				sv2.at(1) = sv2.at(1).substr(1);
			
			if (sv2.at(0) == "sampler2D")
				sampler2DListName.push_back(sv2.at(1));
			else if (sv2.at(0) == "bool")
				boolListName.push_back(sv2.at(1));
			else if (sv2.at(0) == "float4")
				float4ListName.push_back(sv2.at(1));
			else if (sv2.at(0) == "float4x4")
				float4x4ListName.push_back(sv2.at(1));
			else if (sv2.at(0) == "float4x3")
				float4x3ListName.push_back(sv2.at(1));
			//else
				//assert(false);	//if pass here, complete by reading shaders
		}

		if ((isRegisters) && (sv2.size() == 3))
		{
			if ((sv2.at(0) == "Name") || (sv2.at(0).substr(0,5) == "-----"))
				continue;
			
			if (sv2.at(0).substr(0, 1) == "$")					//for uniform parameters
				sv2.at(0) = sv2.at(0).substr(1);

			paramName = sv2.at(0);
			regName = sv2.at(1);
			reg = Ogre::StringConverter::parseUnsignedInt(regName.substr(1));
			size = Ogre::StringConverter::parseUnsignedInt(sv2.at(2));

			


			//try to get the type name 
			bool isDone = false;
			string isUsed = "";
			Ogre::Vector4 defaultValue = Ogre::Vector4::ZERO;					//only for parameter less than 5 floats.

			nbParams = sampler2DListName.size();
			for (size_t j = 0; ((j < nbParams) && (!isDone)); j++)
			{
				if (sampler2DListName.at(j) != paramName)
					continue;


				if ((paramName == "alive_particles"))			//s0
					pass->getTextureUnitState(reg)->setCubicTextureName("cubescene.jpg");

				if ((paramName == "Texture_YTexture") || (paramName == "ImageYTexture")) //s0
					pass->getTextureUnitState(reg)->setTextureName("empty.png");
				if ((paramName == "Texture_UTexture") || (paramName == "ImageUTexture")) //s1
					pass->getTextureUnitState(reg)->setTextureName("empty.png");
				if ((paramName == "Texture_VTexture") || (paramName == "ImageVTexture")) //s2
					pass->getTextureUnitState(reg)->setTextureName("empty.png");
				if ((paramName == "Texture_ATexture") || (paramName == "ImageATexture")) //s3
					pass->getTextureUnitState(reg)->setTextureName("empty.png");


				//SamplerToon is the color swap from dyt texture. it isn't in definition of sampler - embPack, so we have to add it here.
				if ((paramName == "Texture_SamplerToon") || (paramName == "SamplerToon"))				//s4
				{
					pass->getTextureUnitState(reg)->setTextureName(name + ".dyt_0");											//todo add something in UI to change the index of dyt (dyt is about color degrade pallette, witch change with charater's slot).
					pass->getTextureUnitState(reg)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
					pass->getTextureUnitState(reg)->setTextureAddressingMode(Ogre::TextureUnitState::TextureAddressingMode::TAM_CLAMP);
				}


				if ((paramName == "Texture_SamplerCubeMap") ||(paramName == "SamplerCubeMap"))			//s5
					pass->getTextureUnitState(reg)->setCubicTextureName("cubescene.jpg");				//todo tester
				if ((paramName == "Texture_SamplerSphereMap") || (paramName == "ImageSamplerSphereMap")) //s5
					pass->getTextureUnitState(reg)->setTextureName("spheremap.png");					//todo tester
				

				if ((paramName == "Texture_SamplerProjectionMap") || (paramName == "ImageSamplerProjectionMap")) //s6
					pass->getTextureUnitState(reg)->setTextureName("empty.png");					//todo tester
				if ((paramName == "Texture_SamplerShadowMap") || (paramName == "ImageSamplerShadowMap")) //s7
					pass->getTextureUnitState(reg)->setTextureName("empty.png");


				if ((paramName == "Texture_SamplerReflect") || (paramName == "ImageSamplerReflect")) //s8
					pass->getTextureUnitState(reg)->setTextureName("spheremap.png");					//todo change to RTT
				if ((paramName == "Texture_SamplerRefract") || (paramName == "ImageSamplerRefract")) //s9
					pass->getTextureUnitState(reg)->setTextureName("empty.png");					//todo change to RTT
				if ((paramName == "Glare") || (paramName == "Texture_SamplerReflectGlare") || (paramName == "ImageSamplerReflectGlare")) //s9
					pass->getTextureUnitState(reg)->setTextureName("empty.png");


				if ((paramName == "Texture_SamplerAlphaDepth") || (paramName == "ImageSamplerAlphaDepth")) //s10
					pass->getTextureUnitState(reg)->setTextureName("empty.png");					//todo change to RTT

				if ((paramName == "Texture_SamplerCurrentScene") || (paramName == "ImageSamplerCurrentScene")) //s11
				{
					//create RTT
					Ogre::TextureManager* textMgr = Ogre::TextureManager::getSingletonPtr();
					if (textMgr->getByName("RttTex_CurrentScene").isNull())
					{
						Ogre::TexturePtr rttTexture = textMgr->createManual("RttTex_CurrentScene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, viewport->getWidth(), viewport->getHeight(), 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
						Ogre::RenderTexture* renderTexture = rttTexture->getBuffer()->getRenderTarget();
						renderTexture->addViewport(viewport->getCamera());
						renderTexture->getViewport(0)->setClearEveryFrame(true);
						renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
						renderTexture->getViewport(0)->setOverlaysEnabled(false);
					}

					pass->getTextureUnitState(reg)->setTextureName("RttTex_CurrentScene");
				}

				if ((paramName == "Texture_SamplerSmallScene") || (paramName == "ImageSamplerSmallScene")) //s12
				{
					//create RTT
					Ogre::TextureManager* textMgr = Ogre::TextureManager::getSingletonPtr();
					if (textMgr->getByName("RttTex_CurrentScene").isNull())
					{

						Ogre::SceneManager* sceneMgr = viewport->getCamera()->getSceneManager();
						Ogre::Camera* camera = sceneMgr->createCamera("toto");
						
						Ogre::TexturePtr rttTexture = textMgr->createManual("RttTex_CurrentScene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 200, 200, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET, 0, false, 0);
						Ogre::RenderTexture* renderTexture = rttTexture->getBuffer()->getRenderTarget();
						renderTexture->setActive(true);
						renderTexture->addViewport(camera);
						renderTexture->getViewport(0)->setClearEveryFrame(true);
						renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::ZERO);
						renderTexture->getViewport(0)->setOverlaysEnabled(false);
						renderTexture->getViewport(0)->setAutoUpdated(true);
						//renderTexture->update(false);
						//renderTexture->writeContentsToFile("texture_image.png");
					}

					pass->getTextureUnitState(reg)->setTextureName("RttTex_CurrentScene");
					//pass->getTextureUnitState(reg)->setTextureName("spheremap.png");				//todo remove.
				}




				if ((paramName == "Texture_ImageSamplerTemp13") || (paramName == "ImageImageSamplerTemp13"))	//s13
					pass->getTextureUnitState(reg)->setTextureName("empty.png");					//Todo look what is for (only one shader use it)


				if ((paramName == "Texture_ImageSamplerTemp14") || (paramName == "ImageSamplerTemp14"))		//s14
				{
					pass->getTextureUnitState(reg)->setTextureName(name + ".dyt_0");					//for character, it's a dyt texture (in xenoviwer I give the first texture, DATA000.dds of dyt.emb). it less influence the shader , so it's difficult to know what is for.
					pass->getTextureUnitState(reg)->setTextureFiltering(Ogre::TextureFilterOptions::TFO_NONE);
					pass->getTextureUnitState(reg)->setTextureAddressingMode(Ogre::TextureUnitState::TextureAddressingMode::TAM_CLAMP);
				}

				if ((paramName == "Texture_ImageSamplerTemp15") || (paramName == "ImageSamplerTemp15"))		//s15
					pass->getTextureUnitState(reg)->setTextureName("ImgEnvi_15.jpg");





				
				

				//ok, we found it's a sampler2D, but the information , the link with texturepacks, is definied in submesh, so we have information after.
				emm_material->getListSampler2D().push_back(EMMMaterial::Sampler2D_shaderDefinition(paramName, reg, size));
				isUsed = "sampler2D";
				isDone = true;
			}


			nbParams = boolListName.size();
			for (size_t j = 0; ((j < nbParams) && (!isDone)); j++)
			{
				if (boolListName.at(j) != paramName)
					continue;

				//case given by SDS:
				if (paramName == "g_bSkinning_VS")
				{
					bool skinningEnable = false;
					for (size_t k = 0; k < nbSdsdef; k++)
					{
						if (listSDS.at(k).name=="SkinningEnable")
						{
							skinningEnable = true;
							isUsed = true;
							break;
						}
					}
					
					skinningEnable = false;		//softwareSkinning
					//skinningEnable = true;	//hardwareSkinning

					defaultValue = Ogre::Vector4(skinningEnable ? 1.0f : 0.0f, 0, 0, 0);
					fp_parameters->setConstant(reg, defaultValue.x);
					
					//if (skinningEnable)
					//	program->setSkeletalAnimationIncluded(skinningEnable);				//it's sound good to do , but it's crash xenoviewer. todo find why (may be because of shared skeleton instance)

					isUsed = "bool";
				}
				
				//case givent by material
				if (paramName == "g_bOutputGlareMRT_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue.x);								//for not crahsing
					isUsed = "bool";
				}
				if (paramName.substr(0,12) == "g_bVersatile")		//g_bVersatile0_VS, g_bVersatile0_VS
				{
					params = emm_material->getParameter("VsFlag"+ paramName.substr(12, 1));
					defaultValue = Ogre::Vector4(((params) && (params->uint_value != 0)) ? 1.0f : 0.0f, 0, 0, 0);
					fp_parameters->setConstant(reg, defaultValue.x);
					isUsed = "bool";
				}

				if (paramName == "g_bFog_PS")
				{
					params = emm_material->getParameter("Fog");
					defaultValue = Ogre::Vector4(((params) && (params->uint_value != 0)) ? 1.0f : 0.0f, 0, 0, 0);
					fp_parameters->setConstant(reg, defaultValue.x);
					isUsed = "bool";
				}

				if (paramName == "g_bOutputDepthMRT_PS")
				{
					params = emm_material->getParameter("OutputDept");

					defaultValue = Ogre::Vector4(((params) && (params->uint_value != 0)) ? 1.0f : 0.0f, 0, 0, 0);
					fp_parameters->setConstant(reg, defaultValue.x);
					isUsed = "bool";
				}
				

				isDone = true;
			}





			nbParams = float4ListName.size();
			for (size_t j = 0; ((j < nbParams) && (!isDone)); j++)
			{
				if (float4ListName.at(j) != paramName)
					continue;


				//case given by material
				if (paramName.substr(0, 13) == "g_MaterialCol")			//for g_MaterialCol0_PS, g_MaterialCol1_PS,  ...
				{
					string indexMatCol = "MatCol" + paramName.substr(13, 1);
					Ogre::Vector4 vect_tmp(0, 0, 0, 1);

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if (paramName.substr(0, 15) == "g_MaterialScale")			//for g_MaterialScale0_PS, g_MaterialScale1_PS,  ...
				{
					string indexMatCol = "MatScale" + paramName.substr(15, 1);
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					params = emm_material->getParameter(indexMatCol + "X");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Y");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Z");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "W");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}


				if (paramName.substr(0, 7) == "g_vEdge")			//for g_vEdge_PS, g_vEdge_VS,  ...
				{
					string indexMatCol = "NoEdge";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					params = emm_material->getParameter(indexMatCol);
					if ((params)&&(params->uint_value == 1))
						vect_tmp *= 0;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if (paramName.substr(0, 11) == "g_EdgeColor")			//for g_EdgeColor_VS, g_EdgeColor_PS,  ...
				{
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);
					fp_parameters->setConstant(reg, vect_tmp);
					defaultValue = vect_tmp;
					isUsed = "float4";
				}
				


				if (paramName.substr(0, 16) == "g_MaterialOffset")			//for g_MaterialOffset0_VS, g_MaterialOffset1_VS, g_MaterialOffset0_PS, g_MaterialOffset1_PS,  ...
				{
					string indexMatCol = "MatOffset" + paramName.substr(16, 1);
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					EMMParameter* params_tmp = emm_material->getParameter("g_MaterialOffset" + indexMatCol + "_VS");			// for HUF_835, we have "g_MaterialOffset0_VS" directly, instead of "MatOffset0"
					if (!params_tmp)
						params_tmp = emm_material->getParameter("g_MaterialOffset" + indexMatCol + "_PS");
					if (params_tmp)
						vect_tmp.x = vect_tmp.y = vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "X");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Y");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Z");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "W");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}





				if ((paramName == "g_vTone") ||(paramName == "g_vTone_PS"))			//color multiplicator at the end.
				{
					string indexMatCol = "Tone";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					params = emm_material->getParameter(indexMatCol + "X");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Y");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Z");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "W");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}


				if (paramName.substr(0, 10) == "g_vTexTile")						//g_vTexTile01_VS, ... use with UVscroll to scroll into a textureTile. The question is, it is the value for TextScale ? witch is on textureDefinition on emd ?
				{
					size_t index = std::stoi(paramName.substr(10, 2));
					if (listRegForTextTile.size() <= index)
						listRegForTextTile.resize(index +1 , (size_t)-1 );
					listRegForTextTile.at(index) = reg;
					
					string indexMatCol = "TextTile"+ paramName.substr(10, 2);
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					params = emm_material->getParameter(indexMatCol + "X");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Y");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "Z");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "W");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if (paramName.substr(0, 11) == "g_TexScroll")			//for g_TexScroll0_VS, g_TexScroll1_VS, g_TexScroll0_PS, g_TexScroll1_PS
				{
					string indexMatCol = "TexScrl" + paramName.substr(11, 1);
					Ogre::Vector4 vect_tmp(0, 0, 0, 0);

					params = emm_material->getParameter(indexMatCol + "U");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "V");
					if (params)
						vect_tmp.y = params->float_value;


					params = emm_material->getParameter("gScrollU" + paramName.substr(11, 1));
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("gScrollV" + paramName.substr(11, 1));
					if (params)
						vect_tmp.y = params->float_value;

					if (paramName.substr(11, 1) == "0")
					{
						params = emm_material->getParameter("gScrollU");
						if (params)
							vect_tmp.x = params->float_value;

						params = emm_material->getParameter("gScrollV");
						if (params)
							vect_tmp.y = params->float_value;
					}


					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}


				if (paramName == "g_AlphaFade_VS")
				{
					Ogre::Vector4 vect_tmp(0.0, 1.0, 0, 0);

					params = emm_material->getParameter("FadeInit");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("FadeSpeed");
					if (params)
						vect_tmp.y = params->float_value;


					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if (paramName == "g_Gradient_VS")
				{
					Ogre::Vector4 vect_tmp(0.0, 1.0, 0, 0);

					params = emm_material->getParameter("GradientInit");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("GradientSpeed");
					if (params)
						vect_tmp.y = params->float_value;


					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}


				if (paramName == "g_Incidence_VS")
				{
					Ogre::Vector4 vect_tmp(1.0, 0.0, 0, 0);

					
					params = emm_material->getParameter("IncidencePower");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("IncidenceAlphaBias");
					if (params)
						vect_tmp.y = params->float_value;
					

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if (paramName == "g_GlareCoeff_VS")
				{
					string indexMatCol = "GlareCol";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					params = emm_material->getParameter(indexMatCol + "R");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "G");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "B");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "A");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if ((paramName == "g_vFogMultiColor") || (paramName == "g_vFogMultiColor_PS"))
				{
					string indexMatCol = "FogMultiCol";						//todo check
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);

					params = emm_material->getParameter(indexMatCol + "R");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "G");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "B");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "A");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if ((paramName == "g_vFogAddColor") || (paramName == "g_vFogAddColor_PS"))
				{
					string indexMatCol = "FogAddCol";						//todo check
					Ogre::Vector4 vect_tmp(0, 0, 0, 0);

					params = emm_material->getParameter(indexMatCol + "R");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter(indexMatCol + "G");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter(indexMatCol + "B");
					if (params)
						vect_tmp.z = params->float_value;

					params = emm_material->getParameter(indexMatCol + "A");
					if (params)
						vect_tmp.w = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}


				if (paramName == "g_vSpecular_PS")
				{
					Ogre::Vector4 vect_tmp(0, 0, 0, 0);

					params = emm_material->getParameter("SpcCoeff");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("SpcPower");
					if (params)
						vect_tmp.y = params->float_value;


					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if ((paramName == "g_vRim_VS") || (paramName == "g_vRim_PS"))
				{
					Ogre::Vector4 vect_tmp(0, 0, 0, 0);

					params = emm_material->getParameter("RimCoeff");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("RimPower");
					if (params)
						vect_tmp.y = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if ((paramName == "g_Reflection_VS") || (paramName == "g_Reflection_PS"))
				{
					Ogre::Vector4 vect_tmp(0, 0, 0, 0);

					params = emm_material->getParameter("ReflectCoeff");
					if (params)
						vect_tmp.x = params->float_value;

					params = emm_material->getParameter("ReflectFresnelBias");
					if (params)
						vect_tmp.y = params->float_value;

					params = emm_material->getParameter("ReflectFresnelCoeff");
					if (params)
						vect_tmp.z = params->float_value;

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}
				

				if (paramName == "g_vAlphaTest_PS")							//DBXv2
				{
					Ogre::Vector4 vect_tmp(0.0, 0.0, 0.0, 0.0);

					params = emm_material->getParameter("AlphaTest");
					if (params)
						vect_tmp = Ogre::Vector4( ((float)(params->uint_value)) / 255.0, ((float)(params->uint_value)) / 255.0, ((float)(params->uint_value)) / 255.0, 1.0);

					fp_parameters->setConstant(reg, vect_tmp);

					defaultValue = vect_tmp;
					isUsed = "float4";
				}


				






				//case given by Ogre system:
				if(paramName.substr(0,11) == "g_vLightVec")															//g_vLightVec0_VS, g_vLightVec0_PS, g_vLightVec1_VS, g_vLightVec1_PS, etc ...
				{
					size_t index = Ogre::StringConverter::parseUnsignedInt(paramName.substr(11, 1));
					
					string indexMatCol = "gLightDir";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);
					bool useMatParam = false;

					params = emm_material->getParameter(indexMatCol);
					if (params) { vect_tmp.x = vect_tmp.y = vect_tmp.z = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) { vect_tmp.x = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) { vect_tmp.y = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) { vect_tmp.z = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) { vect_tmp.w = params->float_value; useMatParam = true; }

					if (useMatParam)
					{
						fp_parameters->setConstant(reg, vect_tmp);
						defaultValue = vect_tmp;
					}
					else {
						fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE, index);
					}
					isUsed = "float4";
				}
				if(paramName.substr(0,11) == "g_vLightDif")															//g_vLightDif0_VS, g_vLightDif0_PS, g_vLightDif1_VS, g_vLightDif1_PS, etc ...
				{
					size_t index = Ogre::StringConverter::parseUnsignedInt(paramName.substr(11, 1));
					
					string indexMatCol = "MatDif";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);
					bool useMatParam = false;

					params = emm_material->getParameter(indexMatCol);
					if (params) { vect_tmp *= params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) { vect_tmp.x = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) { vect_tmp.y = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) { vect_tmp.z = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) { vect_tmp.w = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "Scale");
					if (params) { vect_tmp.w = params->float_value; useMatParam = true; }

					if (useMatParam)
					{
						fp_parameters->setConstant(reg, vect_tmp);
						defaultValue = vect_tmp;
					}else {
						fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR, index);
					}
					isUsed = "float4";
				}
				if(paramName.substr(0, 11) == "g_vLightSpc")													//g_vLightSpc0_VS, g_vLightSpc0_PS, g_vLightSpc1_VS, g_vLightSpc1_PS, etc ...
				{
					size_t index = Ogre::StringConverter::parseUnsignedInt(paramName.substr(11, 1));

					string indexMatCol = "MatSpc";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);
					bool useMatParam = false;

					params = emm_material->getParameter(indexMatCol);
					if (params) { vect_tmp *= params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) { vect_tmp.x = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) { vect_tmp.y = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) { vect_tmp.z = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) { vect_tmp.w = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "Scale");
					if (params) { vect_tmp.w = params->float_value; useMatParam = true; }

					if (useMatParam)
					{
						fp_parameters->setConstant(reg, vect_tmp);
						defaultValue = vect_tmp;
					}else{
						fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR, index);
					}
					isUsed = "float4";
				}

				

				if (paramName == "g_vEyePos_VS")
				{
					string indexMatCol = "gCamPosR";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);
					bool useMatParam = false;

					params = emm_material->getParameter(indexMatCol);
					if (params) { vect_tmp.x = vect_tmp.y = vect_tmp.z = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) { vect_tmp.x = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) { vect_tmp.y = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) { vect_tmp.z = params->float_value; useMatParam = true; }

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) { vect_tmp.w = params->float_value; useMatParam = true; }

					if (useMatParam)
					{
						fp_parameters->setConstant(reg, vect_tmp);
						defaultValue = vect_tmp;
					}
					else {
						fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
					}
					isUsed = "float4";
				}
				




				if (paramName == "g_vHeightFog_VS")
				{
					
					//xy: height_face and add to have max fog on 200 and start at 100 :
					//	([100, 200] - 100) / 100 <=> ([100, 200] / 100) - 1 <=> ([100, 200] * y) + x => [0, 1]
					//zw: same for distance (pos.w) : from 2000 to 3000: ([2000, 3000] / 1000) - 2 => [0, 1]
					defaultValue = Ogre::Vector4(-1.0, 1.0 / 100.0, -2.0, 1.0 / 1000.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vFog_VS")
				{
					//same as g_vHeightFog_VS.zw, but for multiply 
					defaultValue = Ogre::Vector4(-2.0, 1.0 / 1000.0, -2.0, 1.0 / 1000.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vFogMultiColor_PS")
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
					isUsed = "float4";
				}
				if (paramName == "g_vFogAddColor_PS")
				{
					defaultValue = Ogre::Vector4(0, 0, 0, 0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}

				
				
				
				if ((paramName == "g_SystemTime_VS") || (paramName == "g_ElapsedTime_VS") ||(paramName == "g_SystemTime_PS") || (paramName == "g_ElapsedTime_PS"))
				{
					bool useMatParam = false;
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);


					params = emm_material->getParameter("gTime");
					if (params) { vect_tmp *= params->float_value; useMatParam = true; }


					if (useMatParam)
					{
						fp_parameters->setConstant(reg, vect_tmp);
						defaultValue = vect_tmp;
					}
					else {
						fp_parameters->setAutoConstantReal(reg, Ogre::GpuProgramParameters::ACT_TIME, 1.0f);
					}
					isUsed = "float4";
				}

				
				

				//case gived by defaults values from previous code (the difference is registers are not hardcoded). 
				//TODO understand where come from this parameters
				if (paramName == "g_vColor0_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor1_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					//defaultValue = Ogre::Vector4(1.0, 1.0, 1.0, 1.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor2_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor3_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor4_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor5_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor12_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vColor13_PS")
				{
					defaultValue = Ogre::Vector4(0, 0, 0, 1);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}


				if (paramName == "g_vShadowMap_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vShadowColor_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vShadowParam_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				

				if (paramName == "g_vUserFlag0_VS")							//DBXv2 CustomFlag
				{
					defaultValue = Ogre::Vector4(1, 0, 0, 1); 			//test Todo remttre a 0
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vUserFlag1_VS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(1, 0, 0, 1);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vUserFlag2_VS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(1, 0, 0, 1);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vUserFlag3_VS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(1, 0, 0, 1);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				
				if (paramName == "g_vHemiA_VS")								//DBXv2
				{
					defaultValue = Ogre::Vector4(0.2);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vHemiB_VS")								//DBXv2
				{
					defaultValue = Ogre::Vector4(0.2);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vHemiC_VS")								//DBXv2
				{
					defaultValue = Ogre::Vector4(0.2);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if ((paramName == "g_vSubSurface_VS") || (paramName == "g_vSubSurface_PS"))			//DBXv2  => subSurfaceScaterring ?
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				
				
				if (paramName == "g_vFadeAdd_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vFadeMulti_PS")
				{
					defaultValue = Ogre::Vector4(1.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vFadeRim_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vRimColor_VS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vAltFadeAdd_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vAltFadeMulti_PS")
				{
					defaultValue = Ogre::Vector4(1.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}

				
				
				// Color Multiplier
				if (paramName == "g_vParam0_VS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(1.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vParam0_PS")							//DBXv2
				{
					defaultValue = Ogre::Vector4(1.0, 0.0, 1.0, 1.0);			//AFnmcWTR.nsk water , BIRD_Water_HL_BM_E_S_PS.xpu.hlsl
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}

				if (paramName == "g_vParam3_PS")
				{
					defaultValue = Ogre::Vector4(1.0);
					fp_parameters->setConstant(reg, defaultValue);		//x: for dyt color, y : for color from samlpe14 (second dyt) , so test normal RAD for unbreak shaders.
																		// and est with dyt.emb.data001.dds on samlper14.
					isUsed = "float4";
				}
				if (paramName == "g_vParam4_PS")
				{
					//defaultValue = Ogre::Vector4(0.0);
					defaultValue = Ogre::Vector4(0.25, 0.75, 0, 0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vParam5_PS")
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vParam7_PS")								//Toon Detail Parameter 
				{
					defaultValue = Ogre::Vector4(0.0, 23.2558f, 0.04587f, 0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}

				if (paramName == "g_vParam11_PS")
				{
					defaultValue = Ogre::Vector4(1,1,0.22,0.25);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vParam12_PS")
				{
					defaultValue = Ogre::Vector4(0.0, 0, 0, 1);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				if (paramName == "g_vParam13_PS")
				{
					defaultValue = Ogre::Vector4(1.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}
				

				if (paramName == "g_vGlare_PS")								//DBXv2
				{
					string indexMatCol = "Glare";
					Ogre::Vector4 vect_tmp(0, 0, 0, 0);

					params = emm_material->getParameter(indexMatCol);
					if (params) { vect_tmp = params->float_value; }

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) { vect_tmp.x = params->float_value; }

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) { vect_tmp.y = params->float_value; }

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) { vect_tmp.z = params->float_value; }

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) { vect_tmp.w = params->float_value; }

					fp_parameters->setConstant(reg, vect_tmp);
					defaultValue = vect_tmp;
					isUsed = "float4";
				}

				if (paramName == "g_vLerp_PS")								//DBXv2. may be lerp between WVP and WVP_prev
				{
					defaultValue = Ogre::Vector4(0.0);
					fp_parameters->setConstant(reg, defaultValue);
					isUsed = "float4";
				}


				if((paramName == "g_vAmbUni_VS")||(paramName  == "g_vAmbUni_PS"))								//DBXv2. for the second framebuffer, like a general base ambiente for postEffect.
				{
					string indexMatCol = "MatAmb";
					Ogre::Vector4 vect_tmp(1, 1, 1, 1);
					bool useParam = false;

					params = emm_material->getParameter(indexMatCol);
					if (params) { vect_tmp *= params->float_value; useParam = true; }

					params = emm_material->getParameter(indexMatCol + "R");
					if (params) { vect_tmp.x = params->float_value; useParam = true;}

					params = emm_material->getParameter(indexMatCol + "G");
					if (params) { vect_tmp.y = params->float_value; useParam = true; }

					params = emm_material->getParameter(indexMatCol + "B");
					if (params) { vect_tmp.z = params->float_value; useParam = true; }

					params = emm_material->getParameter(indexMatCol + "A");
					if (params) { vect_tmp.w = params->float_value; useParam = true; }

					params = emm_material->getParameter(indexMatCol + "Scale");
					if (params) { vect_tmp.w = params->float_value; useParam = true; }

					vect_tmp *= Ogre::Vector4(0.5, 0.5, 0.5, 1.0);


					if(useParam)
						fp_parameters->setConstant(reg, vect_tmp);
					else
						fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);

					pass->setAmbient(vect_tmp.x, vect_tmp.y, vect_tmp.z);
					defaultValue = vect_tmp;
					isUsed = "float4";
				}
				

				

				isDone = true;
			}



			nbParams = float4x4ListName.size();
			for (size_t j = 0; ((j < nbParams) && (!isDone)); j++)
			{
				if (float4x4ListName.at(j) != paramName)
					continue;

				//case given by Ogre system:
				if (paramName == "g_mW_VS")									// Matrix World
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
					isUsed = "float4x4";
				}
				if (paramName == "g_mV_VS")									// Matrix View
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_VIEW_MATRIX);
					isUsed = "float4x4";
				}
				if (paramName == "g_mP_VS")									// Matrix Projection
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_PROJECTION_MATRIX);
					isUsed = "float4x4";
				}

				if (paramName == "g_mWV_VS")								// Matrix World View
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX);
					isUsed = "float4x4";
				}
				if (paramName == "g_mVP_VS")								// Matrix View Proj
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_VIEWPROJ_MATRIX);
					isUsed = "float4x4";
				}

				if (paramName == "g_mWVP_VS")								// Matrix World View Proj
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
					isUsed = "float4x4";
				}

				if (paramName == "g_mWVP_Prev_VS")							//DBXv2. may be previous Matrix World View Proj ? TODO test it
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
					isUsed = "float4x4";
				}


				

				if (paramName == "g_mWLP_SM_VS")							//DBXv2. World Light Projection (SM = ShadowMap)
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_SPOTLIGHT_WORLDVIEWPROJ_MATRIX, 0);
					isUsed = "float4x4";
				}
				if (paramName == "g_mWLPB_SM_VS")							//DBXv2. World Light Projection (SM = ShadowMap), (B ? backward ?) ShadowMap, or may be B for a second one ? because of g_mWLP_SM_VS (and g_mWLP_PM_VS, g_mWLPB_PM_VS).
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_SPOTLIGHT_WORLDVIEWPROJ_MATRIX, 1);
					isUsed = "float4x4";
				}
				
				if (paramName == "g_mWLP_PM_VS")							//DBXv2. World Light Projection, projection matrix ? Todo look deeper into TOON shader
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_SPOTLIGHT_WORLDVIEWPROJ_MATRIX, 0);
					isUsed = "float4x4";
				}
				if (paramName == "g_mWLPB_PM_VS")							//DBXv2. World Light Projection, projection matrix ? Todo look deeper into TOON shader
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_SPOTLIGHT_WORLDVIEWPROJ_MATRIX, 1);
					isUsed = "float4x4";
				}


				isDone = true;
			}


			nbParams = float4x3ListName.size();
			for (size_t j = 0; ((j < nbParams) && (!isDone)); j++)
			{
				if (float4x3ListName.at(j) != paramName)
					continue;

				//case given by Ogre system:
				if (paramName == "g_mMatrixPalette_VS")								// Matrix Worl View Proj
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4);

					//if (source.find("g_bSkinning_VS") == string::npos)						//there is for sheron SIN_BG_00.nsk, a shader with bone, but not the skinning activation parameter,so we are oblig to do skinning. Todo check crash.
					//	program->setSkeletalAnimationIncluded(true);

					isUsed = "float4x3";
				}
				if (paramName == "g_mMatrixPalettePrev_VS")								// Matrix Worl View Proj, todo loo after the "Prev", may be for motionBlur.
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4);
					isUsed = "float4x3";
				}
				
				if (paramName == "g_mWV_VS")								// Matrix Worl View par contre Ogre ne delivre pas une version en 4x3
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLDVIEW_MATRIX_4x3);
					isUsed = "float4x3";
				}

				if (paramName == "g_mW_VS")								// Matrix Worl par contre Ogre ne delivre pas une version en 4x3
				{
					fp_parameters->setAutoConstant(reg, Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_4x3);
					isUsed = "float4x3";
				}

				isDone = true;
			}


			if (isUsed=="")
			{
				string ab = paramName;
				//todo warning
				int aa = 42;
				//assert(false);	//if pass here, complete by reading shaders
			}else {
				materialParameters.push_back(EmmMaterialParameter(paramName, isUsed, reg, shaderType));
				if ((isUsed == "bool") || (isUsed == "float4"))
					materialParameters.back().defaultValue = defaultValue;
			}
		}

	}

	return listRegForTextTile;
}


void EMMOgre::createOgreMaterials(std::vector<SDS*> &sds_list, Ogre::Viewport* viewport)
{
	if (material_resources_created)
		return;

	for (size_t i = 0; i < materials.size(); i++)
		createOgreMaterial(materials[i], sds_list, viewport);
	material_resources_created = true;
}





std::vector<EMMOgre::EmmMaterialParameter>*	EMMOgre::getShaderParameters(string ogreMaterialName)
{
	string strToSearch = "_" + ogreMaterialName;
	Ogre::StringUtil::toLowerCase(strToSearch);

	size_t nbCharac = strToSearch.length();
	

	string toTest;
	size_t nbCharaTest;

	size_t nbMat = created_materials.size();
	for (size_t i = 0; i < nbMat; i++)
	{
		toTest = created_materials.at(i).name;
		nbCharaTest = toTest.length();
		
		string testToRemove = toTest.substr(nbCharaTest - nbCharac, nbCharac);

		if ((nbCharaTest >= nbCharac) && (toTest.substr(nbCharaTest - nbCharac, nbCharac)== strToSearch))
			return &created_materials.at(i).parameter;
	}

	return 0;
}


EMMOgre::EmmMaterialParameter	EMMOgre::getShaderParameter(string ogreMaterialName, string parameterName)
{
	size_t nbMat = created_materials.size();
	for (size_t i = 0; i < nbMat; i++)
	{
		if (created_materials.at(i).name== ogreMaterialName)
		{
			EmmMaterialCreated &mat = created_materials.at(i);
			size_t nbParam = mat.parameter.size();
			for (size_t j = 0; j < nbParam; j++)
			{
				if (mat.parameter.at(j).name == parameterName)
					return mat.parameter.at(j);
			}
			return EmmMaterialParameter("", "", 0, "");
		}
	}

	return EmmMaterialParameter("","",0, "");
}


bool EMMOgre::setShaderParameter(string ogreMaterialName, string name, bool value)
{
	Ogre::MaterialPtr mat = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(ogreMaterialName);
	if (mat.isNull())
		return false;
	
	EmmMaterialParameter param = getShaderParameter(ogreMaterialName, name);
	if (param.name.length() == 0)
		return false;

	
	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

	Ogre::GpuProgramParametersSharedPtr fp_parameters;
	if((param.shaderType=="vertex")&&(pass->hasVertexProgram()))
		fp_parameters = pass->getVertexProgramParameters();
	if ((param.shaderType == "fragment") && (pass->hasFragmentProgram()))
		fp_parameters = pass->getFragmentProgramParameters();

	if (fp_parameters.isNull())
		return false;

	if (param.type == "bool")
		fp_parameters->setConstant(param.reg, value);
	else if (param.type == "float4")
		fp_parameters->setConstant(param.reg, Ogre::Vector4(((value) ? 1.0 : 0.0), 0, 0, 0));
	else if (param.type == "float4x4")
		return false;
	else if (param.type == "float4x3")
		return false;
	else if (param.type == "sampler2D")
		return false;

	return true;
}
bool EMMOgre::setShaderParameter(string ogreMaterialName, string name, float value)
{
	Ogre::MaterialPtr mat = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(ogreMaterialName);
	if (mat.isNull())
		return false;

	EmmMaterialParameter param = getShaderParameter(ogreMaterialName, name);
	if (param.name.length() == 0)
		return false;


	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

	Ogre::GpuProgramParametersSharedPtr fp_parameters;
	if ((param.shaderType == "vertex") && (pass->hasVertexProgram()))
		fp_parameters = pass->getVertexProgramParameters();
	if ((param.shaderType == "fragment") && (pass->hasFragmentProgram()))
		fp_parameters = pass->getFragmentProgramParameters();

	if (fp_parameters.isNull())
		return false;

	if (param.type == "bool")
		fp_parameters->setConstant(param.reg, value==1.0f);
	else if (param.type == "float4")
		fp_parameters->setConstant(param.reg, Ogre::Vector4(value, 0, 0, 0));
	else if (param.type == "float4x4")
		return false;
	else if (param.type == "float4x3")
		return false;
	else if (param.type == "sampler2D")
		return false;

	return true;
}
bool EMMOgre::setShaderParameter(string ogreMaterialName, string name, Ogre::Vector3 value)
{
	Ogre::MaterialPtr mat = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(ogreMaterialName);
	if (mat.isNull())
		return false;

	EmmMaterialParameter param = getShaderParameter(ogreMaterialName, name);
	if (param.name.length() == 0)
		return false;


	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

	Ogre::GpuProgramParametersSharedPtr fp_parameters;
	if ((param.shaderType == "vertex") && (pass->hasVertexProgram()))
		fp_parameters = pass->getVertexProgramParameters();
	if ((param.shaderType == "fragment") && (pass->hasFragmentProgram()))
		fp_parameters = pass->getFragmentProgramParameters();

	if (fp_parameters.isNull())
		return false;

	if (param.type == "bool")
		fp_parameters->setConstant(param.reg, value.x == 1.0);
	else if (param.type == "float4")
		fp_parameters->setConstant(param.reg, Ogre::Vector4(value.x, value.y, value.z, 1.0));
	else if (param.type == "float4x4")
		return false;
	else if (param.type == "float4x3")
		return false;
	else if (param.type == "sampler2D")
		return false;

	return true;
}
bool EMMOgre::setShaderParameter(string ogreMaterialName, string name, Ogre::Vector4 value)
{
	Ogre::MaterialPtr mat = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(ogreMaterialName);
	if (mat.isNull())
		return false;

	EmmMaterialParameter param = getShaderParameter(ogreMaterialName, name);
	if (param.name.length() == 0)
		return false;


	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

	Ogre::GpuProgramParametersSharedPtr fp_parameters;
	if ((param.shaderType == "vertex") && (pass->hasVertexProgram()))
		fp_parameters = pass->getVertexProgramParameters();
	if ((param.shaderType == "fragment") && (pass->hasFragmentProgram()))
		fp_parameters = pass->getFragmentProgramParameters();

	if (fp_parameters.isNull())
		return false;

	if (param.type == "bool")
		fp_parameters->setConstant(param.reg, value.x == 1.0);
	else if (param.type == "float4")
		fp_parameters->setConstant(param.reg, value);
	else if (param.type == "float4x4")
		return false;
	else if (param.type == "float4x3")
		return false;
	else if (param.type == "sampler2D")
		return false;

	return true;
}
bool EMMOgre::setShaderParameter(string ogreMaterialName, string name, string value)
{
	Ogre::MaterialPtr mat = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(ogreMaterialName);
	if (mat.isNull())
		return false;

	EmmMaterialParameter param = getShaderParameter(ogreMaterialName, name);
	if (param.name.length() == 0)
		return false;

	if (param.type != "sampler2D")
		return false;

	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

	pass->getTextureUnitState(param.reg)->setTextureName(value);

	return true;
}



void EMMOgre::setShaderParameter(string name, Ogre::Vector4 value)
{
	size_t nbMat = created_materials.size();
	for (size_t i = 0; i < nbMat; i++)
	{
		Ogre::MaterialPtr mat = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(created_materials.at(i).name);
		if (mat.isNull())
			continue;

		EmmMaterialParameter param = getShaderParameter(created_materials.at(i).name, name);
		if (param.name.length() == 0)
			continue;

		Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

		Ogre::GpuProgramParametersSharedPtr fp_parameters;
		if ((param.shaderType == "vertex") && (pass->hasVertexProgram()))
			fp_parameters = pass->getVertexProgramParameters();
		if ((param.shaderType == "fragment") && (pass->hasFragmentProgram()))
			fp_parameters = pass->getFragmentProgramParameters();

		if (fp_parameters.isNull())
			continue;

		if (param.type == "bool")
			fp_parameters->setConstant(param.reg, value.x == 1.0);
		else if (param.type == "float4")
			fp_parameters->setConstant(param.reg, value);
		else if (param.type == "float4x4")
			continue;
		else if (param.type == "float4x3")
			continue;
		else if (param.type == "sampler2D")
			continue;
	}
}


string EMMOgre::getCreatedMaterialName(string materialName)
{
	string toTest = "_" + materialName;
	size_t nbCharac = toTest.length();
	size_t nbMat = created_materials.size();
	for (size_t i = 0; i < nbMat; i++)
	{
		string testedName = created_materials.at(i).name;
		if ((testedName.length() >= nbCharac)&&(testedName.substr(testedName.length()- nbCharac)== toTest))
			return testedName;
	}
	return toTest;
}

EMMOgre::EmmMaterialCreated* EMMOgre::getEmmMaterialCreated(string materialName)
{
	size_t nbMat = created_materials.size();
	for (size_t i = 0; i < nbMat; i++)
	{
		if (created_materials.at(i).name == materialName)
			return &created_materials.at(i);
	}
	return 0;
}

void EMMOgre::destroyResources()
{
	// FIXME: Implement
	material_resources_created = false;
}

EMMOgre::~EMMOgre()
{
	delete texture_pack;
	delete texture_dyt_pack;
	delete texture_player_dyt_pack;

	list<Ogre::String> created_materials;
	for (list<Ogre::String>::iterator it = created_materials.begin(); it != created_materials.end(); it++)
		Ogre::MaterialManager::getSingleton().remove(*it);
}





void EMMOgre::changeAnimation()
{
	stopAnimation_1();

	if (animation_to_change)
		ema_Material_Anim = animation_to_change->getEmaAnimation();
	
	animation_to_change = 0;
}
void EMMOgre::changeAnimation2()
{
	stopAnimation_2();

	if (animation_to_change2)
		ema_Material_Anim2 = animation_to_change2->getEmaAnimation();

	animation_to_change2 = 0;
}



string EMMOgre::getShaderParamNameFromEmmParamName(string paramName)
{
	if ((paramName.length() > 6) && (paramName.substr(0, 6) == "MatCol"))			//for g_MaterialCol0_PS, g_MaterialCol1_PS,  ...
	{
		return "g_MaterialCol" + paramName.substr(6, 1);

	}else if ((paramName.length() > 7) && (paramName.substr(0, 7) == "TexScrl")){
		return "g_TexScroll" + paramName.substr(7, 1);
	}

	return "";
}




void EMMOgre::stopAnimation()
{
	stopAnimation_1();
	stopAnimation_2();
}

void EMMOgre::stopAnimation_1()
{
	if (!ema_Material_Anim)
		return;

	vector<EMAAnimation_ByNode> &nodes = ema_Material_Anim->getOrganizedNodes();
	for (size_t i = 0, nb = nodes.size(); i < nb; i++)
	{
		EMAAnimation_ByNode& node = nodes.at(i);

		//Here, saddly, there is 2 posisblities:
		string name_tmp = "";
		if (node.GetBone())
		{
			name_tmp = node.GetBone()->GetName();			//get concerned EmmMaterial by name
			
			bool isfound = false;
			for (size_t j = 0, nb2 = created_materials.size(); j < nb2; j++)	//search in emm the same name.			
			{
				if (created_materials.at(j).emmMaterialOrigine->getName() == name_tmp)
				{
					isfound = true;
					break;
				}
			}

			if (!isfound)
				name_tmp = "";
		}else {														//but some file just don't have the same name , so , use the index instead.
			if (node.getBoneIndex() < created_materials.size())
				name_tmp = created_materials.at(node.getBoneIndex()).emmMaterialOrigine->getName();
		}
		if (name_tmp.length() == 0)
			continue;

		string ogreMatName = name + "_" + name_tmp;
		Ogre::StringUtil::toLowerCase(ogreMatName);



		std::vector<EMAAnimation_ByNode_ByTransform> &transforms = node.GetTransforms();		//equivalent of MaterialParameters
		for (size_t j = 0, nb2 = transforms.size(); j < nb2; j++)
		{
			EMAAnimation_ByNode_ByTransform &transform = transforms.at(j);		//equivalent of MaterialParameter

			string shadeParamName = getShaderParamNameFromEmmParamName(transform.GetComponents().at(0)->transformNameForMaterialAnim());
			if (shadeParamName.length() == 0)
				continue;

			EmmMaterialParameter emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_VS");
			if (emmMatParam.name.length())
				setShaderParameter(ogreMatName, emmMatParam.name, emmMatParam.defaultValue);

			emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_PS");
			if (emmMatParam.name.length())
				setShaderParameter(ogreMatName, emmMatParam.name, emmMatParam.defaultValue);
		}
	}
	ema_Material_Anim = 0;
}
void EMMOgre::stopAnimation_2()
{
	if (!ema_Material_Anim2)
		return;

	vector<EMAAnimation_ByNode> &nodes = ema_Material_Anim2->getOrganizedNodes();
	for (size_t i = 0, nb = nodes.size(); i < nb; i++)
	{
		EMAAnimation_ByNode& node = nodes.at(i);

		//Here, saddly, there is 2 posisblities:
		string name_tmp = "";
		if (node.GetBone())
		{
			name_tmp = node.GetBone()->GetName();			//get concerned EmmMaterial by name

			bool isfound = false;
			for (size_t j = 0, nb2 = created_materials.size(); j < nb2; j++)	//search in emm the same name.			
			{
				if (created_materials.at(j).emmMaterialOrigine->getName() == name_tmp)
				{
					isfound = true;
					break;
				}
			}

			if (!isfound)
				name_tmp = "";
		}else {														//but some file just don't have the same name , so , use the index instead.
			if (node.getBoneIndex() < created_materials.size())
				name_tmp = created_materials.at(node.getBoneIndex()).emmMaterialOrigine->getName();
		}
		if (name_tmp.length() == 0)
			continue;

		string ogreMatName = name + "_" + name_tmp;
		Ogre::StringUtil::toLowerCase(ogreMatName);



		std::vector<EMAAnimation_ByNode_ByTransform> &transforms = node.GetTransforms();		//equivalent of MaterialParameters
		for (size_t j = 0, nb2 = transforms.size(); j < nb2; j++)
		{
			EMAAnimation_ByNode_ByTransform &transform = transforms.at(j);		//equivalent of MaterialParameter

			string shadeParamName = getShaderParamNameFromEmmParamName(transform.GetComponents().at(0)->transformNameForMaterialAnim());
			if (shadeParamName.length() == 0)
				continue;

			EmmMaterialParameter emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_VS");
			if (emmMatParam.name.length())
				setShaderParameter(ogreMatName, emmMatParam.name, emmMatParam.defaultValue);

			emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_PS");
			if (emmMatParam.name.length())
				setShaderParameter(ogreMatName, emmMatParam.name, emmMatParam.defaultValue);
		}
	}
	ema_Material_Anim2 = 0;
}


void EMMOgre::updateAnimations(Ogre::Real time)
{
	if (ema_Material_Anim)
	{
		Ogre::Real time_tmp = (time * 60.0);
		if (time_tmp > ema_Material_Anim->GetDuration())
		{
			Ogre::Real nbFrames = ema_Material_Anim->GetDuration();
			time_tmp = (animationLoopEnable) ? (time_tmp - Ogre::Math::Floor(time_tmp/ nbFrames) * nbFrames) : (nbFrames - 1);
		}

		vector<EMAAnimation_ByNode> &nodes = ema_Material_Anim->getOrganizedNodes();
		for (size_t i = 0, nb = nodes.size(); i < nb; i++)
		{
			EMAAnimation_ByNode& node = nodes.at(i);

			if (node.getBoneIndex() >= created_materials.size())
				continue;
			
			string ogreMatName = name + "_" + created_materials.at(node.getBoneIndex()).emmMaterialOrigine->getName();
			Ogre::StringUtil::toLowerCase(ogreMatName);


			std::vector<EMAAnimation_ByNode_ByTransform> &transforms = node.GetTransforms();		//equivalent of MaterialParameters
			for (size_t j = 0, nb2 = transforms.size(); j < nb2; j++)
			{
				EMAAnimation_ByNode_ByTransform &transform = transforms.at(j);		//equivalent of MaterialParameter

				string shadeParamName = getShaderParamNameFromEmmParamName(transform.GetComponents().at(0)->transformNameForMaterialAnim());
				if (shadeParamName.length() == 0)
					continue;

				EmmMaterialParameter emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_VS");
				if (emmMatParam.name.length())
				{
					EANKeyframe kf = transform.getInterpolatedKeyframe((float)time_tmp, ema_Material_Anim->getValues(), emmMatParam.defaultValue.x, emmMatParam.defaultValue.y, emmMatParam.defaultValue.z, emmMatParam.defaultValue.w);
					Ogre::Vector4 value(kf.x, kf.y, kf.z, kf.w);
					setShaderParameter(ogreMatName, emmMatParam.name, value);
				}

				emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_PS");
				if (emmMatParam.name.length())
				{
					EANKeyframe kf = transform.getInterpolatedKeyframe((float)time_tmp, ema_Material_Anim->getValues(), emmMatParam.defaultValue.x, emmMatParam.defaultValue.y, emmMatParam.defaultValue.z, emmMatParam.defaultValue.w);
					Ogre::Vector4 value(kf.x, kf.y, kf.z, kf.w);
					setShaderParameter(ogreMatName, emmMatParam.name, value);
				}
			}
		}
	}


	if (ema_Material_Anim2)
	{
		Ogre::Real time_tmp = (time * 60.0);
		if (time_tmp > ema_Material_Anim2->GetDuration())
		{
			Ogre::Real nbFrames = ema_Material_Anim2->GetDuration();
			time_tmp = (animationLoopEnable) ? (time_tmp - Ogre::Math::Floor(time_tmp / nbFrames) * nbFrames) : (nbFrames - 1);
		}

		vector<EMAAnimation_ByNode> &nodes = ema_Material_Anim2->getOrganizedNodes();
		for (size_t i = 0, nb = nodes.size(); i < nb; i++)
		{
			EMAAnimation_ByNode& node = nodes.at(i);

			//Here, saddly, there is 2 posisblities:
			string name_tmp = "";
			if (node.GetBone())
			{
				name_tmp = node.GetBone()->GetName();			//get concerned EmmMaterial by name

				bool isfound = false;
				for (size_t j = 0, nb2 = created_materials.size(); j < nb2; j++)	//search in emm the same name.			
				{
					if (created_materials.at(j).emmMaterialOrigine->getName() == name_tmp)
					{
						isfound = true;
						break;
					}
				}

				if (!isfound)
					name_tmp = "";
			}
			else {														//but some file just don't have the same name , so , use the index instead.
				if (node.getBoneIndex() < created_materials.size())
					name_tmp = created_materials.at(node.getBoneIndex()).emmMaterialOrigine->getName();
			}
			if (name_tmp.length() == 0)
				continue;

			string ogreMatName = name + "_" + name_tmp;
			Ogre::StringUtil::toLowerCase(ogreMatName);


			std::vector<EMAAnimation_ByNode_ByTransform> &transforms = node.GetTransforms();		//equivalent of MaterialParameters
			for (size_t j = 0, nb2 = transforms.size(); j < nb2; j++)
			{
				EMAAnimation_ByNode_ByTransform &transform = transforms.at(j);		//equivalent of MaterialParameter

				string shadeParamName = getShaderParamNameFromEmmParamName(transform.GetComponents().at(0)->transformNameForMaterialAnim());
				if (shadeParamName.length() == 0)
					continue;

				EmmMaterialParameter emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_VS");
				if (emmMatParam.name.length())
				{
					EANKeyframe kf = transform.getInterpolatedKeyframe((float)time_tmp, ema_Material_Anim2->getValues(), emmMatParam.defaultValue.x, emmMatParam.defaultValue.y, emmMatParam.defaultValue.z, emmMatParam.defaultValue.w);
					Ogre::Vector4 value(kf.x, kf.y, kf.z, kf.w);
					setShaderParameter(ogreMatName, emmMatParam.name, value);
				}

				emmMatParam = getShaderParameter(ogreMatName, shadeParamName + "_PS");
				if (emmMatParam.name.length())
				{
					EANKeyframe kf = transform.getInterpolatedKeyframe((float)time_tmp, ema_Material_Anim2->getValues(), emmMatParam.defaultValue.x, emmMatParam.defaultValue.y, emmMatParam.defaultValue.z, emmMatParam.defaultValue.w);
					Ogre::Vector4 value(kf.x, kf.y, kf.z, kf.w);
					setShaderParameter(ogreMatName, emmMatParam.name, value);
				}
			}
		}
	}

}

void EMMOgre::setZMaskReadWrite_defaultValue_for_transparent(bool enable)
{
	zMaskReadWrite_defaultValue_for_transparent = enable;


	Ogre::MaterialPtr compile_material;
	Ogre::Pass *pass = 0;
	for (size_t i = 0, nb = created_materials.size(); i < nb; i++)
	{
		compile_material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(created_materials.at(i).name, XENOVIEWER_RESOURCE_GROUP);
		if (compile_material.isNull())
			continue;

		pass = compile_material->getTechnique(0)->getPass(0);
		if (!pass->getTransparentSortingEnabled())
			continue;

		string emm_materialName = created_materials.at(i).name.substr(name.length() + 1);				// ogre_material_name = name + "_" + emm_material->getName()

		EMMMaterial* emm_material = getMaterial(emm_materialName);

		EMMParameter* params = emm_material->getParameter("ZWriteMask");
		pass->setDepthWriteEnabled((params) ? (params->uint_value == 1) : zMaskReadWrite_defaultValue_for_transparent);

		params = emm_material->getParameter("ZTestMask");
		pass->setDepthCheckEnabled((params) ? (params->uint_value == 1) : zMaskReadWrite_defaultValue_for_transparent);
	}
	
	
}
