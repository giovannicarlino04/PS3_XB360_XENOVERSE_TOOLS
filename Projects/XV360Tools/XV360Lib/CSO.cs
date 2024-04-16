using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Serialization;

namespace XV360Lib
{
    public struct CSO_Data
    {
        public int Char_ID;
        public int Costume_ID;
        public string[] Paths;
    }

    public class CSO
    {
        public CSO_Data[] Data;
        BinaryReader br;
        BinaryWriter bw;
        string FileName;

        static string TextAtAddress(BinaryReader reader, int Address)
        {
            long position = reader.BaseStream.Position;
            string rText = "";
            byte[] c;
            if (Address != 0)
            {
                reader.BaseStream.Seek(Address, SeekOrigin.Begin);
                do
                {
                    c = reader.ReadBytes(1);
                    if (c[0] != 0x00)
                        rText += Encoding.ASCII.GetString(c);
                    else
                        break;
                }
                while (true);

                reader.BaseStream.Seek(position, SeekOrigin.Begin);
            }
            return rText;
        }

        public void Load(string path)
        {
            using (br = new BinaryReader(File.Open(path, FileMode.Open)))
            {
                FileName = path;
                br.BaseStream.Seek(8, SeekOrigin.Begin);
                int Count = ReverseBytes(br.ReadInt32());
                Data = new CSO_Data[Count];
                int Offset = ReverseBytes(br.ReadInt32());

                for (int i = 0; i < Count; i++)
                {
                    br.BaseStream.Seek(Offset + (32 * i), SeekOrigin.Begin);
                    Data[i].Char_ID = ReverseBytes(br.ReadInt32()); // Convert to big endian
                    Data[i].Costume_ID = ReverseBytes(br.ReadInt32()); // Convert to big endian
                    Data[i].Paths = new string[4];
                    Data[i].Paths[0] = TextAtAddress(br, ReverseBytes(br.ReadInt32())); // Convert to big endian
                    Data[i].Paths[1] = TextAtAddress(br, ReverseBytes(br.ReadInt32())); // Convert to big endian
                    Data[i].Paths[2] = TextAtAddress(br, ReverseBytes(br.ReadInt32())); // Convert to big endian
                    Data[i].Paths[3] = TextAtAddress(br, ReverseBytes(br.ReadInt32())); // Convert to big endian
                }
            }
        }

        public void Save(string outputFileName)
        {
            List<string> CmnText = new List<string>();
            for (int i = 0; i < Data.Length; i++)
            {
                for (int j = 0; j < Data[i].Paths.Length; j++)
                {
                    if (!CmnText.Contains(Data[i].Paths[j]))
                        CmnText.Add(Data[i].Paths[j]);
                }
            }

            int[] wordAddress = new int[CmnText.Count];
            int wordstartposition = 16 + (Data.Length * 32);
            using (bw = new BinaryWriter(File.Create(outputFileName)))
            {
                bw.Write(new byte[] { 0x23, 0x43, 0x53, 0x4F, 0xFF, 0xFE, 0x00, 0x00 });
                bw.Write(ReverseBytes(Data.Length)); // Convert to big endian
                bw.Write(ReverseBytes((int)16)); // Convert to big endian
                bw.Seek(wordstartposition, SeekOrigin.Begin);
                for (int i = 0; i < CmnText.Count; i++)
                {
                    wordAddress[i] = (int)bw.BaseStream.Position;
                    bw.Write(Encoding.ASCII.GetBytes(CmnText[i]));
                    bw.Write((byte)0);
                }

                for (int i = 0; i < Data.Length; i++)
                {
                    bw.BaseStream.Seek(16 + (32 * i), SeekOrigin.Begin);
                    bw.Write(ReverseBytes(Data[i].Char_ID)); // Convert to big endian
                    bw.Write(ReverseBytes(Data[i].Costume_ID)); // Convert to big endian
                    bw.Write(ReverseBytes(wordAddress[CmnText.IndexOf(Data[i].Paths[0])])); // Convert to big endian
                    bw.Write(ReverseBytes(wordAddress[CmnText.IndexOf(Data[i].Paths[1])])); // Convert to big endian
                    bw.Write(ReverseBytes(wordAddress[CmnText.IndexOf(Data[i].Paths[2])])); // Convert to big endian
                    bw.Write(ReverseBytes(wordAddress[CmnText.IndexOf(Data[i].Paths[3])])); // Convert to big endian
                }
            }
        }

        public int DataExist(int id, int c)
        {
            for (int i = 0; i < Data.Length; i++)
            {
                if (Data[i].Char_ID == id && Data[i].Costume_ID == c)
                    return i;
            }

            for (int j = 0; j < Data.Length; j++)
            {
                if (Data[j].Char_ID == id)
                    return j;
            }

            return -1;
        }
        public void AddCharacter(int CharID, int CostID, string[] Paths)
        {
            CSO_Data newChar = new CSO_Data
            {
                Char_ID = CharID,
                Costume_ID = CostID,
                Paths = Paths
            };

            Array.Resize(ref Data, Data.Length + 1);
            Data[Data.Length - 1] = newChar;

        }
        public void RemoveCharacter(int id)
        {
            // Find the index of the model with the specified ID
            int indexToRemove = -1;
            for (int i = 0; i < Data.Length; i++)
            {
                if (Data[i].Char_ID == id)
                {
                    indexToRemove = i;
                    break;
                }
            }

            // If the model was found, remove it from the array
            if (indexToRemove != -1)
            {
                // Shift elements to the left to fill the gap
                for (int i = indexToRemove; i < Data.Length - 1; i++)
                {
                    Data[i] = Data[i + 1];
                }

                // Resize the array to remove the last element
                Array.Resize(ref Data, Data.Length - 1);
            }
        }
        private int ReverseInt32(long value)
        {
            return (int)((value & 0x000000FF) << 24 |
                   (value & 0x0000FF00) << 8 |
                   (value & 0x00FF0000) >> 8 |
                   (value & 0xFF000000) >> 24);
        }
        private int ReverseBytesInt32(int value)
        {
            return BitConverter.ToInt32(ReverseBytes(BitConverter.GetBytes(value)), 0);
        }

        private byte[] ReverseBytes(byte[] bytes)
        {
            Array.Reverse(bytes);
            return bytes;
        }

        private int ReverseBytes(int value)
        {
            return BitConverter.ToInt32(ReverseBytes(BitConverter.GetBytes(value)), 0);
        }

        public void CSO2XML(string outputFileName, CSO instance)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(CSO));
            using (TextWriter writer = new StreamWriter(outputFileName))
            {
                serializer.Serialize(writer, instance);
            }
        }

        public static CSO XML2CSO(string xmlFileName, string outputFileName)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(CSO));
            using (TextReader reader = new StreamReader(xmlFileName))
            {
                CSO parsedObject = (CSO)serializer.Deserialize(reader);
                parsedObject.Save(outputFileName);

                return parsedObject;
            }
        }
    }
}