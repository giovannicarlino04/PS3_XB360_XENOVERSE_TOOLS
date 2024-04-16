using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net.Security;
using System.Xml.Serialization;
using System.ComponentModel.Design;

namespace XV360Lib
{
    public struct skill
    {
        public string Name;
        public short ID;
    }

    public struct Char_Data
    {
        public int charID;
        public int CostumeID;
        public short[] SuperIDs;
        public short[] UltimateIDs;
        public short EvasiveID;
    }

    public class CUS
    {
        string FileName;
        public skill[] Supers;
        public skill[] Ultimates;
        public skill[] Evasives;
        int CharCount = 0;
        int CharAddress = 0;
        public Char_Data[] Chars;
        public void Load(string CUSFile)
        {
            FileName = CUSFile;
            using (BinaryReader CUS = new BinaryReader(File.Open(CUSFile, FileMode.Open)))
            {
                CUS.BaseStream.Seek(8, SeekOrigin.Begin);
                CharCount = ReverseBytes(CUS.ReadInt32());
                CharAddress = ReverseBytes(CUS.ReadInt32());

                int SuperCount = ReverseBytes(CUS.ReadInt32());
                int UltimateCount = ReverseBytes(CUS.ReadInt32());
                int EvasiveCount = ReverseBytes(CUS.ReadInt32());

                CUS.BaseStream.Seek(8, SeekOrigin.Current);

                int SupAddress = ReverseBytes(CUS.ReadInt32());
                int UltAddress = ReverseBytes(CUS.ReadInt32());
                int EvaAddress = ReverseBytes(CUS.ReadInt32());

                Chars = new Char_Data[CharCount];
                for (int i = 0; i < CharCount; i++)
                {
                    CUS.BaseStream.Seek(CharAddress + (i * 32), SeekOrigin.Begin);
                    Chars[i].charID = ReverseBytes(CUS.ReadInt32());
                    Chars[i].CostumeID = ReverseBytes(CUS.ReadInt32());

                    Chars[i].SuperIDs = new short[4];
                    Chars[i].UltimateIDs = new short[2];

                    Chars[i].SuperIDs[0] = ReverseBytes(CUS.ReadInt16());
                    Chars[i].SuperIDs[1] = ReverseBytes(CUS.ReadInt16());
                    Chars[i].SuperIDs[2] = ReverseBytes(CUS.ReadInt16());
                    Chars[i].SuperIDs[3] = ReverseBytes(CUS.ReadInt16());
                    Chars[i].UltimateIDs[0] = ReverseBytes(CUS.ReadInt16());
                    Chars[i].UltimateIDs[1] = ReverseBytes(CUS.ReadInt16());
                    Chars[i].EvasiveID = ReverseBytes(CUS.ReadInt16());
                }

                Supers = new skill[SuperCount];
                for (int i = 0; i < SuperCount; i++)
                {
                    CUS.BaseStream.Seek(SupAddress + (i * 48) + 8, SeekOrigin.Begin);
                    Supers[i].ID = ReverseBytes(CUS.ReadInt16());
                }

                Ultimates = new skill[UltimateCount];
                for (int i = 0; i < UltimateCount; i++)
                {
                    CUS.BaseStream.Seek(UltAddress + (i * 48) + 8, SeekOrigin.Begin);
                    Ultimates[i].ID = ReverseBytes(CUS.ReadInt16());
                }

                Evasives = new skill[EvasiveCount];
                for (int i = 0; i < EvasiveCount; i++)
                {
                    CUS.BaseStream.Seek(EvaAddress + (i * 48) + 8, SeekOrigin.Begin);
                    Evasives[i].ID = ReverseBytes(CUS.ReadInt16());
                }
            }
        }
        public int FindSuper(short id)
        {
            for (int i = 0; i < Supers.Length; i++)
            {
                if (Supers[i].ID == id)
                    return i;
            }
            return -1;
        }

        public int FindUltimate(short id)
        {
            for (int i = 0; i < Ultimates.Length; i++)
            {
                if (Ultimates[i].ID == id)
                    return i;
            }
            return -1;
        }

        public int FindEvasive(short id)
        {
            for (int i = 0; i < Evasives.Length; i++)
            {
                if (Evasives[i].ID == id)
                    return i;
            }
            return -1;
        }

        public int DataExist(int id, int c)
        {
            for (int i = 0; i < Chars.Length; i++)
            {
                if (Chars[i].charID == id && Chars[i].CostumeID == c)
                    return i;
            }

            return -1;
        }

        public void CUS2XML(string outputFileName, CUS instance)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(CUS));
            using (TextWriter writer = new StreamWriter(outputFileName))
            {
                serializer.Serialize(writer, instance);
            }
        }

        public static byte[] ReverseBytes(byte[] bytes)
        {
            Array.Reverse(bytes);
            return bytes;
        }

        public static int ReverseBytes(int value)
        {
            return BitConverter.ToInt32(ReverseBytes(BitConverter.GetBytes(value)), 0);
        }
        public static short ReverseBytes(short value)
        {
            return BitConverter.ToInt16(ReverseBytes(BitConverter.GetBytes(value)), 0);
        }

    }
}