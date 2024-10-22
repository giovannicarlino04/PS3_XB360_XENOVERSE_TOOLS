// Decompiled with JetBrains decompiler
// Type: XV360Lib.CSO
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
  public class CSO
  {
    public CSO_Data[] Data;
    private BinaryReader br;
    private BinaryWriter bw;
    private string FileName;

    public void LoadBE(string path)
    {
      using (this.br = new BinaryReader((Stream) File.Open(path, FileMode.Open)))
      {
        this.FileName = path;
        this.br.BaseStream.Seek(8L, SeekOrigin.Begin);
        int length = this.ReverseBytes(this.br.ReadInt32());
        this.Data = new CSO_Data[length];
        int num = this.ReverseBytes(this.br.ReadInt32());
        for (int index = 0; index < length; ++index)
        {
          this.br.BaseStream.Seek((long) (num + 32 * index), SeekOrigin.Begin);
          this.Data[index].Char_ID = this.ReverseBytes(this.br.ReadInt32());
          this.Data[index].Costume_ID = this.ReverseBytes(this.br.ReadInt32());
          this.Data[index].Paths = new string[4];
          this.Data[index].Paths[0] = CSO.TextAtAddress(this.br, this.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[1] = CSO.TextAtAddress(this.br, this.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[2] = CSO.TextAtAddress(this.br, this.ReverseBytes(this.br.ReadInt32()));
          this.Data[index].Paths[3] = CSO.TextAtAddress(this.br, this.ReverseBytes(this.br.ReadInt32()));
        }
      }
    }

    public void SaveBE(string outputFileName)
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
      int offset = 16 + this.Data.Length * 32;
      using (this.bw = new BinaryWriter((Stream) File.Create(outputFileName)))
      {
        this.bw.Write(new byte[8]
        {
          (byte) 35,
          (byte) 67,
          (byte) 83,
          (byte) 79,
          byte.MaxValue,
          (byte) 254,
          (byte) 0,
          (byte) 0
        });
        this.bw.Write(this.ReverseBytes(this.Data.Length));
        this.bw.Write(this.ReverseBytes(16));
        this.bw.Seek(offset, SeekOrigin.Begin);
        for (int index = 0; index < stringList.Count; ++index)
        {
          numArray[index] = (int) this.bw.BaseStream.Position;
          this.bw.Write(Encoding.ASCII.GetBytes(stringList[index]));
          this.bw.Write((byte) 0);
        }
        for (int index = 0; index < this.Data.Length; ++index)
        {
          this.bw.BaseStream.Seek((long) (16 + 32 * index), SeekOrigin.Begin);
          this.bw.Write(this.ReverseBytes(this.Data[index].Char_ID));
          this.bw.Write(this.ReverseBytes(this.Data[index].Costume_ID));
          this.bw.Write(this.ReverseBytes(numArray[stringList.IndexOf(this.Data[index].Paths[0])]));
          this.bw.Write(this.ReverseBytes(numArray[stringList.IndexOf(this.Data[index].Paths[1])]));
          this.bw.Write(this.ReverseBytes(numArray[stringList.IndexOf(this.Data[index].Paths[2])]));
          this.bw.Write(this.ReverseBytes(numArray[stringList.IndexOf(this.Data[index].Paths[3])]));
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
        this.Data = new CSO_Data[length];
        int num = this.br.ReadInt32();
        for (int index = 0; index < length; ++index)
        {
          this.br.BaseStream.Seek((long) (num + 32 * index), SeekOrigin.Begin);
          this.Data[index].Char_ID = this.br.ReadInt32();
          this.Data[index].Costume_ID = this.br.ReadInt32();
          this.Data[index].Paths = new string[4];
          this.Data[index].Paths[0] = CSO.TextAtAddress(this.br, this.br.ReadInt32());
          this.Data[index].Paths[1] = CSO.TextAtAddress(this.br, this.br.ReadInt32());
          this.Data[index].Paths[2] = CSO.TextAtAddress(this.br, this.br.ReadInt32());
          this.Data[index].Paths[3] = CSO.TextAtAddress(this.br, this.br.ReadInt32());
        }
      }
    }

    public void SaveLE(string outputFileName)
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
      int offset = 16 + this.Data.Length * 32;
      using (this.bw = new BinaryWriter((Stream) File.Create(outputFileName)))
      {
        this.bw.Write(new byte[8]
        {
          (byte) 35,
          (byte) 67,
          (byte) 83,
          (byte) 79,
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
          this.bw.BaseStream.Seek((long) (16 + 32 * index), SeekOrigin.Begin);
          this.bw.Write(this.Data[index].Char_ID);
          this.bw.Write(this.Data[index].Costume_ID);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[0])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[1])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[2])]);
          this.bw.Write(numArray[stringList.IndexOf(this.Data[index].Paths[3])]);
        }
      }
    }

    private static string TextAtAddress(BinaryReader reader, int Address)
    {
      long position = reader.BaseStream.Position;
      string str = "";
      if (Address != 0)
      {
        reader.BaseStream.Seek((long) Address, SeekOrigin.Begin);
        while (true)
        {
          byte[] bytes = reader.ReadBytes(1);
          if (bytes[0] > (byte) 0)
            str += Encoding.ASCII.GetString(bytes);
          else
            break;
        }
        reader.BaseStream.Seek(position, SeekOrigin.Begin);
      }
      return str;
    }

    public int DataExist(int id, int c)
    {
      for (int index = 0; index < this.Data.Length; ++index)
      {
        if (this.Data[index].Char_ID == id && this.Data[index].Costume_ID == c)
          return index;
      }
      for (int index = 0; index < this.Data.Length; ++index)
      {
        if (this.Data[index].Char_ID == id)
          return index;
      }
      return -1;
    }

    public void AddCharacter(int CharID, int CostID, string[] Paths)
    {
      CSO_Data csoData = new CSO_Data()
      {
        Char_ID = CharID,
        Costume_ID = CostID,
        Paths = Paths
      };
      Array.Resize<CSO_Data>(ref this.Data, this.Data.Length + 1);
      this.Data[this.Data.Length - 1] = csoData;
    }

    public void RemoveCharacter(int id)
    {
      int num = -1;
      for (int index = 0; index < this.Data.Length; ++index)
      {
        if (this.Data[index].Char_ID == id)
        {
          num = index;
          break;
        }
      }
      if (num == -1)
        return;
      for (int index = num; index < this.Data.Length - 1; ++index)
        this.Data[index] = this.Data[index + 1];
      Array.Resize<CSO_Data>(ref this.Data, this.Data.Length - 1);
    }

    private int ReverseInt32(long value)
    {
      return (int) ((value & (long) byte.MaxValue) << 24 | (value & 65280L) << 8 | (value & 16711680L) >> 8 | (value & 4278190080L) >> 24);
    }

    private int ReverseBytesInt32(int value)
    {
      return BitConverter.ToInt32(this.ReverseBytes(BitConverter.GetBytes(value)), 0);
    }

    private byte[] ReverseBytes(byte[] bytes)
    {
      Array.Reverse(bytes);
      return bytes;
    }

    private int ReverseBytes(int value)
    {
      return BitConverter.ToInt32(this.ReverseBytes(BitConverter.GetBytes(value)), 0);
    }

    public void CSO2XML(string outputFileName, CSO instance)
    {
      using (TextWriter textWriter = (TextWriter) new StreamWriter(outputFileName))
        new XmlSerializer(typeof (CSO)).Serialize(textWriter, (object) instance);
    }

    public static CSO XML2CSO(string xmlFileName, string outputFileName, bool big_endian)
    {
      using (TextReader textReader = (TextReader) new StreamReader(xmlFileName))
      {
        CSO cso = (CSO) new XmlSerializer(typeof (CSO)).Deserialize(textReader);
        if (big_endian)
          cso.SaveBE(outputFileName);
        else
          cso.SaveLE(outputFileName);
        return cso;
      }
    }
  }
}
