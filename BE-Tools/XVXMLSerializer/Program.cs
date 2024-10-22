// Decompiled with JetBrains decompiler
// Type: Program
// Assembly: XVXMLSerializer, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
// MVID: 26271102-2A4A-47D0-A71E-41E9530026C6
// Assembly location: C:\Users\Giovanni\Desktop\Repositories\PS3_XB360_XENOVERSE_TOOLS\LibXenoverse_BE_Build\XVXMLSerializer.dll

using System;
using System.IO;
using XV360Lib;


internal class Program
{
  public static bool big_endian = true;

  private static void Main(string[] args)
  {
    CMS instance1 = new CMS();
    CSO instance2 = new CSO();
    AUR instance3 = new AUR();
    if (args.Length == 0)
      throw new Exception("No args were given");
    foreach (string str in args)
    {
      switch (Path.GetExtension(str).ToLower())
      {
        case ".cms":
          Program.big_endian = Common.isBigEndian(str);
          if (Program.big_endian)
            instance1.LoadBE(str);
          else
            instance1.LoadLE(str);
          instance1.CMS2XML(str.Replace(".cms", ".cmsxml"), instance1);
          break;
        case ".cmsxml":
          CMS.XML2CMS(str, str.Replace(".cmsxml", ".cms"), Program.big_endian);
          break;
        case ".aur":
          Program.big_endian = Common.isBigEndian(str);
          if (Program.big_endian)
            instance3.load(str);
          else
            instance3.load(str);
          instance3.AUR2XML(str.Replace(".aur", ".aurxml"), instance3);
          break;
        case ".aurxml":
          AUR.XML2AUR(str, str.Replace(".aurxml", ".aur"), Program.big_endian);
          break;
        case ".cso":
          Program.big_endian = Common.isBigEndian(str);
          if (Program.big_endian)
            instance2.LoadBE(str);
          else
            instance2.LoadLE(str);
          instance2.CSO2XML(str.Replace(".cso", ".csoxml"), instance2);
          break;
        case ".csoxml":
          CSO.XML2CSO(str, str.Replace(".csoxml", ".cso"), Program.big_endian);
          break;
        default:
          throw new NotImplementedException("Unsupported/Not implemented file extension");
      }
    }
  }
}
