using System;
using XV360Lib;
internal class Program
{
    private static void Main(string[] args)
    {
        bool big_endian = true;

        CMS cms = new CMS();
        CSO cso = new CSO();

        if (args.Length == 0)
            throw new Exception("No args were given");

        foreach (var arg in args)
        {
            string fileExtension = System.IO.Path.GetExtension(arg);

            switch (fileExtension.ToLower())
            {
                case ".cms":
                    if (big_endian)
                        cms.LoadBE(arg);
                    else
                        cms.LoadLE(arg);
                    cms.CMS2XML(arg.Replace(".cms", ".cmsxml"), cms);
                    break;
                case ".cmsxml":
                    CMS.XML2CMS(arg, arg.Replace(".cmsxml", ".cms"), big_endian);
                    break;

                case ".cso":
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