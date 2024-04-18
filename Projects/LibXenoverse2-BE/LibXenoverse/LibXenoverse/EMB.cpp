namespace LibXenoverse {

	bool EMB::load(string filename) {
		name = nameFromFilenameNoExtension(filename, true);

		File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
		if ((!file.valid()) || (!file.readHeader(LIBXENOVERSE_EMB_SIGNATURE))) 
			return false;
			
		file.goToAddress(0x6);

		uint16_t headerSize = 0;
		file.readInt16E(&headerSize);
		
		version = "";
		uint8_t tmp = 0;
		file.readUChar(&tmp);
		version += ToString((uint32_t)tmp) + ".";
		file.readUChar(&tmp);
		version += ToString((uint32_t)tmp) + ".";
		file.readUChar(&tmp);
		version += ToString((uint32_t)tmp) + ".";
		file.readUChar(&tmp);
		version += ToString((uint32_t)tmp);


		uint32_t file_count = 0;
		file.readInt32E(&file_count);

		file.goToAddress(0x18);						//that stange but in two case, it's the same place, and also the theorical header size to add on futur offsets are 0x20, independantly of the real header size.
		uint32_t wrong_data_table_address = 0x20;

		uint32_t data_table_address = 0;
		file.readInt32E(&data_table_address);
		uint32_t filename_table_address = 0;
		file.readInt32E(&filename_table_address);

			

		printf("Found %d files\n", file_count);

		for (size_t i = 0; i < file_count; i++)
		{
			file.goToAddress(data_table_address + i * 8);
			uint32_t data_address = 0;
			file.readInt32E(&data_address);
			uint32_t data_size = 0;
			file.readInt32E(&data_size);


			char *data = (char *) malloc(data_size);
			file.goToAddress(wrong_data_table_address + i * 8 + data_address);
			file.read(data, data_size);

			EMBFile *file_entry = new EMBFile(data, data_size);
			file_entry->setIndex(i);
			files.push_back(file_entry);

			printf("File Entry %d with size %d\n", i, data_size);
		}

		debugHaveNames = (filename_table_address != 0);


		if (filename_table_address) 
		{
			for (size_t i = 0; i < file_count; i++) 
			{
				file.goToAddress(filename_table_address + i*4);
				unsigned int string_address = 0;
				file.readInt32E(&string_address);
				file.goToAddress(string_address);

				string filename_entry = "";
				file.readString(&filename_entry);

				printf("File Entry %d with name %s\n", i, filename_entry.c_str());

				if (files.size() > i)
					files[i]->setName(filename_entry);
			}
		}

		file.close();

		return true;
	}




	void EMB::save(string filename, bool enable_filenames, bool big_endian)
	{
		File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
		if (!file.valid())
			return;

		file.writeHeader(LIBXENOVERSE_EMB_SIGNATURE, big_endian);

		file.goToAddress(0x6);


		uint32_t file_total = files.size();

		// Header
		uint16_t headerSize = 0x20;
		file.writeInt16E(&headerSize);

		if (version.length() == 0)
			version = "0";
		std::vector<string> sv = split(version, '.');
		for (size_t i = 0; i < 4; i++)
		{
			if (i < sv.size())
			{
				uint8_t tmp = std::stoi(sv.at(i));
				file.writeUChar(&tmp);
			}else {
				file.writeNull(1);
			}
		}

		file.writeInt32E(&file_total);
		file.writeNull(8);

		// Main Data
		file.writeNull(2 * sizeof(uint32_t));

		uint32_t data_table_address = file.getCurrentAddress();
		file.writeNull(file_total * 2 * sizeof(uint32_t));

		uint32_t string_table_address = 0;
		if (enable_filenames) {
			string_table_address = file.getCurrentAddress();
			file.writeNull(file_total * sizeof(uint32_t));
		}

		for (size_t i = 0; i < file_total; i++)
		{
			file.fixPadding(0x40);
			uint32_t file_data_address = file.getCurrentAddress() - i * 8 - 32;
			uint32_t file_data_size = files[i]->getSize();
			files[i]->write(&file);

			if (files[i]->getSize() == 0)
			{
				file_data_address = 0;
				file_data_size = 0;
			}

			file.goToAddress(data_table_address + i * 8);
			file.writeInt32E(&file_data_address);
			file.writeInt32E(&file_data_size);

			file.goToEnd();
		}

		if (enable_filenames)
		{
			//if(version != "1.0.1.0")				//only dbvx2 shader emb have no padding.
			//if (file_total > 1)						//on some sfxTk files, there is no padding , seam to be when there is only one file inside emb.
				file.fixPadding(0x40);

			for (size_t i = 0; i < file_total; i++) 
			{
				uint32_t string_address = file.getCurrentAddress();
				file.writeString(&files[i]->getName());
				
				file.goToAddress(string_table_address + i * 4);
				file.writeInt32E(&string_address);
				file.goToEnd();
			}
		}

		file.goToAddress(0x18);
		file.writeInt32E(&data_table_address);
		file.writeInt32E(&string_table_address);

		file.close();
	}













	void EMB::extract(string folder)
	{
		//add a Xml to keep files orders
		TiXmlDocument *doc = new TiXmlDocument();
		TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
		doc->LinkEndChild(decl);

		TiXmlElement* rootNode = new TiXmlElement("EMB");
		rootNode->SetAttribute("version", version);

		rootNode->SetAttribute("debugHaveNames", debugHaveNames ? "true" : "false");

		

		std::vector<string> listFilenameAllreadyCreated;
		std::vector<size_t> listIndexToRenameDuplicate;

		TiXmlElement* xmlNode;
		for (size_t i = 0; i < files.size(); i++)
		{
			string name = files[i]->getName();

			if (!name.size())							//if no name, it's dds texture.
			{
				if (i < 1000)
				{
					char suffix[] = "000";
					sprintf(suffix, "%03d", i);
					name = "DATA" + ToString(suffix) + ".dds";

				}else if(i < 10000){
					char suffix[] = "0000";
					sprintf(suffix, "%04d", i);
					name = "DATA" + ToString(suffix) + ".dds";

				}else{
					char suffix[] = "00000";
					sprintf(suffix, "%05d", i);
					name = "DATA" + ToString(suffix) + ".dds";
				}
			}

				

			string filename = name;
			filename = LibXenoverse::nameFromFilename(filename);


			bool isfound = false;
			size_t nbfilenames = listFilenameAllreadyCreated.size();
			for (size_t j = 0; j<nbfilenames; j++)					//solve duplicate name, in pbind.emb
			{
				if (listFilenameAllreadyCreated.at(j)== filename)
				{
					listIndexToRenameDuplicate.at(j) += 1;
					
					string basename = LibXenoverse::nameFromFilenameNoExtension(filename, true);
					string extension = filename.substr(basename.length());

					filename = basename + "_" + std::to_string(listIndexToRenameDuplicate.at(j)) + extension;

					break;
				}
			}
			if (!isfound)
			{
				listFilenameAllreadyCreated.push_back(filename);
				listIndexToRenameDuplicate.push_back(0);
			}

			files[i]->save(folder + filename);


			xmlNode = new TiXmlElement("File");
			xmlNode->SetAttribute("name", name);
			xmlNode->SetAttribute("filename", filename);

			//comments 
			TiXmlComment* xmlComment = new TiXmlComment(("index:"+ std::to_string(i)).c_str());
			rootNode->LinkEndChild(xmlComment);

			rootNode->LinkEndChild(xmlNode);
		}

		doc->LinkEndChild(rootNode);

		doc->SaveFile(folder +"embFiles.xml");
		delete doc;
	}


	void EMB::addFile(string filename)
	{
		if (fileCheck(filename)) {
			EMBFile *emb_file = new EMBFile(filename);
			emb_file->setIndex(files.size());

			if (emb_file) {
				files.push_back(emb_file);
			}
		}
	}

	void EMB::addFolder(string folder)
	{
		string filename_orderedFilesXml = "";
		
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile((folder + "*.*").c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
		
		}else{
			
			do
			{
				const char *name = FindFileData.cFileName;
				if (name[0] == '.')
					continue;

				string str = ToString(name);
				string new_filename = folder + str;
				if (str != "embFiles.xml")
					addFile(new_filename);
				else
					filename_orderedFilesXml = new_filename;				//there is a special file witch could give the order of files (when emb have filename, so Windows order is'nt good enougth)
			} while (FindNextFile(hFind, &FindFileData) != 0);
			FindClose(hFind);
		}

		if (filename_orderedFilesXml != "")
		{
			TiXmlDocument doc(filename_orderedFilesXml);
			if (doc.LoadFile())
			{
				TiXmlHandle hDoc(&doc);
				TiXmlHandle hRoot(0);

				TiXmlElement* rootNode = hDoc.FirstChildElement("EMB").Element();
				if (rootNode)
				{
					rootNode->QueryStringAttribute("version", &version);

					std::vector<string> listFilenames;
					std::vector<string> listnames;

					string str = "";
					string str2 = "";
					for (TiXmlElement* xmlNode = rootNode->FirstChildElement("File"); xmlNode; xmlNode = xmlNode->NextSiblingElement("File"))
					{
						if (xmlNode->QueryStringAttribute("name", &str) != TIXML_SUCCESS)
							continue;

						if (xmlNode->QueryStringAttribute("filename", &str2) != TIXML_SUCCESS)
							str2 = str;

						listnames.push_back(str);
						listFilenames.push_back(str2);
					}

					if (listFilenames.size())
					{
						vector<EMBFile*> newListFiles;

						string currentName = "";
						size_t nbFiles = listFilenames.size();
						size_t nbFiles_old = files.size();
						for (size_t i = 0; i < nbFiles; i++)
						{
							currentName = listFilenames.at(i);

							for (size_t j = 0; j < nbFiles_old; j++)					//try to match names
							{
								if (files.at(j)->getName() == currentName)
								{
									if (currentName != listnames.at(i))					//if name != filename
										files.at(j)->setName(listnames.at(i));			//just rename correctly
									
									newListFiles.push_back( files.at(j) );				//to order in a new list

									files.erase(files.begin() + j);
									nbFiles_old--;
								}
							}
						}

						for(size_t i=0;i<nbFiles_old;i++)							//new files in folder, witch is not referenced in the file, are add at the end.
							newListFiles.push_back(files.at(i));

						files = newListFiles;
					}
				}
			}
		}


		for(size_t i=0, nbFiles = files.size(); i<nbFiles; i++)
			if ((files[i]->getSize() == 0) && (files[i]->getName().length() > 4) && (files[i]->getName().substr(0, 4) == "DATA"))	//it's a sort of null file in emb, but it's named in extract
				files[i]->setName("");
	}



	bool EMB::detectFilenameMode() 
	{
		bool only_data_dds = true;
		for (size_t i = 0; i < files.size(); i++) 
		{
			string name = files[i]->getName();
			size_t data_found = name.find("DATA");
			size_t dds_found = name.find(".dds");

			if ((data_found == string::npos) || (dds_found == string::npos)) 
			{
				only_data_dds = false;
				break;
			}
		}

		return !only_data_dds;
	}


	EMB* EMB::clone(void)
	{
		EMB* emb = new EMB();
		emb->name = name;

		size_t nbFile = files.size();
		for (size_t i = 0; i < nbFile; i++)
			emb->files.push_back(files.at(i)->clone());

		return emb;
	}
}