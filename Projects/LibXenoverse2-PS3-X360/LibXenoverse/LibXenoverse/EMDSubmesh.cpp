namespace LibXenoverse
{




/*-------------------------------------------------------------------------------\
|                             EMDSubmesh			                             |
\-------------------------------------------------------------------------------*/
EMDSubmesh::EMDSubmesh(EMDSubmesh* emdSubmesh)
{
	zero();

	if (emdSubmesh)
	{
		name = emdSubmesh->name;
		vertex_type_flag = emdSubmesh->vertex_type_flag;
		vertex_size = emdSubmesh->vertex_size;
		unknow_0 = emdSubmesh->unknow_0;

		aabb_center_x = emdSubmesh->aabb_center_x;
		aabb_center_y = emdSubmesh->aabb_center_y;
		aabb_center_z = emdSubmesh->aabb_center_z;
		aabb_center_w = emdSubmesh->aabb_center_w;
		aabb_min_x = emdSubmesh->aabb_min_x;
		aabb_min_y = emdSubmesh->aabb_min_y;
		aabb_min_z = emdSubmesh->aabb_min_z;
		aabb_min_w = emdSubmesh->aabb_min_w;
		aabb_max_x = emdSubmesh->aabb_max_x;
		aabb_max_y = emdSubmesh->aabb_max_y;
		aabb_max_z = emdSubmesh->aabb_max_z;
		aabb_max_w = emdSubmesh->aabb_max_w;

		for (size_t i = 0, nb = emdSubmesh->vertices.size(); i < nb; i++)
			vertices.push_back(EMDVertex(&emdSubmesh->vertices.at(i)));
		for (size_t i = 0, nb = emdSubmesh->triangles.size(); i < nb; i++)
			triangles.push_back(EMDTriangles(&emdSubmesh->triangles.at(i)));
		for (size_t i = 0, nb = emdSubmesh->definitions.size(); i < nb; i++)
			definitions.push_back(EMDTextureUnitState(&emdSubmesh->definitions.at(i)));
	}
}
/*-------------------------------------------------------------------------------\
|                             zero					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::zero(void)
{
	aabb_center_x = aabb_center_y = aabb_center_z = aabb_center_w = aabb_min_x = aabb_min_y = aabb_min_z = aabb_min_w = aabb_max_x = aabb_max_y = aabb_max_z = aabb_max_w = 0.0f;
	vertex_type_flag = 0;
	vertex_size = 0;
	unknow_0 = 0;
	name = uniqName = "";
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::read(File *file, uint16_t &paddingForCompressedVertex)
{
	unsigned int address = 0;
	unsigned int base_submesh_address = file->getCurrentAddress();

	// Read Submesh
	file->readFloat32E(&aabb_center_x);
	file->readFloat32E(&aabb_center_y);
	file->readFloat32E(&aabb_center_z);
	file->readFloat32E(&aabb_center_w);
	file->readFloat32E(&aabb_min_x);
	file->readFloat32E(&aabb_min_y);
	file->readFloat32E(&aabb_min_z);
	file->readFloat32E(&aabb_min_w);
	file->readFloat32E(&aabb_max_x);
	file->readFloat32E(&aabb_max_y);
	file->readFloat32E(&aabb_max_z);
	file->readFloat32E(&aabb_max_w);

	unsigned int vertex_count = 0;
	unsigned int vertex_address = 0;
	unsigned int submesh_name_address = 0;
	unsigned int submesh_offset_1 = 0;
	unsigned int submesh_offset_2 = 0;
	unsigned char submesh_definition_count = 0;
	unsigned short submesh_triangles_count = 0;
	file->readInt32E(&vertex_type_flag);
	file->readInt32E(&vertex_size);
	file->readInt32E(&vertex_count);
	file->readInt32E(&vertex_address);
	file->readInt32E(&submesh_name_address);
	file->readUChar(&unknow_0);
	file->readUChar(&submesh_definition_count);
	file->readInt16E(&submesh_triangles_count);
	file->readInt32E(&submesh_offset_1);
	file->readInt32E(&submesh_offset_2);




	//Check of vertex definition and size.
	size_t calculatedSize = EMDVertex::getSizeFromFlags(vertex_type_flag);
	if (calculatedSize != vertex_size)
	{
		printf("Strange VertexDefinition Size : Flag %x => calculatedSize: %u octets but have %u in file informations. => Will be skipped.\nPress a key to continue.\n", vertex_type_flag, calculatedSize, vertex_size);
		notifyError();
		return;
	}
	


	file->goToAddress(base_submesh_address + submesh_name_address);
	file->readString(&name);

	// There are invalid material names in some meshes which Dimps left broken in the final release
	// Use a placeholder string instead
	if (!name.size()) name = "null";

	printf("Submesh Name: %s, %d vertex (flag: %s)\n", name.c_str(), vertex_count, EMO_BaseFile::UnsignedToString(vertex_type_flag, true).c_str());

	definitions.resize(submesh_definition_count);
	for (size_t i = 0; i < submesh_definition_count; i++)
	{
		file->goToAddress(base_submesh_address + submesh_offset_1 + i * 12);
		definitions[i].read(file);
	}

	// Read Triangle Lists
	triangles.resize(submesh_triangles_count);
	for (size_t i = 0; i < submesh_triangles_count; i++)
	{
		file->goToAddress(base_submesh_address + submesh_offset_2 + i * 4);

		file->readInt32E(&address);

		if (address)
		{
			file->goToAddress(base_submesh_address + address);
		}else{
			/*
			if (submesh_triangles_count > 1)
			{
				printf("More than 1 submesh triangle list but address is 0? Exception not handled.\n");
				notifyError();
			}
			*/

			file->goToAddress(base_submesh_address + submesh_offset_2 + 4);
		}

		triangles[i].read(file);
	}

	AABB vertex_aabb;

	// Read Vertices
	vertices.resize(vertex_count);
	for (size_t v = 0; v < vertex_count; v++)
	{
		file->goToAddress(base_submesh_address + vertex_address + v*vertex_size);
		vertices[v].read(file, vertex_type_flag, paddingForCompressedVertex);
		vertex_aabb.addPoint(vertices[v].pos_x, vertices[v].pos_y, vertices[v].pos_z);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::write(File *file, uint16_t paddingForCompressedVertex)
{
	unsigned int base_submesh_address = file->getCurrentAddress();

	file->writeFloat32E(&aabb_center_x);
	file->writeFloat32E(&aabb_center_y);
	file->writeFloat32E(&aabb_center_z);
	file->writeFloat32E(&aabb_center_w);
	file->writeFloat32E(&aabb_min_x);
	file->writeFloat32E(&aabb_min_y);
	file->writeFloat32E(&aabb_min_z);
	file->writeFloat32E(&aabb_min_w);
	file->writeFloat32E(&aabb_max_x);
	file->writeFloat32E(&aabb_max_y);
	file->writeFloat32E(&aabb_max_z);
	file->writeFloat32E(&aabb_max_w);

	file->writeNull(32);

	unsigned int submesh_name_address = file->getCurrentAddress() - base_submesh_address;
	file->writeString(&name);
	file->fixPadding(0x4);


	unsigned int submesh_offset_1 = file->getCurrentAddress() - base_submesh_address;
	for (size_t k = 0; k < definitions.size(); k++)
		definitions[k].write(file);

	// Write Triangles
	unsigned int submesh_offset_triangle_table = file->getCurrentAddress() - base_submesh_address;
	file->writeNull(triangles.size() * 4);
	for (size_t k = 0; k < triangles.size(); k++)
	{
		unsigned int triangles_address = file->getCurrentAddress() - base_submesh_address;
		file->goToAddress(base_submesh_address + submesh_offset_triangle_table + k * 4);
		file->writeInt32E(&triangles_address);
		file->goToAddress(base_submesh_address + triangles_address);
		triangles[k].write(file, vertices.size());
	}

	// Write Vertices
	unsigned int vertex_address = file->getCurrentAddress() - base_submesh_address;
	for (size_t v = 0; v < vertices.size(); v++)
		vertices[v].write(file, vertex_type_flag, paddingForCompressedVertex);

	// Write Header
	file->goToAddress(base_submesh_address + 0x30);
	unsigned int vertex_count = vertices.size();
	unsigned char definition_count = definitions.size();
	unsigned short triangles_count = triangles.size();
	file->writeInt32E(&vertex_type_flag);
	file->writeInt32E(&vertex_size);
	file->writeInt32E(&vertex_count);
	file->writeInt32E(&vertex_address);
	file->writeInt32E(&submesh_name_address);
	file->writeUChar(&unknow_0);
	file->writeUChar(&definition_count);
	file->writeInt16E(&triangles_count);
	file->writeInt32E(&submesh_offset_1);
	file->writeInt32E(&submesh_offset_triangle_table);

	file->goToEnd();
}
/*-------------------------------------------------------------------------------\
|                             setVertexScale		                             |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::setVertexScale(float scale)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].pos_x = vertices[i].pos_x * scale;
		vertices[i].pos_y = vertices[i].pos_y * scale;
		vertices[i].pos_z = vertices[i].pos_z * scale;
	}
}
/*-------------------------------------------------------------------------------\
|                             getBonesNames				                         |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::getBonesNames(vector<string> &bones_names)
{
	size_t nbTriangles = triangles.size();
	for (size_t i = 0; i < nbTriangles; i++)
		triangles.at(i).getBonesNames(bones_names);
}
/*-------------------------------------------------------------------------------\
|                             replaceBonesNames						             |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::replaceBonesNames(const string &oldName, const string &newName)
{
	size_t nbTriangles = triangles.size();
	for (size_t i = 0; i < nbTriangles; i++)
		triangles.at(i).replaceBonesNames(oldName, newName);
}
/*-------------------------------------------------------------------------------\
|                             mergeVertex										 |
\-------------------------------------------------------------------------------*/
size_t EMDSubmesh::mergeVertex(EMDVertex &v)
{
	size_t nbVertex = vertices.size();
	for (size_t i = 0; i < nbVertex; i++)
	{
		EMDVertex &v_tmp = vertices.at(i);
		if (v_tmp == v)
			return i;
	}

	vertices.push_back(v);
	return (vertices.size() - 1);
}
/*-------------------------------------------------------------------------------\
|                             mergeTriangles									 |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::mergeTriangles()
{
	vector<EMDTriangles> old_triangles = triangles;
	size_t nbOldTriangles = old_triangles.size();
	if (nbOldTriangles < 2)
		return;

	triangles.clear();
	//triangles.push_back(old_triangles.at(0));
	triangles.push_back(EMDTriangles());
	vector<string> &bone_names = triangles.at(0).bone_names;
	vector<unsigned int> &faces = triangles.at(0).faces;

	size_t nbVertices = vertices.size();
	std::vector<bool> verticeAllreadyModified;
	verticeAllreadyModified.resize(nbVertices, false);

	size_t startBoneNameIndex = 0;
	size_t nbBones = 0;
	size_t nbFaces = 0;
	//for (size_t i = 1; i < nbOldTriangles; i++)
	for (size_t i = 0; i < nbOldTriangles; i++)
	{
		startBoneNameIndex = bone_names.size();
		
		EMDTriangles &triangle = old_triangles.at(i);

		nbBones = triangle.bone_names.size();
		for (size_t j = 0; j < nbBones; j++)
			bone_names.push_back(triangle.bone_names.at(j));							//Todo check if there isn't duplication of boneName (not the case of current test)

		nbFaces = triangle.faces.size();
		for (size_t j = 0; j < nbFaces; j++)
		{
			faces.push_back(triangle.faces.at(j));

			//now we need to update blend indice.
			EMDVertex &vertex = vertices.at(faces.back());
			if (verticeAllreadyModified.at(faces.back()))
				continue;
			verticeAllreadyModified.at(faces.back()) = true;

			if ((vertex.flags & EMD_VTX_FLAG_BLEND_WEIGHT) == 0)
				continue;


			for (size_t k = 0; k < 4; k++)
				vertex.blend[k] += startBoneNameIndex;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             getTriangleForBoneNameCombinaison					 |
\-------------------------------------------------------------------------------*/
EMDTriangles* EMDSubmesh::getTriangleForBoneNameCombinaison(vector<string> bone_names, bool createIfDontExist)
{
	size_t nbBones = bone_names.size();

	if (nbBones == 0)												//case no bone influence :EMD_VTX_FLAG_BLEND_WEIGHT
	{
		for (size_t i = 0; i < triangles.size(); i++)
			if(triangles.at(i).bone_names.size()==0)
				return &triangles.at(i);

		if (createIfDontExist)
		{
			triangles.push_back(EMDTriangles());
			return &triangles.back();
		}else {
			return NULL;
		}
	}

	size_t nbTriangleBones;
	vector<string> bones_to_add;
	for (size_t i = 0; i < triangles.size(); i++)
	{
		vector<string> &triangle_boneNames = triangles.at(i).bone_names;
		nbTriangleBones = triangle_boneNames.size();
		bones_to_add.clear();

		// Detect what bones of the list exist on the triangle's bone names
		for (size_t j = 0; j < nbBones; j++)
		{
			bool found = false;
			for (size_t k = 0; k < nbTriangleBones; k++)
			{
				if (bone_names.at(j) == triangle_boneNames.at(k))
				{
					found = true;
					break;
				}
			}
			if (!found)
				bones_to_add.push_back(bone_names.at(j));
		}

		
		if(bones_to_add.size()==0)
			return &triangles.at(i);						// If some bones were undetected, check if it can be added to the triangle list's bones.
		
		if (createIfDontExist)
		{
			// we try to Add new bones on triangle (to not have too much triangle), in the limite of 24 bones per triangle.
			size_t new_size = triangle_boneNames.size() + bones_to_add.size();
			if (new_size > LIBXENOVERSE_EMD_SUBMESH_BONE_LIMIT)
				continue;

			triangle_boneNames.reserve(new_size);
			triangle_boneNames.insert(triangle_boneNames.end(), bones_to_add.begin(), bones_to_add.end());
			return &triangles.at(i);
		}
	}

	if (createIfDontExist)
	{
		// If no good triangle was found, create a new one
		triangles.push_back(EMDTriangles());
		triangles.back().bone_names = bone_names;
		return &triangles.back();
	}
	return NULL;
}





/*-------------------------------------------------------------------------------\
|                             useTextureDefTemplate								 |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::useTextureDefTemplate(EMDSubmesh* other)
{
	definitions = other->definitions;
}



}