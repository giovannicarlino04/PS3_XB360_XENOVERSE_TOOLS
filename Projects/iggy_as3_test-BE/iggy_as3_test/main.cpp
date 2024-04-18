#include "IggyFile.h"
#include "SwfFile.h"
#include "debug.h"

#include "BitStream.h"

static const std::vector<std::string> exceptions =
{
    "KOKUZOUKO.iggy",
    "LOBYMENU.iggy",
    "LOBYMENU_BG.iggy",
    "RESULT.iggy",
    "RESULT_JE.iggy",
};

static bool rt_visitor(const std::string &path, bool, void *)
{
    if (Utils::EndsWith(path, ".iggy", false))
    {
        for (auto &str : exceptions)
        {
            if (Utils::EndsWith(path, str, false))
                return true;
        }


        uint8_t *sig = Utils::ReadFileFrom(path, 0, 4);
        if (*(uint32_t *)sig != IGGY_SIGNATURE)
        {
            DPRINTF("no sig???");
            delete[] sig;
            return true;
        }

        delete[] sig;

        IggyFile iggy;

        UPRINTF("Testing %s\n", path.c_str());

        if (!iggy.LoadFromFile(path))
            return false;

        if (!iggy.SaveToFile("test.iggy"))
            return false;

        if (!Utils::CompareFiles(path, "test.iggy"))
        {
            DPRINTF("Comparison failed at %s\n", path.c_str());
            return false;
        }
        else
        {
            UPRINTF("File is OK.\n");
        }
    }

    return true;
}

void recompose_test()
{
    if (!Utils::VisitDirectory("test", true, false, true, rt_visitor, nullptr))
    {
        UPRINTF("Test failed.\n");
    }
    else
    {
        UPRINTF("Test success.\n");
    }
}
void extract_as3_from_iggy(const std::string &input, const std::string &output)
{
    IggyFile iggy;

    if (!iggy.LoadFromFile(input))
    {
        DPRINTF("Iggy load failed.\n");
        return;
    }
    // Print significant values after loading the Iggy file
    DPRINTF("Iggy loaded successfully from file: %s\n", input.c_str());

    uint16_t abc_size;
    uint8_t *abc = iggy.GetAbcBlob(&abc_size);

    if (!abc)
    {
        DPRINTF("That iggy file doesn't have ActionScript 3 data.\n");
        return;
    }

    DPRINTF("ABC Size: %u\n", abc_size); // Correct way to print abc_size
    DPRINTF("ABC Address: %p\n", static_cast<void*>(abc)); // Correct way to print abc address

    if (Utils::EndsWith(output, ".abc", false))
    {
        if (Utils::WriteFileBool(output, abc, abc_size))
        {
            UPRINTF("ActionScript 3 data extracted successfully.\n");
        }
    }
    else
    {
        SwfFile swf;
        swf.SetGfx(Utils::EndsWith(output, ".gfx", false));

        SwfFileAttributes *attr = new SwfFileAttributes();
        attr->as3 = 1;

        swf.AddBlock(attr);
        swf.AddBlock(new SwfDoABC(abc, abc_size, 1));
        swf.AddBlock(new SwfEnd());

        if (swf.SaveToFile(output))
            UPRINTF("ActionScript 3 data extracted successfully.\n");
        else
            DPRINTF("Save swf failed.\n");
    }

    delete[] abc; // Don't forget to free the memory allocated for abc
}

void extract_as3_from_swf(const std::string &input, const std::string &output)
{
    SwfFile swf;

    if (!swf.LoadFromFile(input))
    {
        DPRINTF("Swf load failed.\n");
        return;
    }

    for (uint32_t i = 0; i < swf.GetNumBlocks(); i++)
    {
        SwfDoABC *abc_block = dynamic_cast<SwfDoABC *>(swf.GetBlock(i));

        if (abc_block)
        {
            if (Utils::WriteFileBool(output, abc_block->abc.data(), abc_block->abc.size()))
                UPRINTF("ActionScript3 data extracted succesfully.\n");

            return;
        }
    }

    DPRINTF("That swf file doesn't have ActionSript 3 data.\n");
}

void inject_as3_to_iggy(const std::string &input, const std::string &inout)
{
    uint8_t *abc = nullptr;
    uint32_t abc_size = 0;

    if (Utils::EndsWith(input, ".abc", false))
    {
        size_t size;
        abc = Utils::ReadFile(input, &size);

        if (!abc)
            return;

        abc_size = (uint32_t)size;
    }
    else
    {
        SwfFile swf;

        if (!swf.LoadFromFile(input))
        {
            DPRINTF("Swf load failed.\n");
            return;
        }

        for (uint32_t i = 0; i < swf.GetNumBlocks(); i++)
        {
            SwfDoABC *abc_block = dynamic_cast<SwfDoABC *>(swf.GetBlock(i));

            if (abc_block)
            {
                abc_size = abc_block->abc.size();
                abc = new uint8_t[abc_size];
                memcpy(abc, abc_block->abc.data(), abc_size);
                break;
            }
        }

        if (!abc)
        {
            DPRINTF("That swf file doesn't have ActionSript 3 data.\n");
            return;
        }
    }

    IggyFile iggy;    

    if (!iggy.LoadFromFile(inout))
    {
        DPRINTF("Iggy load failed.\n");
        delete[] abc;
        return;
    }

    if (!iggy.SetAbcBlob(abc, abc_size))
    {
        DPRINTF("SetAbcBlob failed.\n");
        delete[] abc;
        return;
    }

    if (iggy.SaveToFile(inout))
        UPRINTF("ActionScrip3 data injected succesfully.\n");
    else
        DPRINTF("Save iggy failed.\n");

    delete[] abc;
}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        DPRINTF("Wrong usage.\n");
        DPRINTF("Usage for extract: %s input.iggy [output.swf|abc|gfx]\n", argv[0]);
        DPRINTF("Usage for inject: %s input.swf|abc|gfx [inout.iggy]\n", argv[0]);
        DPRINTF("Optional params are guessed if not specified.\n");

        return -1;
    }

    std::string input = argv[1];

    if (Utils::EndsWith(input, ".iggy", false))
    {
        std::string output;

        if (argc >= 3)
        {
            output = argv[2];

            if (!Utils::EndsWith(output, ".swf", false) && !Utils::EndsWith(output, ".gfx", false) && !Utils::EndsWith(output, ".abc", false))
            {
                DPRINTF("Extension of output file must be swf, gfx or abc.\n");
                goto wait;
            }
        }
        else
        {
            output = input.substr(0, input.length()-4) + "swf";
        }

        extract_as3_from_iggy(input, output);
    }
    else if (Utils::EndsWith(input, ".swf", false) || Utils::EndsWith(input, ".gfx", false) || Utils::EndsWith(input, ".abc", false))
    {
        std::string inout;

        if (argc >= 3)
        {
            inout = argv[2];

            if (!Utils::EndsWith(inout, ".iggy", false))
            {
                DPRINTF("Extension of inout file must be iggy.\n");
                goto wait;
            }
        }
        else
        {
            inout = input.substr(0, input.length()-3) + "iggy";
        }

        inject_as3_to_iggy(input, inout);
    }
    else
    {
        DPRINTF("Extension of input file must be iggy, swf, gfx or abc.\n");
    }

wait:

    UPRINTF("Press enter to exit.");
    getchar();

    return 0;
}
