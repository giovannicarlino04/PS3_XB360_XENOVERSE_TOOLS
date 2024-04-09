using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace XV360Lib
{
    struct Aura
    {
        public int[] Color;
    }

    struct Charlisting
    {
        public int Name;
        public int Costume;
        public int ID;
        public bool inf;
    }

    struct CharName
    {
        public int ID;
        public string Name;

        public CharName(int i, string n)
        {
            ID = i;
            Name = n;
        }
    }
    internal class AUR
    {
        Aura[] Auras;
        byte[] backup = new byte[104];
        Charlisting[] Chars;

        public void load(string FileName)
        {
            var file = File.ReadAllBytes(FileName); if (file == null) throw new ArgumentException("No bytes in .aur file");

            Auras = new Aura[BitConverter.ToInt32(ReverseBytes(file, 8), 0)];
            int AuraAddress = BitConverter.ToInt32(ReverseBytes(file, 12), 0);
            for (int A = 0; A < Auras.Length; A++)
            {
                int id = BitConverter.ToInt32(ReverseBytes(file, AuraAddress + (16 * A)), 0);
                Auras[id].Color = new int[BitConverter.ToInt32(ReverseBytes(file, AuraAddress + (16 * A) + 8), 0)];
                int CAddress = BitConverter.ToInt32(ReverseBytes(file, AuraAddress + (16 * A) + 12), 0);
                for (int C = 0; C < Auras[id].Color.Length; C++)
                    Auras[id].Color[BitConverter.ToInt32(ReverseBytes(file, CAddress + (C * 8)), 0)] = BitConverter.ToInt32(ReverseBytes(file, CAddress + (C * 8) + 4), 0);
            }

            int WAddress = BitConverter.ToInt32(ReverseBytes(file, 20), 0);
            Array.Copy(file, WAddress, backup, 0, 104);

            Chars = new Charlisting[BitConverter.ToInt32(ReverseBytes(file, 24), 0)];
            int ChAddress = BitConverter.ToInt32(ReverseBytes(file, 28), 0);
            for (int C = 0; C < Chars.Length; C++)
            {
                Chars[C].Name = BitConverter.ToInt32(ReverseBytes(file, ChAddress + (C * 16)), 0);
                Chars[C].Costume = BitConverter.ToInt32(ReverseBytes(file, ChAddress + (C * 16) + 4), 0);
                Chars[C].ID = BitConverter.ToInt32(ReverseBytes(file, ChAddress + (C * 16) + 8), 0);
                Chars[C].inf = BitConverter.ToBoolean(ReverseBytes(file, ChAddress + (C * 16) + 12), 0);
            }
        }
        public void save(string FileName)
        {

            List<byte> file = new List<byte>();
            byte[] signature = new byte[] { 0x23, 0x41, 0x55, 0x52, 0xFF, 0xFE, 0x00, 0x20 };
            byte[] Top = new byte[24];
            byte[] Aura1 = new byte[16 * Auras.Length];
            List<byte> Aura2 = new List<byte>();
            Array.Copy(BitConverter.GetBytes(ReverseBytes(Auras.Length)), 0, Top, 0, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(32)), 0, Top, 4, 4);
            for (int A = 0; A < Auras.Length; A++)
            {
                Array.Copy(BitConverter.GetBytes(ReverseBytes(A)), 0, Aura1, (A * 16), 4);
                Array.Copy(BitConverter.GetBytes(ReverseBytes(Auras[A].Color.Length)), 0, Aura1, (A * 16) + 8, 4);
                Array.Copy(BitConverter.GetBytes(ReverseBytes(32 + Aura1.Length + Aura2.Count)), 0, Aura1, (A * 16) + 12, 4);
                for (int C = 0; C < Auras[A].Color.Length; C++)
                {
                    Aura2.AddRange(BitConverter.GetBytes(ReverseBytes(C)));
                    if (Auras[A].Color[C] < 0)
                        Aura2.AddRange(new byte[] { 0xFF, 0xFF, 0xFF, 0xFF });
                    else
                        Aura2.AddRange(BitConverter.GetBytes(ReverseBytes(Auras[A].Color[C])));
                }
            }

            int length = 32 + Aura1.Length + Aura2.Count;

            Array.Copy(BitConverter.GetBytes(ReverseBytes(7)), 0, Top, 8, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length)), 0, Top, 12, 4);
            //backup shift - 28,39,49,58,69,80,93
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 28)), 0, backup, 0, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 39)), 0, backup, 4, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 49)), 0, backup, 8, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 58)), 0, backup, 12, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 69)), 0, backup, 16, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 80)), 0, backup, 20, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length + 93)), 0, backup, 24, 4);

            length += backup.Length;

            byte[] filler = new byte[16 - (length % 16)];

            if (filler.Length != 16)
                length += filler.Length;

            Array.Copy(BitConverter.GetBytes(ReverseBytes(Chars.Length)), 0, Top, 16, 4);
            Array.Copy(BitConverter.GetBytes(ReverseBytes(length)), 0, Top, 20, 4);

            List<byte> Charbytes = new List<byte>();

            for (int C = 0; C < Chars.Length; C++)
            {
                Charbytes.AddRange(BitConverter.GetBytes(ReverseBytes(Chars[C].Name)));
                Charbytes.AddRange(BitConverter.GetBytes(ReverseBytes(Chars[C].Costume)));
                Charbytes.AddRange(BitConverter.GetBytes(ReverseBytes(Chars[C].ID)));
                Charbytes.AddRange(BitConverter.GetBytes(Chars[C].inf));
                Charbytes.AddRange(new byte[] { 0x00, 0x00, 0x00 });
            }

            file.AddRange(signature);
            file.AddRange(Top);
            file.AddRange(Aura1);
            file.AddRange(Aura2);
            file.AddRange(backup);
            if (filler.Length != 16)
                file.AddRange(filler);
            file.AddRange(Charbytes);

            FileStream newfile = new FileStream(FileName, FileMode.Create);
            newfile.Write(file.ToArray(), 0, file.Count);
            newfile.Close();
        }

        private byte[] ReverseBytes(byte[] bytes,int shift)
        {
            return bytes.Reverse().ToArray();
        }

        private int ReverseBytes(int value)
        {
            return BitConverter.ToInt32(ReverseBytes(BitConverter.GetBytes(value), 0));
        }
    }
}
