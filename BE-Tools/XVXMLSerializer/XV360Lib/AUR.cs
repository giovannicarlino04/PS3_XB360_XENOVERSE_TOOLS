// Decompiled with JetBrains decompiler
// Type: XV360Lib.AUR
// Assembly: XV360Lib, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: B9D187B2-9B90-4A2F-B236-CD1D7F0B7832
// Assembly location: C:\Users\Giovanni\Desktop\Repositories\PS3_XB360_XENOVERSE_TOOLS\LibXenoverse_BE_Build\XV360Lib.dll

using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;

namespace XV360Lib
{
  public class AUR
  {
    public Aura[] Auras;
    public byte[] backup = new byte[104];
    public Charlisting[] Chars;

    public void load(string FileName)
    {
      byte[] sourceArray = File.ReadAllBytes(FileName);
      this.Auras = sourceArray != null ? new Aura[this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, 8))] : throw new ArgumentException("No bytes in .aur file");
      int num1 = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, 12));
      for (int index1 = 0; index1 < this.Auras.Length; ++index1)
      {
        int index2 = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num1 + 16 * index1));
        this.Auras[index2].Color = new int[this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num1 + 16 * index1 + 8))];
        int num2 = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num1 + 16 * index1 + 12));
        for (int index3 = 0; index3 < this.Auras[index2].Color.Length; ++index3)
          this.Auras[index2].Color[this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num2 + index3 * 8))] = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num2 + index3 * 8 + 4));
      }
      int sourceIndex = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, 20));
      Array.Copy((Array) sourceArray, sourceIndex, (Array) this.backup, 0, 104);
      this.Chars = new Charlisting[this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, 24))];
      int num3 = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, 28));
      for (int index = 0; index < this.Chars.Length; ++index)
      {
        this.Chars[index].Name = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num3 + index * 16));
        this.Chars[index].Costume = this.ReverseBytesInt32(BitConverter.ToInt32(sourceArray, num3 + index * 16 + 4));
        this.Chars[index].ID = (int) this.ReverseInt16((int) BitConverter.ToInt16(sourceArray, num3 + index * 16 + 8));
        this.Chars[index].inf = BitConverter.ToBoolean(this.ReverseBytes(BitConverter.GetBytes(BitConverter.ToInt32(sourceArray, num3 + index * 16 + 12))), 0);
      }
    }

    public void save(string FileName)
    {
      List<byte> byteList = new List<byte>();
      byte[] collection1 = new byte[8]
      {
        (byte) 35,
        (byte) 65,
        (byte) 85,
        (byte) 82,
        byte.MaxValue,
        (byte) 254,
        (byte) 0,
        (byte) 32
      };
      byte[] numArray1 = new byte[24];
      byte[] numArray2 = new byte[16 * this.Auras.Length];
      List<byte> collection2 = new List<byte>();
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(this.Auras.Length)), 0, (Array) numArray1, 0, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(32)), 0, (Array) numArray1, 4, 4);
      for (int index1 = 0; index1 < this.Auras.Length; ++index1)
      {
        Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(index1)), 0, (Array) numArray2, index1 * 16, 4);
        Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(this.Auras[index1].Color.Length)), 0, (Array) numArray2, index1 * 16 + 8, 4);
        Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(32 + numArray2.Length + collection2.Count)), 0, (Array) numArray2, index1 * 16 + 12, 4);
        for (int index2 = 0; index2 < this.Auras[index1].Color.Length; ++index2)
        {
          collection2.AddRange((IEnumerable<byte>) BitConverter.GetBytes(this.ReverseBytes(index2)));
          if (this.Auras[index1].Color[index2] < 0)
            collection2.AddRange((IEnumerable<byte>) new byte[4]
            {
              byte.MaxValue,
              byte.MaxValue,
              byte.MaxValue,
              byte.MaxValue
            });
          else
            collection2.AddRange((IEnumerable<byte>) BitConverter.GetBytes(this.ReverseBytes(this.Auras[index1].Color[index2])));
        }
      }
      int num1 = 32 + numArray2.Length + collection2.Count;
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(7)), 0, (Array) numArray1, 8, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1)), 0, (Array) numArray1, 12, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 28)), 0, (Array) this.backup, 0, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 39)), 0, (Array) this.backup, 4, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 49)), 0, (Array) this.backup, 8, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 58)), 0, (Array) this.backup, 12, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 69)), 0, (Array) this.backup, 16, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 80)), 0, (Array) this.backup, 20, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num1 + 93)), 0, (Array) this.backup, 24, 4);
      int num2 = num1 + this.backup.Length;
      byte[] collection3 = new byte[16 - num2 % 16];
      if (collection3.Length != 16)
        num2 += collection3.Length;
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(this.Chars.Length)), 0, (Array) numArray1, 16, 4);
      Array.Copy((Array) BitConverter.GetBytes(this.ReverseBytes(num2)), 0, (Array) numArray1, 20, 4);
      List<byte> collection4 = new List<byte>();
      for (int index = 0; index < this.Chars.Length; ++index)
      {
        collection4.AddRange((IEnumerable<byte>) BitConverter.GetBytes(this.ReverseBytes(this.Chars[index].Name)));
        collection4.AddRange((IEnumerable<byte>) BitConverter.GetBytes(this.ReverseBytes(this.Chars[index].Costume)));
        collection4.AddRange((IEnumerable<byte>) BitConverter.GetBytes(this.ReverseInt16(this.Chars[index].ID)));
        collection4.AddRange((IEnumerable<byte>) BitConverter.GetBytes(this.ReverseBytes(this.Chars[index].inf ? 1 : 0)));
        collection4.AddRange((IEnumerable<byte>) new byte[2]);
      }
      byteList.AddRange((IEnumerable<byte>) collection1);
      byteList.AddRange((IEnumerable<byte>) numArray1);
      byteList.AddRange((IEnumerable<byte>) numArray2);
      byteList.AddRange((IEnumerable<byte>) collection2);
      byteList.AddRange((IEnumerable<byte>) this.backup);
      if (collection3.Length != 16)
        byteList.AddRange((IEnumerable<byte>) collection3);
      byteList.AddRange((IEnumerable<byte>) collection4);
      using (FileStream fileStream = new FileStream(FileName, FileMode.Create))
        fileStream.Write(byteList.ToArray(), 0, byteList.Count);
    }

    public void AddCharacter(int id, int name, int costume, bool inf)
    {
      Charlisting[] destinationArray = new Charlisting[this.Chars.Length + 1];
      Array.Copy((Array) this.Chars, (Array) destinationArray, this.Chars.Length);
      destinationArray[destinationArray.Length - 1] = new Charlisting()
      {
        Name = name,
        Costume = costume,
        ID = id,
        inf = inf
      };
      this.Chars = destinationArray;
    }

    public void RemoveCharacter(int index)
    {
      if (index < 0 || index >= this.Chars.Length)
        throw new IndexOutOfRangeException("Index is out of range");
      Charlisting[] destinationArray = new Charlisting[this.Chars.Length - 1];
      Array.Copy((Array) this.Chars, 0, (Array) destinationArray, 0, index);
      Array.Copy((Array) this.Chars, index + 1, (Array) destinationArray, index, this.Chars.Length - index - 1);
      this.Chars = destinationArray;
    }

    private short ReverseInt16(int value)
    {
      return (short) ((value & (int) byte.MaxValue) << 8 | value >> 8 & (int) byte.MaxValue);
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

    public void AUR2XML(string outputFileName, AUR instance)
    {
      using (TextWriter textWriter = (TextWriter) new StreamWriter(outputFileName))
        new XmlSerializer(typeof (AUR)).Serialize(textWriter, (object) instance);
    }

    public static AUR XML2AUR(string xmlFileName, string outputFileName, bool big_endian)
    {
      using (TextReader textReader = (TextReader) new StreamReader(xmlFileName))
      {
        AUR aur = (AUR) new XmlSerializer(typeof (AUR)).Deserialize(textReader);
        aur.save(outputFileName);
        return aur;
      }
    }
  }
}
