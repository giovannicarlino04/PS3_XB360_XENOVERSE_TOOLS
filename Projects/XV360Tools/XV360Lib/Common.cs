using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XV360Lib
{
    public class Common
    {
        public static bool isBigEndian(string arg)
        {
            using (BinaryReader br = new BinaryReader(File.Open(arg, FileMode.Open)))
            {
                br.BaseStream.Seek(4, SeekOrigin.Begin);
                byte byte1 = br.ReadByte();
                byte byte2 = br.ReadByte();

                if (byte1 == 0xFF && byte2 == 0xFE)
                {
                    Console.WriteLine($"File {arg} is Big Endian");
                    return true;
                }
                else if (byte1 == 0xFE && byte2 == 0xFF)
                {
                    Console.WriteLine($"File {arg} is Little Endian");
                    return false;
                }
                //00 09 For iggy
                else if (byte1 == 0x00 && byte2 == 0x09)
                {
                    Console.WriteLine($"File {arg} is Big Endian");
                    return true;
                }
                else if (byte1 == 0x09 && byte2 == 0x00)
                {
                    Console.WriteLine($"File {arg} is Little Endian");
                    return false;
                }
                else
                {
                    throw new ArgumentException($"File {arg} not recognized");
                }
            }
        }


    }
}
