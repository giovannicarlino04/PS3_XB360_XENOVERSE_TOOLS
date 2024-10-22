// Decompiled with JetBrains decompiler
// Type: XV360Lib.CMS
// Assembly: XV360Lib, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: B9D187B2-9B90-4A2F-B236-CD1D7F0B7832
// Assembly location: C:\Users\Giovanni\Desktop\Repositories\PS3_XB360_XENOVERSE_TOOLS\LibXenoverse_BE_Build\XV360Lib.dll

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Serialization;


namespace XV360Lib
{
  public class CMS
  {
    public CMS_Data[] Data;
    private BinaryReader br;
    private BinaryWriter bw;
    private string FileName;

    public void LoadBE(string path)
    {
      using (this.br = new BinaryReader((Stream) File.Open(path, FileMode.Open)))
      {
        this.FileName = path;
        this.br.BaseStream.Seek(8L, SeekOrigin.Begin);
        int length = CMS.ReverseBytes(this.br.ReadInt32());
        this.Data = new CMS_Data[length];
        int num = CMS.ReverseBytes(this.br.ReadInt32());
        for (int index = 0; index < length; ++index)
        {
          this.br.BaseStream.Seek((long) (num + 80 * index), SeekOrigin.Begin);
          this.Data[index].ID = CMS.ReverseBytes(this.br.ReadInt32());
          this.Data[index].ShortName = CMS.ReadString(3, this.br);
          this.br.BaseStream.Seek(9L, SeekOrigin.Current);
          this.Data[index].Unknown = this.br.ReadBytes(8);
          this.br.BaseStream.Seek(8L, SeekOrigin.Current);
          this.Data[index].Paths = new string[7];
          this.Data[index].Paths[0] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[1] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
          this.br.BaseStream.Seek(4L, SeekOrigin.Current);
          this.Data[index].Paths[2] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
          this.br.BaseStream.Seek(8L, SeekOrigin.Current);
          this.Data[index].Paths[3] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[4] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[5] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[6] = this.TextAtAddress(CMS.ReverseBytes(this.br.ReadInt32()));
        }
      }
    }

    public void SaveBE(string OutputFileName)
    {
      List<string> stringList = new List<string>();
      for (int index1 = 0; index1 < this.Data.Length; ++index1)
      {
        for (int index2 = 0; index2 < this.Data[index1].Paths.Length; ++index2)
        {
          if (!stringList.Contains(this.Data[index1].Paths[index2]))
            stringList.Add(this.Data[index1].Paths[index2]);
        }
      }
      int[] numArray = new int[stringList.Count];
      int offset = 16 + this.Data.Length * 80;
      using (this.bw = new BinaryWriter((Stream) File.Create(OutputFileName)))
      {
        this.bw.Write(new byte[8]
        {
          (byte) 35,
          (byte) 67,
          (byte) 77,
          (byte) 83,
          byte.MaxValue,
          (byte) 254,
          (byte) 0,
          (byte) 0
        });
        this.bw.Write(CMS.ReverseBytes(this.Data.Length));
        this.bw.Write(CMS.ReverseBytes(16));
        this.bw.Seek(offset, SeekOrigin.Begin);
        for (int index = 0; index < stringList.Count; ++index)
        {
          numArray[index] = CMS.ReverseBytes((int) this.bw.BaseStream.Position);
          this.bw.Write(Encoding.ASCII.GetBytes(stringList[index]));
          this.bw.Write((byte) 0);
        }
        for (int index = 0; index < this.Data.Length; ++index)
        {
          this.bw.BaseStream.Seek((long) (16 + 80 * index), SeekOrigin.Begin);
          this.bw.Write(CMS.ReverseBytes(this.Data[index].ID));
          this.bw.Write(Encoding.ASCII.GetBytes(this.Data[index].ShortName));
          this.bw.BaseStream.Seek(9L, SeekOrigin.Current);
          this.bw.Write(this.Data[index].Unknown);
          this.bw.Write(new byte[2]
          {
            byte.MaxValue,
            byte.MaxValue
          });
          this.bw.BaseStream.Seek(6L, SeekOrigin.Current);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[0])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[1])]);
          this.bw.BaseStream.Seek(4L, SeekOrigin.Current);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[2])]);
          this.bw.BaseStream.Seek(8L, SeekOrigin.Current);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[3])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[4])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[5])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[6])]);
        }
      }
    }

    public void LoadLE(string path)
    {
      using (this.br = new BinaryReader((Stream) File.Open(path, FileMode.Open)))
      {
        this.FileName = path;
        this.br.BaseStream.Seek(8L, SeekOrigin.Begin);
        int length = this.br.ReadInt32();
        this.Data = new CMS_Data[length];
        int num = this.br.ReadInt32();
        for (int index = 0; index < length; ++index)
        {
          this.br.BaseStream.Seek((long) (num + 80 * index), SeekOrigin.Begin);
          this.Data[index].ID = this.br.ReadInt32();
          this.Data[index].ShortName = CMS.ReadString(3, this.br);
          this.br.BaseStream.Seek(9L, SeekOrigin.Current);
          this.Data[index].Unknown = this.br.ReadBytes(8);
          this.br.BaseStream.Seek(8L, SeekOrigin.Current);
          this.Data[index].Paths = new string[7];
          this.Data[index].Paths[0] = this.TextAtAddress(this.br.ReadInt32());
          this.Data[index].Paths[1] = this.TextAtAddress(this.br.ReadInt32());
          this.br.BaseStream.Seek(4L, SeekOrigin.Current);
          this.Data[index].Paths[2] = this.TextAtAddress(this.br.ReadInt32());
          this.br.BaseStream.Seek(8L, SeekOrigin.Current);
          this.Data[index].Paths[3] = this.TextAtAddress(this.br.ReadInt32());
          this.Data[index].Paths[4] = this.TextAtAddress(this.br.ReadInt32());
          this.Data[index].Paths[5] = this.TextAtAddress(this.br.ReadInt32());
          this.Data[index].Paths[6] = this.TextAtAddress(this.br.ReadInt32());
        }
      }
    }

    public void SaveLE(string OutputFileName)
    {
      List<string> stringList = new List<string>();
      for (int index1 = 0; index1 < this.Data.Length; ++index1)
      {
        for (int index2 = 0; index2 < this.Data[index1].Paths.Length; ++index2)
        {
          if (!stringList.Contains(this.Data[index1].Paths[index2]))
            stringList.Add(this.Data[index1].Paths[index2]);
        }
      }
      int[] numArray = new int[stringList.Count];
      int offset = 16 + this.Data.Length * 80;
      using (this.bw = new BinaryWriter((Stream) File.Create(OutputFileName)))
      {
        this.bw.Write(new byte[8]
        {
          (byte) 35,
          (byte) 67,
          (byte) 77,
          (byte) 83,
          (byte) 254,
          byte.MaxValue,
          (byte) 0,
          (byte) 0
        });
        this.bw.Write(this.Data.Length);
        this.bw.Write(16);
        this.bw.Seek(offset, SeekOrigin.Begin);
        for (int index = 0; index < stringList.Count; ++index)
        {
          numArray[index] = (int) this.bw.BaseStream.Position;
          this.bw.Write(Encoding.ASCII.GetBytes(stringList[index]));
          this.bw.Write((byte) 0);
        }
        for (int index = 0; index < this.Data.Length; ++index)
        {
          this.bw.BaseStream.Seek((long) (16 + 80 * index), SeekOrigin.Begin);
          this.bw.Write(this.Data[index].ID);
          this.bw.Write(Encoding.ASCII.GetBytes(this.Data[index].ShortName));
          this.bw.BaseStream.Seek(9L, SeekOrigin.Current);
          this.bw.Write(this.Data[index].Unknown);
          this.bw.Write(new byte[2]
          {
            byte.MaxValue,
            byte.MaxValue
          });
          this.bw.BaseStream.Seek(6L, SeekOrigin.Current);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[0])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[1])]);
          this.bw.BaseStream.Seek(4L, SeekOrigin.Current);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[2])]);
          this.bw.BaseStream.Seek(8L, SeekOrigin.Current);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[3])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[4])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[5])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[6])]);
        }
      }
    }

    public string TextAtAddress(int Address)
    {
      long position = this.br.BaseStream.Position;
      string str = "";
      if (Address != 0)
      {
        this.br.BaseStream.Seek((long) Address, SeekOrigin.Begin);
        while (true)
        {
          byte[] bytes = this.br.ReadBytes(1);
          if (bytes[0] > (byte) 0)
            str += Encoding.ASCII.GetString(bytes);
          else
            break;
        }
        this.br.BaseStream.Seek(position, SeekOrigin.Begin);
      }
      return str;
    }

    private static int ReverseBytes(int value)
    {
      byte[] bytes = BitConverter.GetBytes(value);
      Array.Reverse(bytes);
      return BitConverter.ToInt32(bytes, 0);
    }

    private static string ReadString(int length, BinaryReader reader)
    {
      return Encoding.ASCII.GetString(reader.ReadBytes(length));
    }

    public void AddModel(int id, string shortName, byte[] unknown, string[] paths)
    {
      CMS_Data cmsData = new CMS_Data()
      {
        ID = id,
        ShortName = shortName,
        Unknown = unknown,
        Paths = paths
      };
      Array.Resize<CMS_Data>(ref this.Data, this.Data.Length + 1);
      this.Data[this.Data.Length - 1] = cmsData;
    }

    public void RemoveModel(int id)
    {
      int num = -1;
      for (int index = 0; index < this.Data.Length; ++index)
      {
        if (this.Data[index].ID == id)
        {
          num = index;
          break;
        }
      }
      if (num == -1)
        return;
      for (int index = num; index < this.Data.Length - 1; ++index)
        this.Data[index] = this.Data[index + 1];
      Array.Resize<CMS_Data>(ref this.Data, this.Data.Length - 1);
    }

    public void CMS2XML(string outputFileName, CMS instance)
    {
      using (TextWriter textWriter = (TextWriter) new StreamWriter(outputFileName))
        new XmlSerializer(typeof (CMS)).Serialize(textWriter, (object) instance);
    }

    public static CMS XML2CMS(string xmlFileName, string outputFileName, bool big_endian)
    {
      using (TextReader textReader = (TextReader) new StreamReader(xmlFileName))
      {
        CMS cms = (CMS) new XmlSerializer(typeof (CMS)).Deserialize(textReader);
        if (big_endian)
          cms.SaveBE(outputFileName);
        else
          cms.SaveLE(outputFileName);
        return cms;
      }
    }
  }
}
