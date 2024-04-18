using System;
using XV360Lib;
internal class Program
{
    public static bool big_endian = true;
    private static void Main(string[] args)
    {
        CMS cms = new CMS();
        CSO cso = new CSO();
        AUR aur = new AUR();

        if (args.Length == 0)
            throw new Exception("No args were given");

        foreach (var arg in args)
        {
            
            string fileExtension = System.IO.Path.GetExtension(arg);

            switch (fileExtension.ToLower())
            {
                case ".cms":
                    big_endian = Common.isBigEndian(arg);
                    if (big_endian)
                        cms.LoadBE(arg);
                    else
                        cms.LoadLE(arg);
                    cms.CMS2XML(arg.Replace(".cms", ".cmsxml"), cms);
                    break;
                case ".cmsxml":
                    CMS.XML2CMS(arg, arg.Replace(".cmsxml", ".cms"), big_endian);
                    break;

                case ".aur":
                    big_endian = Common.isBigEndian(arg);
                    if (big_endian)
                        aur.load(arg);
                    else
                        aur.load(arg);
                    aur.AUR2XML(arg.Replace(".aur", ".aurxml"), aur);
                    break;
                case ".aurxml":
                    AUR.XML2AUR(arg, arg.Replace(".aurxml", ".aur"), big_endian);
                    break;

                case ".cso":
                    big_endian = Common.isBigEndian(arg);
                    if (big_endian)
                        cso.LoadBE(arg);
                    else
                        cso.LoadLE(arg);
                    cso.CSO2XML(arg.Replace(".cso", ".csoxml"), cso);
                    break;
                case ".csoxml":
                    CSO.XML2CSO(arg, arg.Replace(".csoxml", ".cso"), big_endian);
                    break;


                default:
                    throw new NotImplementedException("Unsupported/Not implemented file extension");
            }
        }
    }
}