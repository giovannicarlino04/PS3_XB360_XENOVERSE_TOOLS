#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>

#include <sstream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <climits>
#include <algorithm>

#ifdef _WIN32

#include <windows.h>
#include <direct.h>
#define do_mkdir(a, b) _mkdir(a)

#else
	
#define do_mkdir(a, b) mkdir(a, b)
	
#endif

#ifndef NO_ZLIB
#include <zlib.h>
#endif

#include "Utils.h"
#include "debug.h"

#ifndef NO_CRYPTO
#include "crypto/sha1.h"
#include "crypto/md5.h"
#include "crypto/rijndael.h"
#endif

#define FILE_BUFFER_SIZE	(16*1024*1024)

bool Utils::FileExists(const std::string &path)
{
    struct stat info;

    return (stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode));
}

bool Utils::DirExists(const std::string &path)
{
    struct stat info;

    return (stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode));
}

size_t Utils::GetFileSize(const std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return (size_t)-1;

    return info.st_size;
}

uint8_t *Utils::ReadFile(const std::string &path, size_t *psize, bool show_error)
{
    FILE *f = fopen(path.c_str(), "rb");
	if (!f)
	{
		if (show_error)
            DPRINTF("Cannot open file \"%s\" for reading.\n", path.c_str());
		
        return nullptr;
	}
	
	size_t size, rd;
    uint8_t *buf;
	
	fseeko64(f, 0, SEEK_END);
    size = (size_t)ftello64(f);
    fseeko64(f, 0, SEEK_SET);

    buf = new uint8_t[size];
	rd = fread(buf, 1, size, f);
	fclose(f);
	
	if (rd != size)
	{
		if (show_error)
            DPRINTF("Read failure on file \"%s\"\n", path.c_str());
		
		delete[] buf;
        return nullptr;
	}
	
	*psize = size;
	return buf;
}

uint8_t *Utils::ReadFileFrom(const std::string &path, size_t from, size_t size, bool show_error)
{
    FILE *f = fopen(path.c_str(), "rb");
    if (!f)
    {
        if (show_error)
            DPRINTF("Cannot open file \"%s\" for reading.\n", path.c_str());

        return nullptr;
    }

    size_t rd;
    uint8_t *buf;

    fseeko64(f, from, SEEK_SET);

    buf = new uint8_t[size];
    rd = fread(buf, 1, size, f);
    fclose(f);

    if (rd != size)
    {
        if (show_error)
            DPRINTF("Read failure on file \"%s\"\n", path.c_str());

        delete[] buf;
        return nullptr;
    }

    return buf;
}

bool Utils::ReadTextFile(const std::string &path, std::string &text, bool show_error)
{
    FILE *f = fopen(path.c_str(), "rb");
    if (!f)
    {
        if (show_error)
            DPRINTF("Cannot open file \"%s\" for reading.\n", path.c_str());

        return false;
    }

    size_t size, rd;
    char *buf;

    fseeko64(f, 0, SEEK_END);
    size = (size_t)ftello64(f);
    fseeko64(f, 0, SEEK_SET);

    buf = new char[size+1];
    buf[size] = 0;
    rd = fread(buf, 1, size, f);
    fclose(f);

    if (rd != size)
    {
        if (show_error)
            DPRINTF("Read failure on file \"%s\"\n", path.c_str());

        delete[] buf;
        return false;
    }

    text = buf;
    delete[] buf;

    return true;
}

size_t Utils::WriteFile(const std::string &path, const uint8_t *buf, size_t size, bool show_error, bool write_path)
{
    FILE *f = (write_path) ? Utils::fopen_create_path(path, "wb") : fopen(path.c_str(), "wb");
	
	if (!f)
	{
		if (show_error)
		{
            DPRINTF("Cannot open for write/create file \"%s\"\n", path.c_str());
		}
		
		return -1;
	}
	
	size_t wd = fwrite(buf, 1, size, f);
	fclose(f);
	
	return wd;	
}

bool Utils::WriteFileBool(const std::string &path, const uint8_t *buf, size_t size, bool show_error, bool write_path)
{
	size_t written = Utils::WriteFile(path, buf, size, show_error, write_path);
	
    if ((int)written < 0)
		return false;
	
	if (written != size)
	{
		if (show_error)
            DPRINTF("Couldn't write same amount of bytes as requestedto file \"%s\" (disk full?)\n", path.c_str());
		
		return false;			
	}
	
	return true;
}

bool Utils::Mkdir(const std::string &path)
{
    if (do_mkdir(path.c_str(), 0777) != 0)
    {
        if (errno != EEXIST)
            return false;
    }

    return true;
}

bool Utils::RemoveFile(const std::string &path)
{
    if (remove(path.c_str()) == 0)
        return true;

    return (errno == ENOENT);
}

bool Utils::CreatePath(const std::string &path, bool last_is_directory)
{
	size_t pos = std::string::npos;
	size_t prev_pos = std::string::npos;
	
	while ((pos = path.find_first_of("/\\", pos+1)) != std::string::npos)
	{
		std::string current_dir;
				
		std::string dir = path.substr(0, pos);
		
		if (prev_pos != std::string::npos)
		{
			current_dir = dir.substr(prev_pos);			
		}
		else
		{
			current_dir = dir;
		}
		
		if (current_dir.find_first_of("%:") == std::string::npos)
		{
			if (do_mkdir(dir.c_str(), 0777) != 0)
			{
				if (errno != EEXIST)
                {
                    //DPRINTF("returning false on %s\n", dir.c_str());
                    return false;
                }
			}
		}		

		prev_pos = pos;
	}
	
	if (!last_is_directory)
		return true;
	
	if (do_mkdir(path.c_str(), 0777) != 0)
	{
		return (errno == EEXIST);
	}
	
	return true;
}

FILE *Utils::fopen_create_path(const std::string &filename, const char *mode)
{
	if (!Utils::CreatePath(filename))
        return nullptr;
	
    return fopen(filename.c_str(), mode);
}

bool Utils::VisitDirectory(const std::string &path, bool files, bool directories, bool recursive, bool (* visitor)(const std::string &path, bool is_directory, void *custom_param), void *custom_param)
{
	DIR *dir = opendir(path.c_str());
	struct dirent *entry;
		
	if (!dir)
		return false;
	
	while ((entry = readdir(dir)))
	{
		struct stat st;
		std::string file_path = path + "/" + entry->d_name;
		
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		
		if (stat(file_path.c_str(), &st) != 0)
			continue;
		
		if (S_ISREG(st.st_mode) && files)
		{
			bool ret = visitor(file_path, false, custom_param);			
				
			if (!ret)
			{
				closedir(dir);
				return false;
			}
		}
		
		if (S_ISDIR(st.st_mode))
		{
			if (directories)
			{
				bool ret = visitor(file_path, true, custom_param);
							
				if (!ret)
				{
					closedir(dir);
					return false;
				}
				
			}
			
			if (recursive)
			{
				if (!VisitDirectory(file_path, files, directories, recursive, visitor, custom_param))
					return false;
			}
		}
	}
	
	closedir(dir);
	return true;
}

static bool is_dir_empty_visitor(const std::string &path, bool is_directory, void *custom_param)
{
    UNUSED(path); UNUSED(is_directory); UNUSED(custom_param);
    return false;
}

bool Utils::IsDirectoryEmpty(const std::string &path, bool ignore_directories)
{
    return VisitDirectory(path, true, !ignore_directories, ignore_directories, is_dir_empty_visitor);
}

bool Utils::CompareFiles(const std::string &file1, const std::string &file2)
{
	struct stat st1, st2;
	unsigned int buf_size;
	size_t remaining;
	
	if (stat(file1.c_str(), &st1) != 0 || stat(file2.c_str(), &st2) != 0)
		return false;
	
	if (st1.st_size != st2.st_size)
		return false;
	
	FILE *f1 = fopen(file1.c_str(), "rb");
	FILE *f2 = fopen(file2.c_str(), "rb");
	
	if (!f1 || !f2)
	{
		if (f1)
			fclose(f1);
		
		if (f2)
			fclose(f2);
		
		return false;
	}
	
	buf_size = (st1.st_size < FILE_BUFFER_SIZE) ? st1.st_size : FILE_BUFFER_SIZE;	
	
    uint8_t *buf = new uint8_t[buf_size];

	remaining = st1.st_size;
	
	while (remaining > 0)
	{
		uint8_t sha1_1[20];
		uint8_t sha1_2[20];
		
		size_t r = (remaining < buf_size) ? remaining : buf_size;
		
		if (fread(buf, 1, r, f1) != r)
		{
			fclose(f1);
			fclose(f2);
			delete[] buf;
			return false;
		}
		
        Sha1(buf, (uint32_t)r, sha1_1);
		
		if (fread(buf, 1, r, f2) != r)
		{
			fclose(f1);
			fclose(f2);
			delete[] buf;
			return false;
		}
		
        Sha1(buf, (uint32_t)r, sha1_2);
		
		if (memcmp(sha1_1, sha1_2, sizeof(sha1_1)) != 0)
		{
			fclose(f1);
			fclose(f2);
			delete[] buf;
			return false;
		}
		
		remaining -= r;
	}
	
	fclose(f1);
	fclose(f2);
	delete[] buf;
	
	return true;
}

bool Utils::CompareFilesPartial(const std::string &file1, const std::string &file2, uint64_t compare_size)
{
	struct stat st1, st2;
    size_t buf_size;
	size_t remaining;
	
	if (stat(file1.c_str(), &st1) != 0 || stat(file2.c_str(), &st2) != 0)
		return false;
	
	if (st1.st_size < (off_t)compare_size || st2.st_size < (off_t)compare_size)
		return false;
	
	FILE *f1 = fopen(file1.c_str(), "rb");
	FILE *f2 = fopen(file2.c_str(), "rb");
	
	if (!f1 || !f2)
	{
		if (f1)
			fclose(f1);
		
		if (f2)
			fclose(f2);
		
		return false;
	}
	
	buf_size = (compare_size < FILE_BUFFER_SIZE) ? compare_size : FILE_BUFFER_SIZE;	
	
    uint8_t *buf = new uint8_t[buf_size];

	remaining = compare_size;
	
	while (remaining > 0)
	{
		uint8_t sha1_1[20];
		uint8_t sha1_2[20];
		
		size_t r = (remaining < buf_size) ? remaining : buf_size;
		
		if (fread(buf, 1, r, f1) != r)
		{
			fclose(f1);
			fclose(f2);
			delete[] buf;
			return false;
		}
		
        Sha1(buf, (uint32_t)r, sha1_1);
		
		if (fread(buf, 1, r, f2) != r)
		{
			fclose(f1);
			fclose(f2);
			delete[] buf;
			return false;
		}
		
        Sha1(buf, (uint32_t)r, sha1_2);
		
		if (memcmp(sha1_1, sha1_2, sizeof(sha1_1)) != 0)
		{
			fclose(f1);
			fclose(f2);
			delete[] buf;
			return false;
		}
		
		remaining -= r;
	}
	
	fclose(f1);
	fclose(f2);
	delete[] buf;
	
	return true;
}

bool Utils::DoCopyFile(FILE *src, FILE *dst, uint64_t size)
{
	uint8_t *copy_buf;
	size_t remaining;
		
    if (size < FILE_BUFFER_SIZE)
    {
        copy_buf = new uint8_t[size];
    }
    else
    {
        copy_buf = new uint8_t[FILE_BUFFER_SIZE];
    }

	remaining = size;
	
	while (remaining > 0)
	{
		size_t r = (remaining > FILE_BUFFER_SIZE) ? FILE_BUFFER_SIZE: remaining;
		
		if (fread(copy_buf, 1, r, src) != r)
		{
			delete[] copy_buf;
			return false;
		}
		
		if (fwrite(copy_buf, 1, r, dst) != r)
		{
			delete[] copy_buf;						
			return false;
		}
		
		remaining -= r;
	}
	
	delete[] copy_buf;	
	return true;
}

bool Utils::DoCopyFile(const std::string &input, const std::string &output, bool build_path)
{
	FILE *r = fopen(input.c_str(), "rb");
	if (!r)
		return false;
	
	FILE *w= (build_path) ? fopen_create_path(output, "wb") : fopen(output.c_str(), "wb");
	if (!w)
	{
		fclose(r);
		return false;
	}
	
	fseeko64(r, 0, SEEK_END);
	off64_t size = ftello64(r);
	fseeko64(r, 0, SEEK_SET);
	
    bool ret = DoCopyFile(r, w, (size_t)size);
	
	fclose(r);
	fclose(w);
	return ret;
}

std::string Utils::UnsignedToString(uint64_t value, bool hexadecimal)
{
	char temp[16];
	std::string str;
	
	if (hexadecimal)
	{
        sprintf(temp, "0x%I64x", value);
	}
	else
	{
        sprintf(temp, "%I64u", value);
	}
	
	str = temp;
	return str;
}

std::string Utils::SignedToString(int64_t value)
{
    char temp[16];
    std::string str;

    sprintf(temp, "%I64d", value);

    str = temp;
    return str;
}

std::string Utils::FloatToString(float value)
{
	char temp[32];
	std::string str;
	
	sprintf(temp, "%.9g", value);
	str = temp;
	
	if (str.find('.') == std::string::npos && str.find('e') == std::string::npos)
	{
		str = str + ".0";
	}
	
	return str;
}

void Utils::TrimString(std::string & str, bool trim_left, bool trim_right)
{
	size_t pos = 0;
	
	if (trim_left)
	{
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] > ' ' || str[i] < 0)
			{
				pos = i;
				break;
			}
		}
		
		if (pos != 0)
		{
			str = str.substr(pos);
		}
	}
	
	if (trim_right)
	{
		pos = str.length()-1;
		
        for (size_t i = str.length()-1; i != std::string::npos; i--)
		{
			if (str[i] > ' ' || str[i] < 0)
			{
				pos = i;
				break;
			}
		}
		
		if (pos != (str.length()-1))
		{
			str = str.substr(0, pos + 1);
		}
	}
}

bool Utils::IsEmptyString(const std::string & str)
{
	for (char c : str)
	{
		if (c > ' ' || c < 0)
			return false;
	}
	
	return true;
}

std::string Utils::GetFileNameString(const std::string &path)
{
    size_t s1 = path.rfind('/');
    size_t s2 = path.rfind('\\');
    size_t pos = std::string::npos;

    if (s1 == std::string::npos)
    {
        if (s2 != std::string::npos)
            pos = s2;
    }
    else
    {
        if (s2 != std::string::npos)
            pos = (s1 > s2) ? s1 : s2;
        else
            pos = s1;
    }

    if (pos == std::string::npos)
        return path;

    return path.substr(pos+1);
}

size_t Utils::GetMultipleStrings(const std::string & str_param, std::vector<std::string> & list, char separator)
{
	size_t pos;
	std::string str = str_param;
	
	list.clear();
	
	if (str.length() == 0 || str == "NULL")
	{
        return 0;
	}
	
    while ((pos = str.find(separator)) != std::string::npos)
	{
		std::string this_string = str.substr(0, pos);
		
		if (!IsEmptyString(this_string))
		{
			Utils::TrimString(this_string);
			list.push_back(this_string);
		}
		
		str = str.substr(pos+1);
	}
	
	// Add the last string, 
	if (!IsEmptyString(str))
	{
		Utils::TrimString(str);
		list.push_back(str);	
    }

    return list.size();
}

std::string Utils::ToSingleString(const std::vector<std::string> &list)
{
	std::string ret;
	
	for (const std::string &s : list)
	{
		if (ret.length() != 0)
			ret += ", ";
		
		ret += s;
	}

	return ret;
}

std::string Utils::ToSingleString(const std::vector<uint32_t> &list, bool hexadecimal)
{
	std::string ret;
	
	for (uint32_t u : list)
	{
		if (ret.length() != 0)
			ret += ", ";
		
		ret += UnsignedToString(u, hexadecimal);
	}
	
	return ret;
}

std::string Utils::ToSingleString(const std::vector<uint16_t> &list, bool hexadecimal)
{
	std::string ret;
	
	for (uint16_t u : list)
	{
		if (ret.length() != 0)
			ret += ", ";
		
		ret += UnsignedToString(u, hexadecimal);
	}
	
	return ret;
}

std::string Utils::ToSingleString(const std::vector<uint8_t> &list, bool hexadecimal)
{
	std::string ret;
	
	for (uint8_t u : list)
	{
		if (ret.length() != 0)
			ret += ", ";
		
		ret += UnsignedToString(u, hexadecimal);
	}
	
	return ret;
}

std::string Utils::ToSingleString(const std::vector<float> &list)
{
	std::string ret;
	
	for (float f: list)
	{
		if (ret.length() != 0)
			ret += ", ";
		
		ret += FloatToString(f);
	}
	
	return ret;
}

std::string Utils::ToLowerCase(const std::string & str)
{
	std::string ret = str;
	
	for (char &c : ret)
	{
		if (c >= 'A' && c <= 'Z')
		{
			c = c + ('a' - 'A');
		}
	}
	
	return ret;
}

bool Utils::BeginsWith(const std::string &str, const std::string &substr, bool case_sensitive)
{
    size_t len1 = str.length();
    size_t len2 = substr.length();

    if (len2 > len1)
        return false;

    if (case_sensitive)
        return (str.substr(0, len2) == substr);

    std::string lstr = ToLowerCase(str);
    std::string lsubstr = ToLowerCase(substr);

    return (lstr.substr(0, len2) == lsubstr);
}

bool Utils::EndsWith(const std::string &str, const std::string &substr, bool case_sensitive)
{
	size_t len1 = str.length();
	size_t len2 = substr.length();
	size_t pos;
	
	if (len2 > len1)
		return false;
	
	if (case_sensitive)
	{
		pos = str.rfind(substr);
	}
	else
	{
		std::string lstr = ToLowerCase(str);
		std::string lsubstr = ToLowerCase(substr);
		
		pos = lstr.rfind(lsubstr);
	}
	
	return (pos == (len1-len2));
}

std::string Utils::GUID2String(const uint8_t *guid)
{
	std::string ret;
	
	for (int i = 0; i < 16; i++)
	{
		uint8_t h, l;
		char ch;
		
		h = guid[i] >> 4;
		l = guid[i] & 0xF;
		
        if (h <= 9)
			ch = h + '0';
		else
			ch = (h-10) + 'a';
		
		ret += ch;
		
        if (l <= 9)
			ch = l + '0';
		else
			ch = (l-10) + 'a';
		
		ret += ch;
		
		if (i == 3 || i == 5 || i == 7 || i == 9)
			ret += '-';
	}
	
	return ret;
}

std::string Utils::NormalizePath(const std::string &path)
{
	std::string new_path = path;
	
	for (char &c : new_path)
	{
		if (c == '\\')
			c = '/';
	}
	
	return new_path;
}

std::string Utils::RandomString(size_t len)
{
    std::string str;
    str.resize(len);

    for (char &c : str)
    {
        if (RandomInt(0, 0x10000) & 1)
        {
            c = (char)Utils::RandomInt('A', 'Z');
        }
        else
        {
            c = (char)Utils::RandomInt('a', 'z');
        }
    }

    return str;
}

uint32_t Utils::GetUnsigned(const std::string &str, uint32_t default_value)
{
	uint32_t ret = 0;
    size_t len = str.length();
	
	if (len == 0)
	{
        //DPRINTF("WARNING: length of integer string = 0 (param \"%s\"), setting value to 0.\n", param_name.c_str());
        return default_value;
	}
	
	if (str[0] == '0')
	{
		if (len == 1)
			return 0;
		
		if (str[1] != 'x')
		{
            /*DPRINTF("WARNING: Integer format error on \"%s\". "
							"Value must be decimal values without any 0 on the left, or hexadecimal values with 0x prefix. "
							"Octal values not allowed (offending_string = %s). "
                            "Setting value to 0.\n", param_name.c_str(), str.c_str());*/
							
            return default_value;
		}
		
		if (len == 2)
		{
            //DPRINTF("WARNING: nothing on the right of hexadecimal prefix (on param \"%s\"). Setting value to 0.\n", param_name.c_str());
            return default_value;
		}
		
        if (sscanf(str.c_str()+2, "%x", &ret) != 1)
		{
            //DPRINTF("sscanf failed on param \"%s\", offending string = \"%s\"\n. Setting value to 0.", param_name.c_str(), str.c_str());
            return default_value;
		}
	}
	else
	{
        if (sscanf(str.c_str(), "%u", &ret) != 1)
		{
            //DPRINTF("sscanf failed on param \"%s\", offending string = \"%s\"\n. Setting value to 0.", param_name.c_str(), str.c_str());
            return default_value;
		}
	}
	
	return ret;
}

uint64_t Utils::GetUnsigned64(const std::string &str, uint64_t default_value)
{
    uint64_t ret = 0;
    size_t len = str.length();

    if (len == 0)
    {
        //DPRINTF("WARNING: length of integer string = 0 (param \"%s\"), setting value to 0.\n", param_name.c_str());
        return default_value;
    }

    if (str[0] == '0')
    {
        if (len == 1)
            return 0;

        if (str[1] != 'x')
        {
            /*DPRINTF("WARNING: Integer format error on \"%s\". "
                            "Value must be decimal values without any 0 on the left, or hexadecimal values with 0x prefix. "
                            "Octal values not allowed (offending_string = %s). "
                            "Setting value to 0.\n", param_name.c_str(), str.c_str());*/

            return default_value;
        }

        if (len == 2)
        {
            //DPRINTF("WARNING: nothing on the right of hexadecimal prefix (on param \"%s\"). Setting value to 0.\n", param_name.c_str());
            return default_value;
        }

        if (sscanf(str.c_str()+2, "%I64x", &ret) != 1)
        {
            //DPRINTF("sscanf failed on param \"%s\", offending string = \"%s\"\n. Setting value to 0.", param_name.c_str(), str.c_str());
            return default_value;
        }
    }
    else
    {
        if (sscanf(str.c_str(), "%I64u", &ret) != 1)
        {
            //DPRINTF("sscanf failed on param \"%s\", offending string = \"%s\"\n. Setting value to 0.", param_name.c_str(), str.c_str());
            return default_value;
        }
    }

    return ret;
}

int32_t Utils::GetSigned(const std::string &str, int32_t default_value)
{
    int32_t ret = 0;
    size_t len = str.length();

    if (len == 0)
    {
        //DPRINTF("WARNING: length of integer string = 0 (param \"%s\"), setting value to 0.\n", param_name.c_str());
        return default_value;
    }

    if (str[0] == '0')
    {
        if (len == 1)
            return 0;

        if (str[1] != 'x')
        {
            /*DPRINTF("WARNING: Integer format error on \"%s\". "
                            "Value must be decimal values without any 0 on the left, or hexadecimal values with 0x prefix. "
                            "Octal values not allowed (offending_string = %s). "
                            "Setting value to 0.\n", param_name.c_str(), str.c_str());*/

            return default_value;
        }

        if (len == 2)
        {
            //DPRINTF("WARNING: nothing on the right of hexadecimal prefix (on param \"%s\"). Setting value to 0.\n", param_name.c_str());
            return default_value;
        }

        if (sscanf(str.c_str()+2, "%x", &ret) != 1)
        {
            //DPRINTF("sscanf failed on param \"%s\", offending string = \"%s\"\n. Setting value to 0.", param_name.c_str(), str.c_str());
            return default_value;
        }
    }
    else
    {
        if (sscanf(str.c_str(), "%d", &ret) != 1)
        {
            //DPRINTF("sscanf failed on param \"%s\", offending string = \"%s\"\n. Setting value to 0.", param_name.c_str(), str.c_str());
            return default_value;
        }
    }

    return ret;
}

bool Utils::GetBoolean(const std::string &str, bool default_value)
{
    std::string boolean_str = Utils::ToLowerCase(str);

    if (boolean_str == "false" || boolean_str == "0")
        return false;

    if (boolean_str == "true" || GetUnsigned(str) > 0)
        return true;

    return default_value;
}

TiXmlElement *Utils::FindRoot(TiXmlHandle *handle, const std::string &root_name)
{
    for (TiXmlElement *elem = handle->FirstChildElement().Element(); elem != nullptr; elem = elem->NextSiblingElement())
    {
        std::string name = elem->ValueStr();

        if (name == root_name)
        {
            return elem;
        }
    }

    return nullptr;
}

size_t Utils::GetElemCount(const TiXmlElement *root, const char *name, const TiXmlElement **first)
{
    size_t count = 0;

    for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
    {
        if (elem->ValueStr() == name)
        {
            if (first && count == 0)
            {
                *first = elem;
            }

            count++;
        }
    }

    return count;
}

void Utils::WriteParamString(TiXmlElement *root, const char *name, const std::string & value)
{
	TiXmlElement *param = new TiXmlElement(name);	
	param->SetAttribute("value", value);	
	root->LinkEndChild(param);
}

void Utils::WriteParamMultipleStrings(TiXmlElement *root, const char *name, const std::vector<std::string> & values)
{
	TiXmlElement *param = new TiXmlElement(name);
	param->SetAttribute("value", ToSingleString(values));
	root->LinkEndChild(param);
}

void Utils::WriteParamUnsigned(TiXmlElement *root, const char *name, uint64_t value, bool hexadecimal)
{
	TiXmlElement *param = new TiXmlElement(name);	
	param->SetAttribute("value", UnsignedToString(value, hexadecimal));	
	root->LinkEndChild(param);
}

void Utils::WriteParamSigned(TiXmlElement *root, const char *name, int64_t value)
{
    TiXmlElement *param = new TiXmlElement(name);
    param->SetAttribute("value", SignedToString(value));
    root->LinkEndChild(param);
}

void Utils::WriteParamMultipleUnsigned(TiXmlElement *root, const char *name, const std::vector<uint32_t> &values, bool hexadecimal)
{
	TiXmlElement *param = new TiXmlElement(name);
	param->SetAttribute("value", ToSingleString(values, hexadecimal));
	root->LinkEndChild(param);
}

void Utils::WriteParamMultipleUnsigned(TiXmlElement *root, const char *name, const std::vector<uint16_t> &values, bool hexadecimal)
{
	TiXmlElement *param = new TiXmlElement(name);
	param->SetAttribute("value", ToSingleString(values, hexadecimal));
	root->LinkEndChild(param);
}

void Utils::WriteParamMultipleUnsigned(TiXmlElement *root, const char *name, const std::vector<uint8_t> &values, bool hexadecimal)
{
	TiXmlElement *param = new TiXmlElement(name);
	param->SetAttribute("value", ToSingleString(values, hexadecimal));
	root->LinkEndChild(param);
}

void Utils::WriteParamFloat(TiXmlElement *root, const char *name, float value)
{
	TiXmlElement *param = new TiXmlElement(name);	
	param->SetAttribute("value", FloatToString(value));	
	root->LinkEndChild(param);
}

void Utils::WriteParamMultipleFloats(TiXmlElement *root, const char *name, const std::vector<float> &values)
{
	TiXmlElement *param = new TiXmlElement(name);
	param->SetAttribute("value", ToSingleString(values));
	root->LinkEndChild(param);
}

void Utils::WriteParamGUID(TiXmlElement *root, const char *name, const uint8_t *value)
{
	std::string str = Utils::GUID2String(value);
	WriteParamString(root, name, str);
}

void Utils::WriteParamBlob(TiXmlElement *root, const char *name, const uint8_t *value, size_t size)
{
	TiXmlElement *param = new TiXmlElement(name);		
	TiXmlText *base64= new TiXmlText(Base64Encode(value, size, true));
	
	base64->SetCDATA(true);	
	param->LinkEndChild(base64);	
	root->LinkEndChild(param);
}

bool Utils::ReadAttrString(const TiXmlElement *root, const char *name, std::string & value)
{
	if (root->QueryStringAttribute(name, &value) != TIXML_SUCCESS)
		return false;
	
	return true;
}

bool Utils::ReadAttrMultipleStrings(const TiXmlElement *root, const char *name, std::vector<std::string> &values)
{
    std::string str;

    if (!ReadAttrString(root, name, str))
        return false;

    GetMultipleStrings(str, values);
    return true;
}

bool Utils::ReadAttrUnsigned(const TiXmlElement *root,  const char *name, uint32_t *value)
{
	std::string str;
	
	if (root->QueryStringAttribute(name, &str) != TIXML_SUCCESS)
		return false;
	
    *value = GetUnsigned(str.c_str());
	return true;
}

bool Utils::ReadAttrUnsigned(const TiXmlElement *root,  const char *name, uint64_t *value)
{
    std::string str;

    if (root->QueryStringAttribute(name, &str) != TIXML_SUCCESS)
        return false;

    *value = GetUnsigned64(str.c_str());
    return true;
}

bool Utils::ReadAttrFloat(const TiXmlElement *root, const char *name, float *value)
{
    std::string str;

    if (root->QueryFloatAttribute(name, value) != TIXML_SUCCESS)
        return false;

    return true;
}

bool Utils::ReadParamString(const TiXmlElement *root, const char *name, std::string &value, const TiXmlElement **ret)
{
	bool found = false;
	
    for (const TiXmlElement *elem = root->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
	{
		std::string str;
		
		str = elem->ValueStr();
		
		if (str == name)
		{
			if (elem->QueryStringAttribute("value", &value) != TIXML_SUCCESS)
				return false;
			
			found = true;
            if (ret)
                *ret = elem;

			break;
		}
	}
	
	return found;
}

bool Utils::ReadParamMultipleStrings(const TiXmlElement *root, const char *name, std::vector<std::string> & values, const TiXmlElement **ret)
{
	std::string str;
		
    if (!ReadParamString(root, name, str, ret))
		return false;
	
	GetMultipleStrings(str, values);	
	return true;
}

bool Utils::ReadParamUnsigned(const TiXmlElement *root, const char *name, uint32_t *value)
{
	std::string str;
	
	if (!ReadParamString(root, name, str))
		return false;
	
    *value = GetUnsigned(str.c_str());
	return true;
}

bool Utils::ReadParamUnsigned(const TiXmlElement *root, const char *name, uint64_t *value)
{
    std::string str;

    if (!ReadParamString(root, name, str))
        return false;

    *value = GetUnsigned64(str.c_str());
    return true;
}

bool Utils::ReadParamMultipleUnsigned(const TiXmlElement *root, const char *name, std::vector<uint32_t> &values)
{
	std::vector<std::string> values_str;
	
	values.clear();
	
	if (!ReadParamMultipleStrings(root, name, values_str))
		return false;
	
	values.reserve(values_str.size());
	
	for (const std::string &s : values_str)
	{
        values.push_back(GetUnsigned(s.c_str()));
	}
	
	return true;
}

bool Utils::ReadParamMultipleUnsigned(const TiXmlElement *root, const char *name, std::vector<uint16_t> &values)
{
	std::vector<std::string> values_str;
	
	values.clear();
	
	if (!ReadParamMultipleStrings(root, name, values_str))
		return false;
	
	values.reserve(values_str.size());
	
	for (const std::string &s : values_str)
	{
        uint32_t value = GetUnsigned(s.c_str());
		
		if (value > 0xFFFF)
			return false;
		
		values.push_back(value);	
	}
	
	return true;
}

bool Utils::ReadParamMultipleUnsigned(const TiXmlElement *root, const char *name, std::vector<uint8_t> &values)
{
	std::vector<std::string> values_str;
	
	values.clear();
	
	if (!ReadParamMultipleStrings(root, name, values_str))
		return false;
	
	values.reserve(values_str.size());
	
	for (const std::string &s : values_str)
	{
        uint32_t value = GetUnsigned(s.c_str());
		
		if (value > 0xFF)
			return false;
		
		values.push_back(value);	
	}
	
	return true;
}

bool Utils::ReadParamMultipleUnsigned(const TiXmlElement *root, const char *name, uint32_t *values, size_t count)
{
    std::vector<uint32_t> vec;

    if (!ReadParamMultipleUnsigned(root, name, vec))
        return false;

    if (vec.size() != count)
        return false;

    memcpy(values, vec.data(), count*sizeof(uint32_t));
    return true;
}

bool Utils::ReadParamMultipleUnsigned(const TiXmlElement *root, const char *name, uint16_t *values, size_t count)
{
    std::vector<uint16_t> vec;

    if (!ReadParamMultipleUnsigned(root, name, vec))
        return false;

    if (vec.size() != count)
        return false;

    memcpy(values, vec.data(), count*sizeof(uint16_t));
    return true;
}

bool Utils::ReadParamMultipleUnsigned(const TiXmlElement *root, const char *name, uint8_t *values, size_t count)
{
    std::vector<uint8_t> vec;

    if (!ReadParamMultipleUnsigned(root, name, vec))
        return false;

    if (vec.size() != count)
        return false;

    memcpy(values, vec.data(), count*sizeof(uint8_t));
    return true;
}

bool Utils::ReadParamSigned(const TiXmlElement *root, const char *name, int32_t *value)
{
    std::string str;

    if (!ReadParamString(root, name, str))
        return false;

    *value = GetSigned(str.c_str());
    return true;
}

bool Utils::ReadParamFloat(const TiXmlElement *root, const char *name, float *value)
{
	bool found = false;
	
    for (const TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string str;
		
		str = elem->ValueStr();
		
		if (str == name)
		{
			if (elem->QueryFloatAttribute("value", value) != TIXML_SUCCESS)
				return false;
			
			found = true;
			break;
		}
	}
	
	return found;
}

bool Utils::ReadParamMultipleFloats(const TiXmlElement *root, const char *name, std::vector<float> &values)
{
	std::vector<std::string> values_str;
	
	values.clear();
	
	if (!ReadParamMultipleStrings(root, name, values_str))
		return false;
	
	values.reserve(values_str.size());
	
	for (const std::string &s : values_str)
	{
		float value;
		
		if (sscanf(s.c_str(), "%f", &value) != 1)
			return false;
		
		values.push_back(value);
	}
	
	return true;
}

bool Utils::ReadParamMultipleFloats(const TiXmlElement *root, const char *name, float *values, size_t count)
{
    std::vector<float> vec;

    if (!ReadParamMultipleFloats(root, name, vec))
        return false;

    if (vec.size() != count)
        return false;

    memcpy(values, vec.data(), count*sizeof(float));
    return true;
}

bool Utils::ReadParamGUID(const TiXmlElement *root, const char *name, uint8_t *value)
{
	std::string guid;
	
	if (!ReadParamString(root, name, guid))
		return false;
	
	TrimString(guid);
	
	if (guid.length() != 36)
		return false;
	
	bool high_flag = true;
	uint8_t h = 0;
	
	for (size_t i = 0, j = 0; i < guid.length(); i++)
	{
		if (i == 8 || i == 13 || i == 18 || i == 23)
		{
			if (guid[i] != '-')
				return false;
		}
		else
		{
			uint8_t d;
			
			if (guid[i] >= '0' && guid[i] <= '9')
			{
				d = guid[i] - '0';
			}
			else if (guid[i] >= 'a' && guid[i] <= 'f')
			{
				d = guid[i] - 'a' + 10;
			}
			else if (guid[i] >= 'A' && guid[i] <= 'F')
			{
				d = guid[i] - 'A' + 10;
			}
			else
			{
				return false;
			}
			
			if (high_flag)
			{
				h = d << 4;
			}
			else
			{
				value[j++] = h | d;				
			}
			
			high_flag = !high_flag;			
		}
	}	
	
	return true;
}

uint8_t *Utils::ReadParamBlob(const TiXmlElement *root, const char *name, size_t *psize)
{
    for (const TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string str;
		
		str = elem->ValueStr();
		
		if (str == name)
		{
			std::string base64_data = elem->GetText();
			return Base64Decode(base64_data, psize);
		}
	}
	
	return nullptr;
}

bool Utils::ReadParamUnsignedWithMultipleNames(const TiXmlElement *root, uint32_t *value, const char *name1, const char *name2, const char *name3, const char *name4, const char *name5)
{
	if (ReadParamUnsigned(root, name1, value))
		return true;
	
	if (ReadParamUnsigned(root, name2, value))
		return true;
	
	if (name3 && ReadParamUnsigned(root, name3, value))
		return true;
	
	if (name4 && ReadParamUnsigned(root, name4, value))
		return true;
	
    return (name5 && ReadParamUnsigned(root, name5, value));
}

bool Utils::ReadParamFloatWithMultipleNames(const TiXmlElement *root, float *value, const char *name1, const char *name2, const char *name3, const char *name4, const char *name5)
{
    if (ReadParamFloat(root, name1, value))
        return true;

    if (ReadParamFloat(root, name2, value))
        return true;

    if (name3 && ReadParamFloat(root, name3, value))
        return true;

    if (name4 && ReadParamFloat(root, name4, value))
        return true;

    return (name5 && ReadParamFloat(root, name5, value));
}

bool Utils::GetParamString(const TiXmlElement *root, const char *name, std::string &value, const TiXmlElement **ret)
{
    if (!ReadParamString(root, name, value, ret))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamMultipleStrings(const TiXmlElement *root, const char *name, std::vector<std::string> & values, const TiXmlElement **ret)
{
    if (!ReadParamMultipleStrings(root, name, values, ret))
	{
		DPRINTF("Cannot read parameter \"%s\"\n", name);
		return false;
	}
	
	return true;
}

bool Utils::GetParamUnsigned(const TiXmlElement *root, const char *name, uint32_t *value)
{
    if (!ReadParamUnsigned(root, name, value))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamUnsigned(const TiXmlElement *root, const char *name, uint64_t *value)
{
    if (!ReadParamUnsigned(root, name, value))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamMultipleUnsigned(const TiXmlElement *root, const char *name, std::vector<uint32_t> &values)
{
	if (!ReadParamMultipleUnsigned(root, name, values))
	{
		DPRINTF("Cannot read parameter \"%s\"\n", name);
		return false;
	}
	
	return true;
}

bool Utils::GetParamMultipleUnsigned(const TiXmlElement *root, const char *name, std::vector<uint16_t> &values)
{
	if (!ReadParamMultipleUnsigned(root, name, values))
	{
		DPRINTF("Cannot read parameter \"%s\"\n", name);
		return false;
	}
	
	return true;
}

bool Utils::GetParamMultipleUnsigned(const TiXmlElement *root, const char *name, std::vector<uint8_t> &values)
{
	if (!ReadParamMultipleUnsigned(root, name, values))
	{
		DPRINTF("Cannot read parameter \"%s\"\n", name);
		return false;
	}
	
	return true;
}

bool Utils::GetParamMultipleUnsigned(const TiXmlElement *root, const char *name, uint32_t *values, size_t count)
{
    if (!ReadParamMultipleUnsigned(root, name, values, count))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamMultipleUnsigned(const TiXmlElement *root, const char *name, uint16_t *values, size_t count)
{
    if (!ReadParamMultipleUnsigned(root, name, values, count))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamMultipleUnsigned(const TiXmlElement *root, const char *name, uint8_t *values, size_t count)
{
    if (!ReadParamMultipleUnsigned(root, name, values, count))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamSigned(const TiXmlElement *root, const char *name, int32_t *value)
{
    if (!ReadParamSigned(root, name, value))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamFloat(const TiXmlElement *root, const char *name, float *value)
{
    if (!ReadParamFloat(root, name, value))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamMultipleFloats(const TiXmlElement *root, const char *name, std::vector<float> &values)
{
	if (!ReadParamMultipleFloats(root, name, values))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;	
}

bool Utils::GetParamMultipleFloats(const TiXmlElement *root, const char *name, float *values, size_t count)
{
    if (!ReadParamMultipleFloats(root, name, values, count))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

bool Utils::GetParamGUID(const TiXmlElement *root, const char *name, uint8_t *value)
{
    if (!ReadParamGUID(root, name, value))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name);
        return false;
    }

    return true;
}

uint8_t *Utils::GetParamBlob(const TiXmlElement *root, const char *name, size_t *psize)
{
	uint8_t *ret = ReadParamBlob(root, name, psize);
	if (!ret)
	{
		DPRINTF("Cannot read parameter \"%s\"\n", name);
	}
	
	return ret;
}

bool Utils::GetParamUnsignedWithMultipleNames(const TiXmlElement *root, uint32_t *value, const char *name1, const char *name2, const char *name3, const char *name4, const char *name5)
{
    if (!ReadParamUnsignedWithMultipleNames(root, value, name1, name2, name3, name4, name5))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name1);
        return false;
    }

    return true;
}

bool Utils::GetParamFloatWithMultipleNames(const TiXmlElement *root, float *value, const char *name1, const char *name2, const char *name3, const char *name4, const char *name5)
{
    if (!ReadParamFloatWithMultipleNames(root, value, name1, name2, name3, name4, name5))
    {
        DPRINTF("Cannot read parameter \"%s\"\n", name1);
        return false;
    }

    return true;
}

void Utils::WriteComment(TiXmlElement *root, const std::string & comment)
{
	TiXmlComment *tx_comment = new TiXmlComment();
	
	tx_comment->SetValue(comment);
	root->LinkEndChild(tx_comment);
}

std::string Utils::ModelFileName(uint32_t cms_entry, uint32_t cms_model_spec_idx, const char *ext)
{
    char filename[48];

    snprintf(filename, sizeof(filename), "0x%x_%d%s", cms_entry, cms_model_spec_idx, ext);
    return std::string(filename);
}

#ifdef _WIN32

#include <Wincrypt.h>

uint64_t Utils::RandomInt(uint64_t min, uint64_t max)
{
    HCRYPTPROV hProv;
    int64_t random;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        DPRINTF("%s: CryptAquireContext error\n", FUNCNAME);
        exit(-1);
    }

    if (!CryptGenRandom(hProv, sizeof(uint64_t), (BYTE *)&random))
    {
        DPRINTF("%s: CryptGenRandom error\n", FUNCNAME);
        exit(-1);
    }

    CryptReleaseContext(hProv, 0);

    uint64_t rnd =  random % (max - min);
    return rnd + min;
}

#endif

std::string Utils::GetRandomString(size_t len)
{
    static bool started = false;
    std::string s;

    if (!started)
    {
         std::srand((unsigned int)std::time(0));
         started = true;
    }

    for (unsigned int i = 0; i < len; i++)
    {
        char ch;

        if (std::rand() & 1)
        {
            ch = 'A';
        }
        else
        {
            ch = 'a';
        }

        ch += (std::rand() % 26);
        s += ch;
    }

    return s;
}

static int utf8_to_ucs2(const unsigned char * input, const unsigned char ** end_ptr)
{
    *end_ptr = input;
    if (input[0] == 0) {
        return -1;
    }
    if (input[0] < 0x80) {
	/* One byte (ASCII) case. */
        * end_ptr = input + 1;
        return input[0];
    }
    if ((input[0] & 0xE0) == 0xE0) {
	/* Three byte case. */
        if (input[1] < 0x80 || input[1] > 0xBF ||
	    input[2] < 0x80 || input[2] > 0xBF) {
            return -1;
	}
        * end_ptr = input + 3;
        return
            (input[0] & 0x0F)<<12 |
            (input[1] & 0x3F)<<6  |
            (input[2] & 0x3F);
    }
    if ((input[0] & 0xC0) == 0xC0) {
	/* Two byte case. */
        if (input[1] < 0x80 || input[1] > 0xBF) {
            return -1;
	}
        * end_ptr = input + 2;
        return
            (input[0] & 0x1F)<<6  |
            (input[1] & 0x3F);
    }
    return -1;
}

std::u16string Utils::Utf8ToUcs2(const std::string & utf8)
{
	if (utf8.length() == 0)
    {
        return std::u16string(); // Empty string
    }
	
	const char *c_utf8 = utf8.c_str();
	std::u16string ucs2;
	int ret;
	
	while (*c_utf8 != 0 && (ret = utf8_to_ucs2((const unsigned char *)c_utf8, (const unsigned char **)&c_utf8)) != -1)
	{
		if (ret < 0x10000)
			ucs2.push_back((char16_t)ret);
	}
	
	return ucs2;
}

static int ucs2_to_utf8 (int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
	if (ucs2 >= 0xD800 && ucs2 <= 0xDFFF) {
	    /* Ill-formed. */
	    return -1;
	}
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
        return 3;
    }
    if (ucs2 >= 0x10000 && ucs2 < 0x10FFFF) {
	/* http://tidy.sourceforge.net/cgi-bin/lxr/source/src/utf8.c#L380 */
	utf8[0] = 0xF0 | (ucs2 >> 18);
	utf8[1] = 0x80 | ((ucs2 >> 12) & 0x3F);
	utf8[2] = 0x80 | ((ucs2 >> 6) & 0x3F);
	utf8[3] = 0x80 | ((ucs2 & 0x3F));
        utf8[4] = '\0';
        return 4;
    }
    return -1;;
}

std::string Utils::Ucs2ToUtf8(const std::u16string & ucs2)
{
	if (ucs2.length() == 0)
		return "";
	
	std::string utf8;
	
	for (char16_t c : ucs2)
	{
		char c_utf8[5];
		
		if (ucs2_to_utf8(c, (unsigned char *)c_utf8) < 0)
			break;
		
		utf8 += c_utf8;		
	}
	
	return utf8;
}

static const std::string base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Utils::Base64Encode(const uint8_t *buf, size_t size, bool add_new_line)
{
    std::string ret;
    size_t written = 0;

    if (add_new_line)
        ret += '\n';

    for (size_t i = 0; i < size; i += 3)
    {
        unsigned int st;

        st = (buf[i] & 0xFC) >> 2;
        ret += base64_table[st];
        written++;

        st = (buf[i] & 3) << 4;

        if (i+1 < size)
        {
            st |= (buf[i+1]&0xF0) >> 4;
            ret += base64_table[st];
            written++;

            st = (buf[i+1]&0xF) << 2;

            if (i+2 < size)
            {
                st |= (buf[i+2]&0xC0) >> 6;
                ret += base64_table[st];

                st = buf[i+2]&0x3F;
                ret += base64_table[st];
                written += 2;
            }
            else
            {
                ret += base64_table[st];
                ret += '=';
                written += 2;
            }
        }
        else
        {
            ret += base64_table[st];
            ret += "==";
            written += 3;
        }

        if (((written % 76) == 0) && (i+3 < size))
            ret += '\n';
    }

    if (add_new_line && ret[ret.length()-1] != '\n')
        ret += '\n';

    return ret;
}

uint8_t *Utils::Base64Decode(const std::string &data, size_t *ret_size)
{
    char empty_chars[33];

    for (int c = 1; c < 33; c++)
       empty_chars[c-1] = c;

    empty_chars[32] = 0;

    size_t empty_count = 0;
    size_t pos = data.find_first_of(empty_chars);

    while (pos != std::string::npos)
    {
        empty_count++;
        pos = data.find_first_of(empty_chars, pos+1);
    }

    size_t data_len = data.length() - empty_count;

    if ((data_len % 4) != 0)
        return nullptr;

    *ret_size = (data_len*3) / 4;

    if (data.rfind('=') != std::string::npos)
    {
        size_t pos = data.find_last_not_of(empty_chars);

        if (pos == std::string::npos)
            *ret_size = *ret_size - data.length();
        else
            *ret_size = *ret_size - (pos+1 - data.find('='));
    }

    uint8_t *buf = new uint8_t[*ret_size];

    size_t st[4];
    size_t n = 0;
    size_t i = 0;

    while (i < data.length())
    {        
        if (data[i] <= ' ')
        {
            i++;
            continue;
        }

        for (size_t j = 0; j < 4; j++)
        {
            st[j] = base64_table.find(data[i+j]);
            if (st[j] == std::string::npos)
            {
                if (data[i+j] != '=' || i+j < (data_len - 2))
                {
                    //DPRINTF("Fail: %x\n", data[i+j]);
                    delete[] buf;
                    return nullptr;
                }
            }
        }        

        buf[n++] = (uint8_t) (st[0] << 2) | (uint8_t)(st[1] >> 4);
        if (st[2] != std::string::npos)
        {
            buf[n++] = (uint8_t)(st[1] << 4) | (uint8_t)(st[2] >> 2);
            if (st[3] != std::string::npos)
            {
                buf[n++] = (uint8_t)(st[2] << 6) | (uint8_t)st[3];
            }
        }

        i += 4;
    }

    assert(n == *ret_size);

    return buf;
}

#ifdef NO_CRYPTO

void Utils::Sha1(const void *, uint32_t, uint8_t *)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
}

void Utils::Md5(const void *, uint32_t, uint8_t *)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
}

bool Utils::FileSha1(const std::string &, uint8_t *)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
    return false;
}

bool Utils::FileMd5(const std::string &, uint8_t *)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
    return false;
}

void Utils::PlainAESDecrypt(void *, size_t, const uint8_t *, int)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
}

void Utils::PlainAESEncrypt(void *, size_t, const uint8_t *, int)
{
    DPRINTF("%s: Crypto is not enabled.\n", FUNCNAME);
}

#else

void Utils::Sha1(const void *buf, uint32_t size, uint8_t *result)
{
	SHA1_CTX ctx;
	
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, (const uint8_t *)buf, size);
	SHA1_Final(&ctx, result);
}

void Utils::Md5(const void *buf, uint32_t size, uint8_t *result)
{
	MD5_CTX ctx;
	
	MD5_Init(&ctx);
	MD5_Update(&ctx, buf, size);
	MD5_Final(result, &ctx);
}

bool Utils::FileSha1(const std::string &path, uint8_t *result)
{
	size_t remaining;
	FILE *in;
	SHA1_CTX ctx;
	
	remaining = GetFileSize(path);
	if (remaining == (size_t)-1)
		return false;
	
	SHA1_Init(&ctx);
	
	if (remaining == 0) // Special case, 0 bytes file
	{
		SHA1_Final(&ctx, result);
		return true;
	}
	
	in = fopen(path.c_str(), "rb");
	if (!in)
		return false;	
	
	size_t buf_size = (remaining < FILE_BUFFER_SIZE) ? remaining : FILE_BUFFER_SIZE;
    uint8_t *buf;

    buf = new uint8_t[buf_size];

	while (remaining > 0)
	{
		size_t r = (remaining < buf_size) ? remaining : buf_size;
		
		if (fread(buf, 1, r, in) != r)
		{
			fclose(in);
			delete[] buf;
			return false;
		}
		
        SHA1_Update(&ctx, buf, (uint32_t)r);
		remaining -= r;
	}
	
	fclose(in);
	SHA1_Final(&ctx, result);
	
	return true;
}

bool Utils::FileMd5(const std::string &path, uint8_t *result)
{
	size_t remaining;
	FILE *in;
	MD5_CTX ctx;
	
	remaining = GetFileSize(path);
	if (remaining == (size_t)-1)
		return false;
	
	MD5_Init(&ctx);
	
	if (remaining == 0) // Special case, 0 bytes file
	{
		MD5_Final(result, &ctx);
		return true;
	}
	
	in = fopen(path.c_str(), "rb");
	if (!in)
		return false;	
	
	size_t buf_size = (remaining < FILE_BUFFER_SIZE) ? remaining : FILE_BUFFER_SIZE;
    uint8_t *buf;

    buf = new uint8_t[buf_size];

	while (remaining > 0)
	{
		size_t r = (remaining < buf_size) ? remaining : buf_size;
		
		if (fread(buf, 1, r, in) != r)
		{
			fclose(in);
			delete[] buf;
			return false;
		}
		
        MD5_Update(&ctx, buf, (unsigned long)r);
		remaining -= r;
	}
	
	fclose(in);
	MD5_Final(result, &ctx);
	
	return true;
}

void Utils::PlainAESDecrypt(void *buf, size_t size, const uint8_t *key, int key_size)
{
    if (key_size != 256 && key_size != 128 && key_size != 192)
        return;

    uint32_t rk[RKLENGTH(256)]; // Max size
    static const size_t block_size = 16;

    int nrounds = rijndaelSetupDecrypt(rk, key, key_size);
    size_t num_blocks = size / block_size;
    uint8_t *ptr = (uint8_t *)buf;
    bool last_pass = false;

    if ((size % block_size) != 0)
    {
        num_blocks++;
        last_pass = true;
    }

    for (size_t i = 0; i < num_blocks; i++)
    {
        if ( (i == (num_blocks-1)) && last_pass )
        {
            uint8_t temp[block_size];
            rijndaelDecrypt(rk, nrounds, ptr, temp);

            memcpy(ptr, temp, size % block_size);
        }
        else
        {
            rijndaelDecrypt(rk, nrounds, ptr, ptr);
        }

        ptr += block_size;
    }
}

void Utils::PlainAESEncrypt(void *buf, size_t size, const uint8_t *key, int key_size)
{
    if (key_size != 256 && key_size != 128 && key_size != 192)
        return;

    uint32_t rk[RKLENGTH(256)]; // Max size
    static const size_t block_size = 16;

    int nrounds = rijndaelSetupEncrypt(rk, key, key_size);
    size_t num_blocks = size / block_size;
    uint8_t *ptr = (uint8_t *)buf;

    if ((size % block_size) != 0)
    {
        DPRINTF("%s: Bad function usage. Encryption function requires a size multiple of block_size", FUNCNAME);
        return;
    }

    for (size_t i = 0; i < num_blocks; i++)
    {
        rijndaelEncrypt(rk, nrounds, ptr, ptr);
        ptr += block_size;
    }
}

#endif // NO_CRYPTO

std::string Utils::GetAppData()
{
    char *appdata = getenv("APPDATA");

    if (!appdata)
    {
        DPRINTF("APPDATA doesn't exist in this system.\n");
        throw std::runtime_error("APPDATA doesn't exist in this system.\n");
    }

    return std::string(appdata);
}

#ifdef _WIN32

#include <windows.h>

std::string Utils::GetTempFile(const std::string &prefix, const std::string &extension)
{
    char path[MAX_PATH-14];
    char file[MAX_PATH];

    if (GetTempPathA(sizeof(path), path) == 0)
        return "<NULL>";

    if (GetTempFileNameA(path, prefix.c_str(), (int)RandomInt(0, UINT64_MAX), file) == 0)
        return "<NULL>";

    return std::string(file) + extension;
}

#endif

std::string Utils::GetTempFileLocal(const std::string &extension)
{
    Utils::Mkdir("temp");
    std::string file, ext;

    ext = (Utils::BeginsWith(extension, ".")) ? extension : "." + extension;

    do
    {
        file = "temp/" + ToString(RandomInt(0, UINT64_MAX)) + ToString(RandomInt(0, UINT64_MAX)) + ext;

    } while (Utils::FileExists(file));

    return file;
}

#ifdef _WIN32

int Utils::RunProgram(const std::string &program, const std::vector<std::string> &args)
{
    std::string cmd;

    cmd = program;

    for (const std::string &s : args)
    {
        cmd += " \"" + s + '"';
    }

    PROCESS_INFORMATION process_info;
    STARTUPINFOA startup_info;

    memset(&startup_info, 0, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);

    BOOL result = CreateProcessA(nullptr,
                                 (LPSTR)cmd.c_str(),
                                 nullptr,
                                 nullptr,
                                 FALSE,
                                 NORMAL_PRIORITY_CLASS,
                                 nullptr,
                                 nullptr,
                                 &startup_info,
                                 &process_info);

    if (!result)
        return -1;

    DWORD ret;

    WaitForSingleObject(process_info.hProcess, INFINITE);
    if (!GetExitCodeProcess(process_info.hProcess, &ret))
        ret = -1;

    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    return ret;
}

#else

int Utils::RunProgram(const std::string &program, const std::vector<std::string> &args)
{
    return RunCmd(program, args);
}

#endif


int Utils::RunCmd(const std::string &program, const std::vector<std::string> &args)
{
    std::string cmd;

    cmd = program;

    for (const std::string &s : args)
    {
        cmd += " \"" + s + '"';
    }

    return system(cmd.c_str());
}

#ifdef NO_ZLIB

bool Utils::UncompressZlib(void *, uint32_t *, const void *, uint32_t, int)
{
    DPRINTF("%s: zlib is not enabled.\n", FUNCNAME);
    return false;
}

#else

static void *zalloc(void *opaque, unsigned int size, unsigned int num)
{
    UNUSED(opaque);
    return malloc(size * num);
}

static void zfree(void *opaque, void *p)
{
    UNUSED(opaque);
    free(p);
}

bool Utils::UncompressZlib(void *uncomp_buf, uint32_t *uncomp_size, const void *comp_buf, uint32_t comp_size, int window)
{
    z_stream stream;

    stream.zalloc = &zalloc;
    stream.zfree = &zfree;
    stream.opaque = Z_NULL;
    stream.next_in = (uint8_t *)comp_buf;
    stream.avail_in = comp_size;
    stream.next_out = (uint8_t *)uncomp_buf;
    stream.avail_out = *uncomp_size;

    if (inflateInit2(&stream, window) != Z_OK)
        return false;

    int ret = inflate(&stream, Z_FINISH);
    if (ret == Z_STREAM_END)
    {
        *uncomp_size = stream.total_out;
    }

    ret = inflateEnd(&stream);
    return (ret == Z_OK);
}

#endif
