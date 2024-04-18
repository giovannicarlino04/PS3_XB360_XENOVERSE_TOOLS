using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;

namespace XV360Lib
{
    public class Iggy
    {
        const uint IGGY_SIGNATURE = 0xED0A6749;

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct IGGYHeader
        {
            public uint signature; // 0
            public uint version; // 4
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] plattform; // 8
            public uint unk_0C; // 0xC
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
            public byte[] reserved; // 0x10
            public uint num_subfiles; // 0x1C
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct IGGYSubFileEntry
        {
            public uint id; // 0
            public uint size; // 4
            public uint size2; // 8
            public uint offset; // 0xC
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct IGGYFlashHeader32
        {
            public ushort main_offset; // 0
            public ushort as3_section_offset; // 4
            public uint unk_offset; // 8
            public uint unk_offset2; // 0xC
            public uint unk_offset3; // 0x10
            public uint unk_offset4; // 0x14
            public ulong unk_18;
            public uint width; // 0x20
            public uint height; // 0x24
            public uint unk_28;
            public uint unk_2C;
            public uint unk_30;
            public uint unk_34;
            public uint unk_38;
            public uint unk_3C;
            public float unk_40;
            public uint unk_44;
            public uint unk_48;
            public uint unk_4C;
            public uint names_offset; // 0x50
            public uint unk_offset5; // 0x54
            public ulong unk_58;
            public uint last_section_offset; // 0x60
            public uint unk_offset6; // 0x64
            public uint as3_code_offset; // 0x68
            public uint as3_names_offset; // 0x6C
            public uint unk_70;
            public uint unk_74;
            public uint unk_78;
            public uint unk_7C;
        }

        private IGGYHeader header;
        private List<IGGYSubFileEntry> subFileEntries;
        private IGGYFlashHeader32 flashHeader32;

        public bool Load(string IggyFile)
        {

            using (MemoryStream stream = new MemoryStream(File.ReadAllBytes(IggyFile)))
            using (BinaryReader reader = new BinaryReader(stream))
            {
                // Read header
                header = ReadStruct<IGGYHeader>(reader);

                // Convert endianness if needed
                ConvertEndianness(ref header);
                

                // Read subfile entries
                subFileEntries = new List<IGGYSubFileEntry>();
                for (int i = 0; i < header.num_subfiles; i++)
                {
                    IGGYSubFileEntry entry = ReadStruct<IGGYSubFileEntry>(reader);
                    ConvertEndianness(ref entry);
                    
                    subFileEntries.Add(entry);
                }

                // Read flash header
                stream.Seek(subFileEntries[0].offset, SeekOrigin.Begin);
                flashHeader32 = ReadStruct<IGGYFlashHeader32>(reader);
                ConvertEndianness(ref flashHeader32);
                

                return true;
            }
        }

        private T ReadStruct<T>(BinaryReader reader)
        {
            int structSize = Marshal.SizeOf(typeof(T));
            byte[] bytes = reader.ReadBytes(structSize);
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            T structure = (T)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(T));
            handle.Free();
            return structure;
        }

        private void ConvertEndianness<T>(ref T obj)
        {
            Type type = typeof(T);
            if (type == typeof(uint))
            {
                obj = (T)(object)BitConverter.ToUInt32((byte[])BitConverter.GetBytes((uint)(object)obj).Reverse(), 0);
            }
            else if (type == typeof(ulong))
            {
                obj = (T)(object)BitConverter.ToUInt64((byte[])BitConverter.GetBytes((ulong)(object)obj).Reverse(), 0);
            }
            else if (type == typeof(float))
            {
                obj = (T)(object)BitConverter.ToSingle((byte[])BitConverter.GetBytes((float)(object)obj).Reverse(), 0);
            }
            // Add other types as needed
        }

        public void PrintDebugInfo()
        {
            Console.WriteLine($"Signature: 0x{header.signature:X}");
            Console.WriteLine($"Version: {header.version}");
            Console.WriteLine($"Platform: {BitConverter.ToString(header.plattform)}");
            Console.WriteLine($"Number of Subfiles: {header.num_subfiles}");

            Console.WriteLine("Subfile Entries:");
            foreach (var entry in subFileEntries)
            {
                Console.WriteLine($"ID: {entry.id}, Size: {entry.size}, Offset: 0x{entry.offset:X}");
            }

            Console.WriteLine($"Main Offset: {flashHeader32.main_offset}");
            Console.WriteLine($"AS3 Section Offset: {flashHeader32.as3_section_offset}");
            Console.WriteLine($"Width: {flashHeader32.width}");
            Console.WriteLine($"Height: {flashHeader32.height}");
            // Print other fields as needed
        }
        public void Save(string iggyFilePath)
        {
            using (FileStream stream = new FileStream(iggyFilePath, FileMode.Create))
            using (BinaryWriter writer = new BinaryWriter(stream))
            {
                // Write header
                WriteStruct(writer, header);

                // Write subfile entries
                foreach (var entry in subFileEntries)
                {
                    WriteStruct(writer, entry);
                }

                // Write flash header
                WriteStruct(writer, flashHeader32);

                // Write the remaining data after the flash header
                long currentPosition = stream.Position;
                long remainingDataLength = stream.Length - currentPosition;
                byte[] remainingData = new byte[remainingDataLength];
                stream.Seek(currentPosition, SeekOrigin.Begin);
                stream.Read(remainingData, 0, (int)remainingDataLength);
                writer.Write(remainingData);
            }
        }

        private void WriteStruct<T>(BinaryWriter writer, T obj)
        {
            byte[] bytes = StructToBytes(obj);
            writer.Write(bytes);
        }

        private byte[] StructToBytes<T>(T obj)
        {
            int structSize = Marshal.SizeOf(typeof(T));
            byte[] bytes = new byte[structSize];
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            Marshal.StructureToPtr(obj, handle.AddrOfPinnedObject(), false);
            handle.Free();
            return bytes;
        }

    }
}