using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;

namespace XV360Lib
{
    public struct CMS_Data
    {
        public int ID;
        public string ShortName;
        public byte[] Unknown;
        public string[] Paths;
    }

    public class CMS
    {
        public CMS_Data[] Data;
        BinaryReader br;
        BinaryWriter bw;
        string FileName;

        public void Load(string path)
        {
            using (br = new BinaryReader(File.Open(path, FileMode.Open)))
            {
                FileName = path;
                br.BaseStream.Seek(8, SeekOrigin.Begin);
                int Count = ReverseBytes(br.ReadInt32());
                Data = new CMS_Data[Count];
                int Offset = ReverseBytes(br.ReadInt32());

                for (int i = 0; i < Count; i++)
                {
                    br.BaseStream.Seek(Offset + (80 * i), SeekOrigin.Begin);
                    Data[i].ID = ReverseBytes(br.ReadInt32());
                    Data[i].ShortName = ReadString(3, br);
                    br.BaseStream.Seek(9, SeekOrigin.Current);
                    Data[i].Unknown = br.ReadBytes(8);
                    br.BaseStream.Seek(8, SeekOrigin.Current);
                    Data[i].Paths = new string[7];
                    Data[i].Paths[0] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                    Data[i].Paths[1] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                    br.BaseStream.Seek(4, SeekOrigin.Current);
                    Data[i].Paths[2] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                    br.BaseStream.Seek(8, SeekOrigin.Current);
                    Data[i].Paths[3] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                    Data[i].Paths[4] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                    Data[i].Paths[5] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                    Data[i].Paths[6] = TextAtAddress(ReverseBytes(br.ReadInt32()));
                }
            }
        }

        public string TextAtAddress(int Address)
        {
            long position = br.BaseStream.Position;
            string rText = "";
            byte[] c;
            if (Address != 0)
            {
                br.BaseStream.Seek(Address, SeekOrigin.Begin);
                do
                {
                    c = br.ReadBytes(1);
                    if (c[0] != 0x00)
                        rText += System.Text.Encoding.ASCII.GetString(c);
                    else
                        break;
                } while (true);

                br.BaseStream.Seek(position, SeekOrigin.Begin);
            }
            return rText;
        }

        public void Save(string OutputFileName)
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
            int wordstartposition = 16 + (Data.Length * 80);
            using (bw = new BinaryWriter(File.Create(OutputFileName)))
            {
                bw.Write(new byte[] { 0x23, 0x43, 0x4D, 0x53, 0xFF, 0xFE, 0x00, 0x00 });
                bw.Write(ReverseBytes(Data.Length));
                bw.Write(ReverseBytes((int)16));
                bw.Seek(wordstartposition, SeekOrigin.Begin);
                for (int i = 0; i < CmnText.Count; i++)
                {
                    wordAddress[i] = ReverseBytes((int)bw.BaseStream.Position);
                    bw.Write(System.Text.Encoding.ASCII.GetBytes(CmnText[i]));
                    bw.Write((byte)0);
                }

                for (int i = 0; i < Data.Length; i++)
                {
                    bw.BaseStream.Seek(16 + (80 * i), SeekOrigin.Begin);
                    bw.Write(ReverseBytes(Data[i].ID));
                    bw.Write(System.Text.Encoding.ASCII.GetBytes(Data[i].ShortName));
                    bw.BaseStream.Seek(9, SeekOrigin.Current);
                    bw.Write(Data[i].Unknown);
                    bw.Write(new byte[] { 0xFF, 0xFF });
                    bw.BaseStream.Seek(6, SeekOrigin.Current);
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[0])]));
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[1])]));
                    bw.BaseStream.Seek(4, SeekOrigin.Current);
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[2])]));
                    bw.BaseStream.Seek(8, SeekOrigin.Current);
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[3])]));
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[4])]));
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[5])]));
                    bw.Write((wordAddress[CmnText.IndexOf(Data[i].Paths[6])]));
                }
            }
        }


        private static int ReverseBytes(int value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            Array.Reverse(bytes);
            return BitConverter.ToInt32(bytes, 0);
        }

        private static string ReadString(int length, BinaryReader reader)
        {
            byte[] buffer = reader.ReadBytes(length);
            return System.Text.Encoding.ASCII.GetString(buffer);
        }

        public void AddModel(int id, string shortName, byte[] unknown, string[] paths)
        {
            CMS_Data newModel = new CMS_Data
            {
                ID = id,
                ShortName = shortName,
                Unknown = unknown,
                Paths = paths
            };

            Array.Resize(ref Data, Data.Length + 1);
            Data[Data.Length - 1] = newModel;
            
        }
        public void RemoveModel(int id)
        {
            int indexToRemove = -1;
            for (int i = 0; i < Data.Length; i++)
            {
                if (Data[i].ID == id)
                {
                    indexToRemove = i;
                    break;
                }
            }

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
        public void CMS2XML(string outputFileName, CMS instance)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(CMS));
            using (TextWriter writer = new StreamWriter(outputFileName))
            {
                serializer.Serialize(writer, instance);
            }
        }

        public static CMS XML2CMS(string xmlFileName, string outputFileName)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(CMS));
            using (TextReader reader = new StreamReader(xmlFileName))
            {
                CMS parsedObject = (CMS)serializer.Deserialize(reader);
                parsedObject.Save(outputFileName);

                return parsedObject;
            }
        }

    }
}
