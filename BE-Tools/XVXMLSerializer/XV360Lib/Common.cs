// Decompiled with JetBrains decompiler
// Type: XV360Lib.Common
// Assembly: XV360Lib, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: B9D187B2-9B90-4A2F-B236-CD1D7F0B7832
// Assembly location: C:\Users\Giovanni\Desktop\Repositories\PS3_XB360_XENOVERSE_TOOLS\LibXenoverse_BE_Build\XV360Lib.dll

using System;
using System.IO;


namespace XV360Lib
{
  public class Common
  {
    public static bool isBigEndian(string arg)
    {
      using (BinaryReader binaryReader = new BinaryReader((Stream) File.Open(arg, FileMode.Open)))
      {
        binaryReader.BaseStream.Seek(4L, SeekOrigin.Begin);
        byte num1 = binaryReader.ReadByte();
        byte num2 = binaryReader.ReadByte();
        if (num1 == byte.MaxValue && num2 == (byte) 254)
        {
          Console.WriteLine("File " + arg + " is Big Endian");
          return true;
        }
        if (num1 != (byte) 254 || num2 != byte.MaxValue)
          throw new ArgumentException("File " + arg + " not recognized");
        Console.WriteLine("File " + arg + " is Little Endian");
        return false;
      }
    }
  }
}
