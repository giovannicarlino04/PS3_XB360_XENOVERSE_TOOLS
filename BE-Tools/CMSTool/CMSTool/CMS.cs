using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace CMSTool
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
        private BinaryReader br;
        private BinaryWriter bw;
        private string FileName;

        public void Load(string path)
        {
            using (br = new BinaryReader(File.Open(path, FileMode.Open)))
            {
                FileName = path;
                br.BaseStream.Seek(8, SeekOrigin.Begin);
                int Count = ReadInt32BigEndian();
                Data = new CMS_Data[Count];
                int Offset = ReadInt32BigEndian();

                for (int i = 0; i < Count; i++)
                {
                    br.BaseStream.Seek(Offset + (80 * i), SeekOrigin.Begin);
                    Data[i].ID = ReadInt32BigEndian();
                    Data[i].ShortName = Encoding.ASCII.GetString(br.ReadBytes(3));
                    br.BaseStream.Seek(9, SeekOrigin.Current);
                    Data[i].Unknown = br.ReadBytes(8);
                    br.BaseStream.Seek(8, SeekOrigin.Current);
                    Data[i].Paths = new string[7];
                    Data[i].Paths[0] = TextAtAddress(ReadInt32BigEndian());
                    Data[i].Paths[1] = TextAtAddress(ReadInt32BigEndian());
                    br.BaseStream.Seek(4, SeekOrigin.Current);
                    Data[i].Paths[2] = TextAtAddress(ReadInt32BigEndian());
                    br.BaseStream.Seek(8, SeekOrigin.Current);
                    Data[i].Paths[3] = TextAtAddress(ReadInt32BigEndian());
                    Data[i].Paths[4] = TextAtAddress(ReadInt32BigEndian());
                    Data[i].Paths[5] = TextAtAddress(ReadInt32BigEndian());
                    Data[i].Paths[6] = TextAtAddress(ReadInt32BigEndian());
                }
            }
        }

        private int ReadInt32BigEndian()
        {
            byte[] bytes = br.ReadBytes(4);
            Array.Reverse(bytes);
            return BitConverter.ToInt32(bytes, 0);
        }

        public string TextAtAddress(int Address)
        {
            long position = br.BaseStream.Position;
            string rText = string.Empty;
            byte[] c;

            if (Address != 0)
            {
                br.BaseStream.Seek(Address, SeekOrigin.Begin);
                do
                {
                    c = br.ReadBytes(1);
                    if (c[0] != 0x00)
                        rText += Encoding.ASCII.GetString(c);
                    else
                        break;
                } while (true);

                br.BaseStream.Seek(position, SeekOrigin.Begin);
            }
            return rText;
        }

        public void Save()
        {
            List<string> CmnText = new List<string>();
            for (int i = 0; i < Data.Length; i++)
            {
                foreach (string path in Data[i].Paths)
                {
                    if (!CmnText.Contains(path))
                        CmnText.Add(path);
                }
            }

            int[] wordAddress = new int[CmnText.Count];
            int wordStartPosition = 16 + (Data.Length * 80);

            using (bw = new BinaryWriter(File.Create(FileName)))
            {
                bw.Write(new byte[] { 0x23, 0x43, 0x4D, 0x53, 0xFF, 0xFE, 0x00, 0x00 });
                bw.Write(Data.Length);
                bw.Write(0x10); // Adjusted to Big Endian if necessary, but usually it's kept as 16 in both endians.
                bw.Seek(wordStartPosition, SeekOrigin.Begin);

                for (int i = 0; i < CmnText.Count; i++)
                {
                    wordAddress[i] = (int)bw.BaseStream.Position;
                    bw.Write(Encoding.ASCII.GetBytes(CmnText[i]));
                    bw.Write((byte)0); // Null terminator
                }

                for (int i = 0; i < Data.Length; i++)
                {
                    bw.BaseStream.Seek(16 + (80 * i), SeekOrigin.Begin);
                    bw.Write(Data[i].ID);
                    bw.Write(Encoding.ASCII.GetBytes(Data[i].ShortName));
                    bw.BaseStream.Seek(9, SeekOrigin.Current);
                    bw.Write(Data[i].Unknown);
                    bw.Write(new byte[] { 0xFF, 0xFF });
                    bw.BaseStream.Seek(6, SeekOrigin.Current);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[0])]);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[1])]);
                    bw.BaseStream.Seek(4, SeekOrigin.Current);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[2])]);
                    bw.BaseStream.Seek(8, SeekOrigin.Current);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[3])]);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[4])]);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[5])]);
                    bw.Write(wordAddress[CmnText.IndexOf(Data[i].Paths[6])]);
                }
            }
        }
    }
}
