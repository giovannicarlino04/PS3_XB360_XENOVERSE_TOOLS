using System;
using XV360Lib;
internal class Program
{
    private static void Main(string[] args)
    {
        AUR aur = new AUR();
        CMS cms = new CMS();
        CSO cso = new CSO();
        CUS cus = new CUS();

        if (args.Length == 0)
            throw new Exception("No args were given");

        string fileExtension = System.IO.Path.GetExtension(args[0]);

        switch (fileExtension.ToLower())
        {
            case ".aur":
                aur.load(args[0]);
                aur.AUR2XML(args[0].Replace(".aur", ".aurxml"), aur);
                break;
            case ".aurxml":
                AUR.XML2AUR(args[0], args[0].Replace(".aurxml", ".aur"));
                break;

            case ".cms":
                cms.Load(args[0]);
                cms.CMS2XML(args[0].Replace(".cms", ".cmsxml"), cms);
                break;
            case ".cmsxml":
                CMS.XML2CMS(args[0], args[0].Replace(".cmsxml", ".cms"));
                break;

            case ".cso":
                cso.Load(args[0]);
                cso.CSO2XML(args[0].Replace(".cso", ".csoxml"), cso);
                break;
            case ".csoxml":
                CSO.XML2CSO(args[0], args[0].Replace(".csoxml", ".cso"));
                break;


            default:
                throw new NotImplementedException("Unsupported/Not implemented file extension");
        }
    }
}
