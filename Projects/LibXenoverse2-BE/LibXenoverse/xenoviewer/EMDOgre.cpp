#include "EMDOgre.h"
#include "EMMOgre.h"
#include "EMBOgre.h"
#include "ESKOgre.h"
#include "EMDRenderObjectListener.h"
#include "OgreCommon.h"


/*-------------------------------------------------------------------------------\
|                             EMDOgre				                             |
\-------------------------------------------------------------------------------*/
EMDOgre::EMDOgre(EMD* emd) : EMD(emd)
{
	tag = "EMD";
	mesh_resources_created = false;
	skeleton = NULL;
	material_pack = NULL;
	to_rebuild = false;
	to_delete = false;
	mVisible = true;
	mAllVisible = true;
	fullName = "";
}
/*-------------------------------------------------------------------------------\
|                             EMDOgre				                             |
\-------------------------------------------------------------------------------*/
EMDOgre::~EMDOgre()
{
	destroyResources();
	delete material_pack;
	cleanNodes(true);
}







/*-------------------------------------------------------------------------------\
|                             createOgreSceneNode	                             |
\-------------------------------------------------------------------------------*/
Ogre::SceneNode *EMDOgre::build(Ogre::SceneManager *scene_manager)
{
	Ogre::SceneNode* parent_node = scene_manager->getRootSceneNode()->createChildSceneNode();

	createOgreEMD(parent_node, scene_manager);
	mesh_resources_created = true;
	scene_nodes.push_back(parent_node);

	return parent_node;
}
/*-------------------------------------------------------------------------------\
|                             rebuild				                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::rebuild()
{
	cleanNodes();
	destroyResources();

	for(list<Ogre::SceneNode *>::iterator it = scene_nodes.begin(); it != scene_nodes.end(); it++)
	{
		createOgreEMD(*it, (*it)->getCreator());
		mesh_resources_created = true;
	}

	to_rebuild = false;
}
/*-------------------------------------------------------------------------------\
|                             createOgreSceneNodeEMD                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::createOgreEMD(Ogre::SceneNode *parent, Ogre::SceneManager *scene_manager)
{
	size_t nbModel = models.size();
	for (size_t i = 0; i < models.size(); i++)
		createOgreEmdModel(models.at(i), parent, scene_manager);
}
/*-------------------------------------------------------------------------------\
|                             createOgreSceneNodeModel                           |
\-------------------------------------------------------------------------------*/
Ogre::SceneNode* EMDOgre::createOgreEmdModel(EMDModel *model, Ogre::SceneNode *parent, Ogre::SceneManager *scene_manager)
{
	Ogre::SceneNode* model_node = parent->createChildSceneNode();
	
	model_node->setUseNoUniformScale(true);						//hack done on ogre to be NoScaleUniform (less performances, but could make a arm longer and solve the probelme of a indirect axis.)
	model_node->setScale(-1, 1, 1);								//repere axis left side. => solve the symetry problem //todo add a option in Ui to desactive this for debug of datas.
	


	vector<EMDMesh*> &meshes = model->getMeshes();
	size_t nbMesh = meshes.size();


	if (!mesh_resources_created)														//done once, only on build.
	{
		for (size_t i = 0; i < nbMesh; i++)												//begin by creating one Ogre::Mesh for each submesh (vertex definition oblige to)
		{
			vector<EMDSubmesh *> &submeshes = meshes.at(i)->getSubmeshes();
			size_t nbSubMesh = submeshes.size();
			for (size_t j = 0; j < nbSubMesh; j++)
				createOgreMesh_EmdSubMesh(submeshes.at(j), meshes.at(i)->getName(), model_node);	//future reference by using the name witch is a conbinaison of mesh name and submesh name
		}
	}


	for (size_t i = 0; i < nbMesh; i++)												//and after create the instance of meshs , a Entity
	{
		vector<EMDSubmesh *> &submeshes = meshes.at(i)->getSubmeshes();
		size_t nbSubMesh = submeshes.size();
		for (size_t j = 0; j < nbSubMesh; j++)
			createOgreEntity_EmdSubMesh(submeshes.at(j), meshes.at(i)->getName(), model_node, scene_manager, model->getName());	//future reference by using the name witch is a conbinaison of mesh name and submesh name
	}
	return model_node;
}
/*-------------------------------------------------------------------------------\
|                             createOgreMesh_EmdSubMesh                          |
\-------------------------------------------------------------------------------*/
void EMDOgre::createOgreMesh_EmdSubMesh(EMDSubmesh* submesh, string mesh_name, Ogre::SceneNode* model_node)
{
	vector<EMDVertex> &submesh_vertices = submesh->getVertices();
	size_t nVertices = submesh_vertices.size();
	if (nVertices == 0)
	{
		Ogre::LogManager::getSingleton().logMessage("Error: EmdSubmesh " + submesh->getMaterialName() + " have 0 vertice. skipped.");
		return;
	}
	

	string ogre_mesh_name = name + "_" + mesh_name + "_" + submesh->getMaterialName();
	if (!Ogre::MeshManager::getSingleton().getByName(ogre_mesh_name).isNull())
	{
		size_t inc = 1;
		while ((!Ogre::MeshManager::getSingleton().getByName(ogre_mesh_name).isNull()) && (inc<1000))
			ogre_mesh_name = mesh_name + "_" + submesh->getMaterialName() + "__" + Ogre::StringConverter::toString(inc++);

		if (inc >= 1000)
		{
			Ogre::LogManager::getSingleton().logMessage("Warning: " + ogre_mesh_name + " allready present (1000 instances). skipped.");
			return;
		}
	}
	Ogre::MeshPtr ogre_mesh = Ogre::MeshManager::getSingleton().createManual(ogre_mesh_name, XENOVIEWER_RESOURCE_GROUP);
	submesh->uniqName = ogre_mesh_name;
	


	//------- Create Vertex Pool
	LibXenoverse::AABB mesh_aabb;
	mesh_aabb.reset();


	// Create Shared Vertex Data for all submeshes
	Ogre::VertexData* vertex_data = new Ogre::VertexData();
	ogre_mesh->sharedVertexData = vertex_data;
	vertex_data->vertexCount = nVertices;
	size_t flags = submesh->getVertexTypeFlags();

	// declarations 
	Ogre::VertexDeclaration* decl = vertex_data->vertexDeclaration;
	size_t offset = 0;

	if (flags & EMD_VTX_FLAG_POS)
	{
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	}

	if (flags & EMD_VTX_FLAG_NORM)
	{
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	}

	if (flags & EMD_VTX_FLAG_TANGENT)
	{
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_TANGENT);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	}

	if (flags & EMD_VTX_FLAG_TEX)
	{
		decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	}

	if (flags & EMD_VTX_FLAG_TEX2)
	{
		decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	}

	if (flags & EMD_VTX_FLAG_COLOR)
	{
		decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
	}
	
	//(flags & EMD_VTX_FLAG_BLEND_WEIGHT), Notice BlendIndice and BlendWieght come in a second time with a Ogre::VertexBoneAssignment. 
	




	Ogre::Matrix4 mat = Ogre::Matrix4::IDENTITY;
	

	// fill datas of vertices
	size_t vertexSize = offset/4;				// one float = 4 octets = 32 bits
	size_t index = 0;
	const size_t vbufCount = vertexSize * nVertices;
	float *vertices = (float *)malloc(sizeof(float)*vbufCount);
	for (size_t i = 0; i < nVertices; i++)
	{
		EMDVertex &v = submesh_vertices.at(i);
		

		index = i*vertexSize;

		Ogre::Vector3 pos(v.pos_x, v.pos_y, v.pos_z);
		pos = mat * pos;

		if (flags & EMD_VTX_FLAG_POS)
		{
			vertices[index    ] = pos.x;
			vertices[index + 1] = pos.y;
			vertices[index + 2] = pos.z;
			index += 3;
		}

		if (flags & EMD_VTX_FLAG_NORM)
		{
			Ogre::Vector3 norm(v.norm_x, v.norm_y, v.norm_z);
			norm = (mat * norm) - (mat * Ogre::Vector3::ZERO);
			
			vertices[index	  ] = norm.x;
			vertices[index + 1] = norm.y;
			vertices[index + 2] = norm.z;
			index += 3;
		}

		if (flags & EMD_VTX_FLAG_TANGENT)
		{
			Ogre::Vector3 tang(v.tang_x, v.tang_y, v.tang_z);
			tang = (mat * tang) - (mat * Ogre::Vector3::ZERO);
			
			vertices[index	  ] = tang.x;
			vertices[index + 1] = tang.y;
			vertices[index + 2] = tang.z;
			index += 3;
		}

		if (flags & EMD_VTX_FLAG_TEX)
		{
			vertices[index	  ] = v.text_u;
			vertices[index + 1] = v.text_v;
			index += 2;
		}

		if (flags & EMD_VTX_FLAG_TEX2)
		{
			vertices[index	  ] = v.text2_u;
			vertices[index + 1] = v.text2_v;
			index += 2;
		}
		
		if (flags & EMD_VTX_FLAG_COLOR)
		{
			vertices[index] = *((float*)&v.color);			//must be uint32, but is the same size, bytes we be copy normally.
			
			//uint32_t argb = ((v.color >> 8) & 0xFFFFFF ) + ((v.color & 0xFF) << 24) ;			// RGBA -> ARGB
			//vertices[index] = *((float*)&argb);			//must be uint32, but is the same size, bytes we be copy normally.

			//uint32_t abgr = ((v.color & 0xFF) << 24) + ((v.color & 0xFF00) << 8) + ((v.color & 0xFF0000) >> 8) + ((v.color & 0xFF000000) >> 24);			// RGBA -> ABGR
			//vertices[index] = *((float*)&abgr);			//must be uint32, but is the same size, bytes we be copy normally.


			index += 1;
		}

		mesh_aabb.addPoint(pos.x, pos.y, pos.z);
	}


	// create the hardwareBuffer for vertices.
	Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offset, nVertices, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
	Ogre::VertexBufferBinding* bind = vertex_data->vertexBufferBinding;
	bind->setBinding(0, vbuf);


	




	// Create Submeshes for each Triangle List
	vector<EMDTriangles> submesh_triangles = submesh->getTriangles();
	for (size_t i = 0; i < submesh_triangles.size(); i++)
		createOgreIndexBuffer_EmdSubMesh(&submesh_triangles.at(i), ogre_mesh, nVertices);







	// Create Bone Assignments if Skeleton name exists
	if((flags & EMD_VTX_FLAG_BLEND_WEIGHT) && (skeleton) && (skeleton->getOgreSkeleton()))
	{
		ogre_mesh->setSkeletonName(skeleton->getName());
		Ogre::Skeleton* ogre_skeleton = skeleton->getOgreSkeleton();
		

		EMDTriangles* triangles;
		size_t face_count, nbVertexIndices, nbBones;
		vector<unsigned int> vertex_indices;
		vector<unsigned short> bone_table;
		string bone_name;

		size_t nbTriangles = submesh_triangles.size();
		for (size_t i = 0; i < nbTriangles; i++)
		{
			triangles = &submesh_triangles.at(i);
			

			// Make a list of all vertex indices being used in the submesh
			vertex_indices.clear();
			face_count = triangles->faces.size();
			for (size_t j = 0; j < face_count; j++)
			{
				bool found = false;
				nbVertexIndices = vertex_indices.size();
				for (size_t k = 0; k < nbVertexIndices; k++)
				{
					if (vertex_indices.at(k) == triangles->faces.at(j))
					{
						found = true;
						break;
					}
				}
				if (!found)
					vertex_indices.push_back(triangles->faces.at(j));
			}


			// Build Bone Mapping Table
			bone_table.clear();
			nbBones = triangles->bone_names.size();
			bone_table.resize(nbBones, (unsigned short)-1);

			for (size_t j = 0; j < nbBones; j++)
			{
				bone_name = triangles->bone_names.at(j);

				if (ogre_skeleton->hasBone(bone_name))
				{
					Ogre::Bone* mBone = ogre_skeleton->getBone(bone_name);
					bone_table.at(j) = mBone->getHandle();
				}else {
					printf("Couldn't find %s in ogre skeleton!\n", bone_name.c_str());
					notifyError();
				}
			}

			

			// Add bone assignments to all the vertices that were found
			nbVertexIndices = vertex_indices.size();
			for (size_t j = 0; j < nbVertexIndices; j++)
			{
				Ogre::VertexBoneAssignment vba;
				vba.vertexIndex = vertex_indices.at(j);
				EMDVertex &vertex = submesh_vertices.at(vba.vertexIndex);



				for (size_t k = 0; k < 4; k++)
				{
					unsigned char bone_index = vertex.blend[k];

					//hack to have the sum to 1.0
					float bone_weight = ((k != 3) ? vertex.blend_weight[k] : (1.0 - (vertex.blend_weight[0] + vertex.blend_weight[1] + vertex.blend_weight[2])));					

					if (bone_index >= bone_table.size())
					{
						bone_index = 0;
						bone_weight = 0.0f;
					}


					//if (bone_weight > 0.0f)
					{
						vba.boneIndex = bone_table.at(bone_index);
						vba.weight = bone_weight;
						ogre_mesh->addBoneAssignment(vba);
					}
				}
			}
		}

		// Apply changes, build the buffer
		ogre_mesh->_compileBoneAssignments();
		ogre_mesh->sharedVertexData->reorganiseBuffers(decl->getAutoOrganisedDeclaration(true, false));
	}

	//ogre_mesh->_setBounds(Ogre::AxisAlignedBox(mesh_aabb.start_x, mesh_aabb.start_y, mesh_aabb.start_z, mesh_aabb.end_x, mesh_aabb.end_y, mesh_aabb.end_z)); //not working well because it forget about animation states.
	ogre_mesh->_setBounds(Ogre::AxisAlignedBox(-10000, -10000, -10000, 10000, 10000, 10000));
	ogre_mesh->_setBoundingSphereRadius(mesh_aabb.sizeMax());
	ogre_mesh->load();

	free(vertices);

	created_meshes.push_back(ogre_mesh_name);
}
/*-------------------------------------------------------------------------------\
|                             createOgreIndexBuffer_EmdSubMesh                   |
\-------------------------------------------------------------------------------*/
Ogre::SubMesh* EMDOgre::createOgreIndexBuffer_EmdSubMesh(EMDTriangles* triangles, Ogre::MeshPtr mesh, size_t nVertices)
{
	Ogre::SubMesh *sub = mesh->createSubMesh();

	if (nVertices < 65535)
	{
		size_t ibufCount = triangles->faces.size();
		unsigned short* faces = (unsigned short*)malloc(sizeof(unsigned short) * ibufCount);
		for (size_t i = 0; i < ibufCount; i++)
			faces[i] = triangles->faces.at(i);

		Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, ibufCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
		sub->useSharedVertices = true;
		sub->indexData->indexBuffer = ibuf;
		sub->indexData->indexCount = ibufCount;
		sub->indexData->indexStart = 0;
		::free(faces);

	}else {
		size_t ibufCount = triangles->faces.size();
		unsigned int* faces = (unsigned int*)malloc(sizeof(unsigned int) * ibufCount);
		for (size_t i = 0; i < ibufCount; i++)
			faces[i] = triangles->faces.at(i);

		Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT, ibufCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
		sub->useSharedVertices = true;
		sub->indexData->indexBuffer = ibuf;
		sub->indexData->indexCount = ibufCount;
		sub->indexData->indexStart = 0;
		::free(faces);
	}
	return sub;
}
/*-------------------------------------------------------------------------------\
|                             createOgreEntity_EmdSubMesh	                     |
\-------------------------------------------------------------------------------*/
void EMDOgre::createOgreEntity_EmdSubMesh(EMDSubmesh* submesh, string mesh_name, Ogre::SceneNode* model_node, Ogre::SceneManager *scene_manager, string modelName)
{
	Ogre::MaterialManager* matMgr = Ogre::MaterialManager::getSingletonPtr();
		
	string meshName = submesh->uniqName;
	string materialName = name + "_" + submesh->getMaterialName();
	Ogre::StringUtil::toLowerCase(materialName);

	if (Ogre::MeshManager::getSingleton().getByName(meshName).isNull())
		return;

	string instanceName = meshName;
	if (scene_manager->hasSceneNode(instanceName))
	{
		size_t inc = 1;
		while ((scene_manager->hasSceneNode(instanceName)) && (inc<1000))
			instanceName = meshName +"__"+ Ogre::StringConverter::toString(inc++);

		if (inc >= 1000)
		{
			Ogre::LogManager::getSingleton().logMessage("Warning: "+ instanceName +" allready present (1000 instances). skipped.");
			return;
		}
	}

	Ogre::SceneNode* node =  model_node->createChildSceneNode(instanceName);

	Ogre::Entity* entity = scene_manager->createEntity(instanceName, meshName);
	node->attachObject(entity);

	entity->setCastShadows(false);


	
	if (((submesh->getVertexTypeFlags() & EMD_VTX_FLAG_BLEND_WEIGHT) == 0) && (skeleton) && (skeleton->getOgreSkeleton()) && (submesh->getTriangles().size() != 0))			//if there isn't skining, and after emd link to esk.
	{
		LibXenoverse::ESKBone* boneToForceLink = 0;

		std::vector<LibXenoverse::ESKBone*> &bones = skeleton->getBones();
		LibXenoverse::ESKBone* bone = 0;
		size_t nbBones = bones.size();
		for (size_t i = 0; i < nbBones; i++)
		{
			if ((bones.at(i)->getName() == modelName))		//a bone with the same name of mesh, is for NodeAnim		//todo recheck.
			{
				boneToForceLink = bones.at(i);
				break;
			}
		}

		if (boneToForceLink)
		{
			Ogre::SkeletonInstance* skInst =  skeleton->getEntity()->getSkeleton();

			//we must init Node as the bone transform , else, the binding of the skinning willl be wrong
			//Ogre::Bone* ogreBone = skeleton->getOgreSkeleton()->getBone(boneToForceLink->getName());
			Ogre::Bone* ogreBone = skInst->getBone(boneToForceLink->getName());
			
			ogreBone->_update(false, true);

			node->setPosition(ogreBone->convertLocalToWorldPosition(Ogre::Vector3::ZERO));
			node->setOrientation(ogreBone->_getFullTransform().extractQuaternion());

			mListLinkBones.push_back(LinkBetweenBoneAndNode(node, ogreBone));
		}
	}




	// Share Skeleton Instances with ESK's fake entity
	if ((submesh->getVertexTypeFlags() & EMD_VTX_FLAG_BLEND_WEIGHT) && (skeleton) && (entity->hasSkeleton()))
	{
		entity->shareSkeletonInstanceWith(skeleton->getEntity());

		//hack for scd : separate initial position for skinning and initial position for animation (because of relative information from animation)
		skeleton->applyBinding(entity->getSkeleton());
	}
	
	

	Ogre::MaterialPtr mat = (Ogre::MaterialPtr)matMgr->getByName(materialName);
	if (mat.isNull())
	{
		Ogre::LogManager::getSingleton().logMessage("Warning: Material " + (materialName)+" was not found. Using Default White material.");
		mat = (Ogre::MaterialPtr)matMgr->getByName("White");
	}
	entity->setMaterialName(mat->getName());


	/*
	// Hack : push Additive alphaBlending to a next renderingOrder, to solve problem with transparent alpha.
	{
		Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
		if ((pass->getSourceBlendFactor()==Ogre::SBF_ONE)&&(pass->getDestBlendFactor() == Ogre::SBF_ONE) && (pass->getSourceBlendFactorAlpha() == Ogre::SBF_ONE) && (pass->getDestBlendFactorAlpha() == Ogre::SBF_ZERO))	//additive
			entity->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_WORLD_GEOMETRY_2);
	}
	*/

	EMMOgre::EmmMaterialCreated* matCrea = 0;
	EMMMaterial* emm_material = 0;
	if (material_pack)
	{
		matCrea = material_pack->getEmmMaterialCreated(mat->getName());
		if (matCrea)
		{
			emm_material = matCrea->emmMaterialOrigine;
			
			EMMParameter* params = emm_material->getParameter("Billboard");
			if ((params) && (params->uint_value == 1))
			{
				node->setAutoTracking(true, scene_manager->getSceneNode("PlayerCam"), Ogre::Vector3::UNIT_Z);

				params = emm_material->getParameter("BillboardType");
				if ((params) && (params->uint_value == 0))
					node->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
				else if ((params) && (params->uint_value == 1))							//if 1 use axis for rotation
					node->setFixedYawAxis_b(true, Ogre::Vector3::UNIT_Y);
			}
		}
	}
	





	//add texture on material by reading texturesDefinitions of EmdSubmesh
	{
		Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);

		if (pass->isTransparent())
		{
			entity->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_6);

			//static Ogre::ushort priority = (Ogre::ushort)-1;
			//entity->setRenderQueueGroupAndPriority(Ogre::RenderQueueGroupID::RENDER_QUEUE_6, priority--);	//test transparent after the rest, because of smokes on bFcel01. Todo remove or check every case
		}

		// Create Render Object Listeners depending on submesh definitions
		{
			vector<Ogre::TexturePtr> textures;
			EMBOgre* texture_pack = (material_pack) ? material_pack->getTexturePack() : 0;
			if(texture_pack)
				textures = texture_pack->getOgreTextures();

			/*
			vector<Ogre::TexturePtr> player_dyt;
			EMBOgre* texture_player_dyt_pack = (material_pack) ? material_pack->getPlayerDytTexturePack() : 0;
			if(texture_player_dyt_pack)
				player_dyt = texture_player_dyt_pack->getOgreTextures();
			*/

			//each defintion is about a sampler ImageSampler0, ImageSampler1, .... 
			vector<EMDTextureUnitState> &definitions = submesh->getDefinitions();
			size_t nbSubMeshDefinition = definitions.size();
			if (nbSubMeshDefinition > pass->getNumTextureUnitStates())
				nbSubMeshDefinition = pass->getNumTextureUnitStates();

			vector<Ogre::Real> listTileFloats;

			vector<Ogre::TexturePtr>* textures_list = 0;

			for (size_t k = 0; k < nbSubMeshDefinition; k++)
			{
				EMDTextureUnitState &tus = definitions.at(k);

				unsigned short texIndex = tus.texIndex;

				listTileFloats.push_back(tus.textScale_u);			//texture scale will be merged into few shader parameters, so we have to compile them.
				listTileFloats.push_back(tus.textScale_v);

				textures_list = &textures;


				// Adress Mode.
				Ogre::TextureUnitState::TextureAddressingMode tam_u = Ogre::TextureUnitState::TAM_WRAP;
				if(tus.adressMode_u==EMD_TUS_ADRESSMODE_MIRROR)
					tam_u = Ogre::TextureUnitState::TAM_MIRROR;
				else if (tus.adressMode_u == EMD_TUS_ADRESSMODE_CLAMP)
					tam_u = Ogre::TextureUnitState::TAM_CLAMP;

				Ogre::TextureUnitState::TextureAddressingMode tam_v = Ogre::TextureUnitState::TAM_WRAP;
				if (tus.adressMode_v == EMD_TUS_ADRESSMODE_MIRROR)
					tam_v = Ogre::TextureUnitState::TAM_MIRROR;
				else if (tus.adressMode_v == EMD_TUS_ADRESSMODE_CLAMP)
					tam_v = Ogre::TextureUnitState::TAM_CLAMP;

				pass->getTextureUnitState(k)->setTextureAddressingMode(tam_u, tam_v, Ogre::TextureUnitState::TAM_WRAP);

				
				//filtering.
				Ogre::FilterOptions filter_min = Ogre::FilterOptions::FO_NONE;
				if (tus.filtering_minification == EMD_TUS_FILTERING_POINT)
					filter_min = Ogre::FilterOptions::FO_POINT;
				else if (tus.filtering_minification == EMD_TUS_FILTERING_LINEAR)
					filter_min = Ogre::FilterOptions::FO_LINEAR;

				Ogre::FilterOptions filter_magn = Ogre::FilterOptions::FO_NONE;
				if (tus.filtering_magnification == EMD_TUS_FILTERING_POINT)
					filter_magn = Ogre::FilterOptions::FO_POINT;
				else if (tus.filtering_magnification == EMD_TUS_FILTERING_LINEAR)
					filter_magn = Ogre::FilterOptions::FO_LINEAR;

				pass->getTextureUnitState(k)->setTextureFiltering(filter_min, filter_magn, Ogre::FilterOptions::FO_NONE);




				if (emm_material)
				{
					EMMParameter* params = emm_material->getParameter("MipMapLod"+ Ogre::StringConverter::toString(k));
					if (params)
						pass->getTextureUnitState(k)->setTextureMipmapBias(params->float_value);
				}

				if ((!textures_list)||(textures_list->size() == 0))
					continue;

				if (texIndex >= textures_list->size())
					texIndex = textures_list->size() - 1;

				Ogre::TexturePtr tex_tmp = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(textures_list->at(texIndex)->getName());
				if (tex_tmp.isNull())
					continue;

				pass->getTextureUnitState(k)->setTextureName(textures_list->at(texIndex)->getName());
				
				


				//todoo look after unknow next to textureIndex for 
				//pass->getTextureUnitState(k)->setNumMipmaps(0);
				//pass->getTextureUnitState(k)->setTextureAnisotropy(1.0);
				//pass->getTextureUnitState(k)->setTextureFiltering(Ogre::FilterOptions::FO_NONE, Ogre::FilterOptions::FO_NONE, Ogre::FilterOptions::FO_NONE);

				
				/*
				//Here a new problem : when you use shaders, you have to deal with textureScale into shader. but the Dbx2 shaders don't do this.
				//so changing TextureScale of TextureUnitStates don't have any influence.
				//And as the goal is to not modify shader by hands, We need to create a new texture with the repetition. it's not a good hack. 
				// Todo do better, or try to understand how the game do.
				if ( (false) && ((definitions.at(k).textScale_u != 1.0) || (definitions.at(k).textScale_v  != 1.0)) )
				{
					string targetTextTile = Ogre::StringConverter::toString(texIndex + 1);
					if (targetTextTile.length() == 1)
						targetTextTile = "0" + targetTextTile;

					
					//test if shader have entry for that.
					string indexStr = std::to_string(texIndex + 1);
					if (indexStr.length() == 1)
						indexStr = "0" + indexStr;
					bool allreadyHaveShaderEntry = material_pack->setShaderParameter(mat->getName(), "g_vTexTile"+ indexStr + "_VS", Ogre::Vector4(definitions.at(k).textScale_u, definitions.at(k).textScale_v, 1.0, 1.0));
					allreadyHaveShaderEntry = ((allreadyHaveShaderEntry) || (material_pack->setShaderParameter(mat->getName(), "g_vTexTile" + indexStr + "_PS", Ogre::Vector4(definitions.at(k).textScale_u, definitions.at(k).textScale_v, 1.0, 1.0)) ));

					
					
					Ogre::TexturePtr text = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(name + "_" + Ogre::StringConverter::toString(texIndex));
					if ((!allreadyHaveShaderEntry) && (!text.isNull()))				//if allready used by shader entry , we didn't need to create a new texture for repetitions.
					{
						Ogre::PixelFormat pf = text->getFormat();

						Ogre::Vector2 resolution( Ogre::Math::Floor(text->getWidth() * definitions.at(k).textScale_u), Ogre::Math::Floor(text->getHeight() * definitions.at(k).textScale_v));
						if (resolution.x > 8192)				//security about old GPU.
							resolution.x = 8192;
						if (resolution.y > 8192) 
							resolution.y = 8192;

						Ogre::Vector2 nbRepete = resolution / Ogre::Vector2(text->getWidth(), text->getHeight());

						string nexNameRepete = name + "_" + Ogre::StringConverter::toString(texIndex) + "__" + Ogre::StringConverter::toString((int)resolution.x) +"x"+ Ogre::StringConverter::toString((int)resolution.y);
						Ogre::TexturePtr textRepete = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(nexNameRepete);
						if (textRepete.isNull())
						{
							textRepete = Ogre::TextureManager::getSingleton().createManual(nexNameRepete, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, resolution.x, resolution.y, 0, text->getFormat(), Ogre::TU_DEFAULT);

							Ogre::HardwarePixelBufferSharedPtr hpbs = text->getBuffer();
							Ogre::HardwarePixelBufferSharedPtr hpbsRepete = textRepete->getBuffer();
							Ogre::PixelBox pixelBox;
							Ogre::PixelBox pixelBoxRepete;

							size_t start_X, start_Y, size_X, size_Y;
							size_t nbRepete_X = Ogre::Math::Ceil(nbRepete.x);
							size_t nbRepete_Y = Ogre::Math::Ceil(nbRepete.y);
							for (size_t j = 0; j < nbRepete_Y; j++)
							{
								start_Y = j * text->getHeight();
								size_Y = ((j + 1 < nbRepete.y) ? 1.0 : (nbRepete.y - j)) * text->getHeight();
								
								for (size_t i = 0; i < nbRepete_X; i++)
								{
									start_X = i * text->getWidth();
									size_X = ((i + 1 < nbRepete.x) ? 1.0 : (nbRepete.x - i)) * text->getWidth();

									
									pixelBox = hpbs->lock(Ogre::Image::Box(0, 0, size_X, size_Y), Ogre::HardwareBuffer::HBL_READ_ONLY);
									pixelBoxRepete = hpbsRepete->lock(Ogre::Image::Box(start_X, start_Y, start_X + size_X, start_Y + size_Y), Ogre::HardwareBuffer::HBL_NORMAL);
									
									Ogre::PixelUtil::bulkPixelConversion(pixelBox, pixelBoxRepete);

									hpbs->unlock();
									hpbsRepete->unlock();
								}
							}
						}

						pass->getTextureUnitState(k)->setTextureName(textRepete->getName());

					}else {
						pass->getTextureUnitState(k)->setTextureName(name + "_" + Ogre::StringConverter::toString(texIndex));
					}
				}else {
					pass->getTextureUnitState(k)->setTextureName(name + "_" + Ogre::StringConverter::toString(texIndex));
				}
				*/

				
				
				/*
				//test for stage textures, witch have trouble with filtering, aliasing or mipmapping.
				pass->getTextureUnitState(k)->setNumMipmaps(0);
				pass->getTextureUnitState(k)->setTextureAnisotropy(1.0);
				pass->getTextureUnitState(k)->setTextureFiltering(Ogre::FilterOptions::FO_NONE, Ogre::FilterOptions::FO_NONE, Ogre::FilterOptions::FO_NONE);
				*/

				//pass->getTextureUnitState(k)->setTextureName(name + "_" + Ogre::StringConverter::toString(texIndex));			//test bug scouter before skin.
			}



			//here, we try a new solution , fill first find g_vTexTileXX_VS (or PS) to put floats of texture_scale_u and v at follow. and the next after.
			size_t nbFloats = listTileFloats.size();
			if ((nbFloats != 0)&&(material_pack)&&(matCrea))
			{
				size_t inc = 0;
				std::vector<EMMOgre::EmmMaterialParameter> &parameter = matCrea->parameter;
				size_t nbParam = parameter.size();
				for (size_t i = 0; i < nbParam; i++)
				{
					if ((parameter.at(i).name.length() > 10) && (parameter.at(i).name.substr(0, 10) == "g_vTexTile"))
					{
						Ogre::Vector4 vector_tmp(1, 1, 1, 1);

						vector_tmp.x = listTileFloats.at(inc++);
						vector_tmp.y = listTileFloats.at(inc++);

						if (inc < nbFloats)
						{
							vector_tmp.z = listTileFloats.at(inc++);
							vector_tmp.w = listTileFloats.at(inc++);
						}

						material_pack->setShaderParameter(mat->getName(), parameter.at(i).name, vector_tmp);
						if (inc >= nbFloats)
							break;
					}
					
				}
			}
		}
	}
}








/*-------------------------------------------------------------------------------\
|                             cleanNodes			                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::cleanNodes(bool parent)
{
	
	for (list<EMDRenderObject *>::iterator it = created_render_objects.begin(); it != created_render_objects.end(); it++)
		delete *it;

	mListLinkBones.clear();
	created_render_objects.clear();

	for (list<Ogre::SceneNode *>::iterator it = scene_nodes.begin(); it != scene_nodes.end(); it++)
	{
		OgreUtil::destroySceneNodeChildren(*it);

		if (parent)
			(*it)->getCreator()->destroySceneNode((*it));
	}
}
/*-------------------------------------------------------------------------------\
|                             destroyResources		                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::destroyResources()
{
	if (mesh_resources_created)
	{
		for (list<Ogre::String>::iterator it = created_meshes.begin(); it != created_meshes.end(); it++)
			Ogre::MeshManager::getSingleton().remove(*it);
	}

	mesh_resources_created = false;
}


/*-------------------------------------------------------------------------------\
|                             updateBonesLinks		                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::updateBonesLinks()
{
	Ogre::SceneNode* node;
	Ogre::Bone* bone;
	for (size_t i = 0, nb = mListLinkBones.size(); i < nb; i++)
	{
		bone = mListLinkBones.at(i).bone;
		node = mListLinkBones.at(i).node;

		bone->_update(false, true);

		node->setPosition(bone->convertLocalToWorldPosition(Ogre::Vector3::ZERO));
		node->setOrientation(bone->_getFullTransform().extractQuaternion());
	}
}





/*-------------------------------------------------------------------------------\
|                             setVisible			                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::setVisible(bool enable)
{
	if ((mAllVisible)&&(enable != mVisible))
	{
		for (list<Ogre::SceneNode *>::iterator it = scene_nodes.begin(); it != scene_nodes.end(); it++)
			(*it)->setVisible(enable);
	}
	mVisible = enable;
}
/*-------------------------------------------------------------------------------\
|                             setVisible			                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::setVisible(string materialName, bool enable)
{
	if (scene_nodes.size() == 0)
		return;
	Ogre::SceneManager* scene_manager = scene_nodes.back()->getCreator();
	
	
	size_t nbMesh, nbSubMesh;
	size_t nbModel = models.size();
	for (size_t i = 0; i < models.size(); i++)
	{
		vector<EMDMesh*> &meshes = models.at(i)->getMeshes();
		nbMesh = meshes.size();
		for (size_t j = 0; j < nbMesh; j++)
		{
			vector<EMDSubmesh *> &submeshes = meshes.at(j)->getSubmeshes();
			nbSubMesh = submeshes.size();
			for (size_t k = 0; k < nbSubMesh; k++)
			{
				if (submeshes.at(k)->getMaterialName() != materialName)
					continue;

				string meshName = submeshes.at(k)->uniqName;
				
				Ogre::SceneNode* node = scene_manager->getSceneNode(meshName);
				node->setVisible(enable);
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             setAllVisible			                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::setAllVisible(bool enable)
{
	if (mAllVisible != enable)
	{
		for (list<Ogre::SceneNode *>::iterator it = scene_nodes.begin(); it != scene_nodes.end(); it++)
			(*it)->setVisible(enable && mVisible);
	}
	mAllVisible = enable;
}
/*-------------------------------------------------------------------------------\
|                             setMaterialName		                             |
\-------------------------------------------------------------------------------*/
void EMDOgre::setMaterialName(const Ogre::String &materialName, Ogre::SceneNode *node)
{
	if (!node)
	{
		for (list<Ogre::SceneNode *>::iterator it = scene_nodes.begin(); it != scene_nodes.end(); it++)
			setMaterialName(materialName, (*it));

	}else{

		size_t nbObj = node->numAttachedObjects();
		for (size_t i = 0; i < nbObj; i++)
			((Ogre::Entity*)(node->getAttachedObject(i)))->setMaterialName(materialName);

		nbObj = node->numChildren();
		for (size_t i = 0; i < nbObj; i++)
			setMaterialName(materialName, (Ogre::SceneNode*)node->getChild(i));
	}
}
/*-------------------------------------------------------------------------------\
|                             setMaterialParameter		                         |
\-------------------------------------------------------------------------------*/
void EMDOgre::setMaterialParameter(const Ogre::String &paramName, Ogre::Vector4 value)
{
	if (material_pack)
		material_pack->setShaderParameter(paramName, value);
}




