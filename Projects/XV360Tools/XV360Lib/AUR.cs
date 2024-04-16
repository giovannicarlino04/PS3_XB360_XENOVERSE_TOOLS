using System;
using System.Collections.Generic;
using System.IO;
using System.Xml.Serialization;

namespace XV360Lib
{
    public struct Aura
    {
        public int[] Color;
    }

    public struct Charlisting
    {
        public int Name;
        public int Costume;
        public int ID;
        public bool inf;
    }

    public struct CharName
    {
        public int ID;
        public string Name;

        public CharName(int i, string n)
        {
            ID = i;
            Name = n;
        }
    }

    public class AUR
    {
        public Aura[] Auras;
        public byte[] backup = new byte[104];
        public Charlisting[] Chars;

        public void load(string FileName)
        {
            var file = File.ReadAllBytes(FileName);
            if (file == null)
                throw new ArgumentException("No bytes in .aur file");

            Auras = new Aura[ReverseBytesInt32(BitConverter.ToInt32(file, 8))];
            int AuraAddress = ReverseBytesInt32(BitConverter.ToInt32(file, 12));
            for (int A = 0; A < Auras.Length; A++)
            {
                int id = ReverseBytesInt32(BitConverter.ToInt32(file, AuraAddress + (16 * A)));
                Auras[id].Color = new int[ReverseBytesInt32(BitConverter.ToInt32(file, AuraAddress + (16 * A) + 8))];
                int CAddress = ReverseBytesInt32(BitConverter.ToInt32(file, AuraAddress + (16 * A) + 12));
                for (int C = 0; C < Auras[id].Color.Length; C++)
                    Auras[id].Color[ReverseBytesInt32(BitConverter.ToInt32(file, CAddress + (C * 8)))] = ReverseBytesInt32(BitConverter.ToInt32(file, CAddress + (C * 8) + 4));
            }

            int WAddress = ReverseBytesInt32(BitConverter.ToInt32(file, 20));
            Array.Copy(file, WAddress, backup, 0, 104);

            Chars = new Charlisting[ReverseBytesInt32(BitConverter.ToInt32(file, 24))];
            int ChAddress = ReverseBytesInt32(BitConverter.ToInt32(file, 28));
            for (int C = 0; C < Chars.Length; C++)
            {
                Chars[C].Name = ReverseBytesInt32(BitConverter.ToInt32(file, ChAddress + (C * 16)));
                Chars[C].Costume = ReverseBytesInt32(BitConverter.ToInt32(file, ChAddress + (C * 16) + 4));
                Chars[C].ID = ReverseInt16(BitConverter.ToInt16(file, ChAddress + (C * 16) + 8));
                Chars[C].inf = BitConverter.ToBoolean(ReverseBytes(BitConverter.GetBytes(BitConverter.ToInt32(file, ChAddress + (C * 16) + 12))), 0);
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
                Charbytes.AddRange(BitConverter.GetBytes(ReverseInt16(Chars[C].ID))); 
                Charbytes.AddRange(BitConverter.GetBytes(ReverseBytes(Chars[C].inf ? 1 : 0))); // Convert bool to int (1 for true, 0 for false)
                Charbytes.AddRange(new byte[] { 0x00, 0x00 });
            }



            file.AddRange(signature);
            file.AddRange(Top);
            file.AddRange(Aura1);
            file.AddRange(Aura2);
            file.AddRange(backup);
            if (filler.Length != 16)
                file.AddRange(filler);
            file.AddRange(Charbytes);

            using (FileStream newfile = new FileStream(FileName, FileMode.Create))
            {
                newfile.Write(file.ToArray(), 0, file.Count);
            }
        }
        public void AddCharacter(int id, int name, int costume, bool inf)
        {
            // Create a new array to hold the updated characters
            Charlisting[] newChars = new Charlisting[Chars.Length + 1];

            // Copy existing characters to the new array
            Array.Copy(Chars, newChars, Chars.Length);

            // Add the new character at the end
            newChars[newChars.Length - 1] = new Charlisting
            {
                Name = name, // Set default name, you can change it if needed
                Costume = costume,
                ID = id,
                inf = inf // Set default value for inf, change it as needed
            };

            // Update the Chars array with the new array
            Chars = newChars;
        }
        public void RemoveCharacter(int index)
        {

            if (index < 0 || index >= Chars.Length)
            {
                throw new IndexOutOfRangeException("Index is out of range");
            }

            Charlisting[] newChars = new Charlisting[Chars.Length - 1];

            Array.Copy(Chars, 0, newChars, 0, index);

            Array.Copy(Chars, index + 1, newChars, index, Chars.Length - index - 1);

            Chars = newChars;
        }
        private short ReverseInt16(int value)
        {
            return (short)(((value & 0xFF) << 8) | ((value >> 8) & 0xFF));
        }


        private int ReverseInt32(long value)
        {
            return (int)((value & 0x000000FF) << 24 |
                   (value & 0x0000FF00) << 8 |
                   (value & 0x00FF0000) >> 8 |
                   (value & 0xFF000000) >> 24);
        }
        private int ReverseBytesInt32(int value)
        {
            return BitConverter.ToInt32(ReverseBytes(BitConverter.GetBytes(value)), 0);
        }

        private byte[] ReverseBytes(byte[] bytes)
        {
            Array.Reverse(bytes);
            return bytes;
        }

        private int ReverseBytes(int value)
        {
            return BitConverter.ToInt32(ReverseBytes(BitConverter.GetBytes(value)), 0);
        }


        public void AUR2XML(string outputFileName, AUR instance)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(AUR));
            using (TextWriter writer = new StreamWriter(outputFileName))
            {
                serializer.Serialize(writer, instance);
            }
        }

        public static AUR XML2AUR(string xmlFileName, string outputFileName)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(AUR));
            using (TextReader reader = new StreamReader(xmlFileName))
            {
                AUR parsedObject = (AUR)serializer.Deserialize(reader);
                // Perform any additional actions here if needed

                // Save the parsed object to the original desired file
                parsedObject.save(outputFileName);

                return parsedObject;
            }
        }



    }
}
