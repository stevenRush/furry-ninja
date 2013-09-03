using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

class TagID3v1Header
{
    public byte[] ID = new byte[3];
    public byte[] Title = new byte[30];
    public byte[] Artist = new byte[30];
    public byte[] Album = new byte[30];
    public byte[] Year = new byte[4];
    public byte[] Comment = new byte[30];
    public byte[] Genre = new byte[1];
}

class TagID3v2Header
{
    public byte[] ID = new byte[3];
    public byte[] Version = new byte[1];
    public byte[] Revision = new byte[1];
    public byte[] Flags = new byte[1];
    public byte[] Size = new byte[4];
}

class TagID3v2Frame
{
    public byte[] ID = new byte[4];
    public int Size;
    public byte[] Flags = new byte[2];
}

class TagID3v2Helper
{
    protected static int DecodeSyncSafeInteger(byte[] Value, int ValueSize)
    {
        // The most significant bit (bit 7) is set to zero in every byte in size
        int Result = 0;
        int Shift = 0;
        for (int i = ValueSize - 1; i >= 0; --i, Shift += 7)
            Result |= Value[i] << Shift;
        return Result;
    }

    public static int GetID3v2Size(TagID3v2Header Header)
    {
        int Result = DecodeSyncSafeInteger(Header.Size, 4);
        Result += 10; // Tag Header size
        return Result;
    }

    public static bool ReadID3v2Frame(FileStream Reader, int Version, out TagID3v2Frame Frame)
    {
        Frame = new TagID3v2Frame();
        if (Version == 2)
        {
            // in ID3v2.2 ID and Size are 3-bytes long 
            byte[] Size = new byte[3];
            Reader.Read(Frame.ID, 0, 3);
            Reader.Read(Size, 0, 3);
            Frame.Size = DecodeSyncSafeInteger(Size, 3);
        }
        else
        {
            byte[] Size = new byte[4];
            Reader.Read(Frame.ID, 0, Frame.ID.Length);
            Reader.Read(Size, 0, 4);
            Reader.Read(Frame.Flags, 0, Frame.Flags.Length);
            Frame.Size = DecodeSyncSafeInteger(Size, 4);
        }
        // TODO: add flag checking
        return (Frame.Size > 1);
    }

    public static void ReadID3v2Value(FileStream Reader, int FrameSize, ref string Value)
    {
        byte StrEncoding = (byte)Reader.ReadByte();
        byte[] Data = new byte[FrameSize - 1];
        Reader.Read(Data, 0, FrameSize - 1);
        Reader.Seek(-FrameSize, SeekOrigin.Current);
        switch (StrEncoding)
        {
            case 0:
                Value = Encoding.Default.GetString(Data);
                break;
            case 1:
                Value = Encoding.Unicode.GetString(Data, 2, Data.Length - 2); // Skip 0xFFFE
                break;
            case 2:
                Value = Encoding.Unicode.GetString(Data);
                break;
            case 3:
                Value = Encoding.UTF8.GetString(Data);
                break;
        }
    }
}

class TagsReader
{
    protected bool LoadID3v1(FileStream FileReader)
    {
        TagID3v1Header Tag = new TagID3v1Header();
        if (FileReader.Length < 128) 
            return false;
        FileReader.Seek(-128, SeekOrigin.End);
        FileReader.Read(Tag.ID, 0, Tag.ID.Length);
        if (Tag.ID[0] == 'T' && Tag.ID[1] == 'A' && Tag.ID[2] == 'G')
        {
            FileReader.Read(Tag.Title, 0, Tag.Title.Length);
            FileReader.Read(Tag.Artist, 0, Tag.Artist.Length);
            FileReader.Read(Tag.Album, 0, Tag.Album.Length);
            FileReader.Read(Tag.Year, 0, Tag.Year.Length);
            //
            Title = Encoding.Default.GetString(Tag.Title);
            Artist = Encoding.Default.GetString(Tag.Artist);
            Album = Encoding.Default.GetString(Tag.Album);
            Year = Encoding.Default.GetString(Tag.Year);
            return true;
        }
        return false;
    }

    protected bool LoadID3v2(FileStream FileReader)
    {
        TagID3v2Header Tag = new TagID3v2Header();
        FileReader.Seek(0, SeekOrigin.Begin);
        FileReader.Read(Tag.ID, 0, Tag.ID.Length);
        FileReader.Read(Tag.Version, 0, Tag.Version.Length);
        FileReader.Read(Tag.Revision, 0, Tag.Revision.Length);
        FileReader.Read(Tag.Flags, 0, Tag.Flags.Length);
        FileReader.Read(Tag.Size, 0, Tag.Size.Length);
        int TagSize = TagID3v2Helper.GetID3v2Size(Tag);
        if (TagSize > 0 && Tag.Version[0] >= 2 && Tag.Version[0] <= 4)
        {
            TagID3v2Frame Frame;
            int Position;
            while (TagID3v2Helper.ReadID3v2Frame(FileReader, Tag.Version[0], out Frame))
            {
             //   Position  = (int)FileReader.Position;
             
                if ((Frame.ID[0] == 0x54) && (Frame.ID[1] == 0x41) && (Frame.ID[2] == 0x4C) && (Frame.ID[3] == 0x42))
                    TagID3v2Helper.ReadID3v2Value(FileReader, Frame.Size, ref Album);
                if ((Frame.ID[0] == 0x54) && (Frame.ID[1] == 0x50) && (Frame.ID[2] == 0x45) && (Frame.ID[3] == 0x31))
                    TagID3v2Helper.ReadID3v2Value(FileReader, Frame.Size, ref Artist);
                if ((Frame.ID[0] == 'T') && (Frame.ID[1] == 'I') && (Frame.ID[2] == 'T') && (Frame.ID[3] == '2'))
                    TagID3v2Helper.ReadID3v2Value(FileReader, Frame.Size, ref Title);
                if ((Frame.ID[0] == 'T') && (Frame.ID[1] == 'Y') && (Frame.ID[2] == 'E') && (Frame.ID[3] == 'R'))
                    TagID3v2Helper.ReadID3v2Value(FileReader, Frame.Size, ref Year);
                FileReader.Seek(Frame.Size, SeekOrigin.Current);
            }
            return true;
        }
        return false;
    }

    public void Clear()
    {
        Title = "";
        Artist = "";
        Album = "";
        Year = "";
    }

    public bool LoadTags(string FilePath)
    {
        FileStream Reader = new FileStream(FilePath, FileMode.Open);
        return LoadID3v2(Reader) || LoadID3v1(Reader);
    }

    public string Title;
    public string Artist;
    public string Album;
    public string Year;
}
