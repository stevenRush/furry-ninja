using System;
using System.Security.Cryptography;
using System.IO;
using System.Text;
using System.Data.SqlClient;

class Hasher
{
    private static SHA256 mySHA256 = SHA256Managed.Create();

    public static long HashFile(string path)
    {
        byte[] name = Encoding.Unicode.GetBytes( Path.GetFileName( path ) );
        byte[] buff = new byte[1024 * 512 + name.Length];
        byte[] hashValue;
        int bytesCount = 0;
        using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read))
        {
            if (fs.Length < 512 * 1024)
            {
                bytesCount = fs.Read( buff, 0, 1024 * 512 );
            }
            else
            {
                fs.Read(buff, 0, 128 * 1024);
                fs.Seek(fs.Length / 2 - 128 * 1024, SeekOrigin.Begin);
                fs.Read(buff, 128 * 1024, 256 * 1024);
                fs.Seek(-128 * 1024, SeekOrigin.End);
                fs.Read(buff, 384 * 1024, 128 * 1024);
                bytesCount = 1024 * 512;
            }
            fs.Close();
        }
        Array.Copy(name, 0, buff, bytesCount, name.Length); 
        hashValue = mySHA256.ComputeHash(buff);
        return BitConverter.ToInt64(hashValue, 0);
    }

    public static long HashPath(string path)
    {
        byte[] buff = Encoding.Unicode.GetBytes(path);
        byte[] hashValue = mySHA256.ComputeHash(buff);
        return BitConverter.ToInt64(hashValue, 0);
    }
}

class SQLSettings
{
    public const string Server                  =   "LAPTOP\\SQLEXPRESS";
    public const string Database                =   "MIPTSearch";
    public const string filesTable              =   "Files";
    public const string extensionTable          =   "Extensions";
    public const string filePathsTable          =   "Folders";
    public const string locationsTable          =   "Locations";
    public const string insertProcedure         =   "InsertFile";
    public const string checkPathHashProcedure  =   "CheckPathHash";
}

class SQLWorkerBase
{
    protected SqlConnection connect;
    protected SqlDataReader reader;
    protected string connectionString;

    public SQLWorkerBase()
    {
        connectionString = String.Format(   "Server = {0}; Database = {1}; Trusted_Connection=True;", 
                                            SQLSettings.Server, SQLSettings.Database );
        connect = new SqlConnection(connectionString);
        connect.Open();
    }
}

struct BaseFileInfo
{
    public long FileHash;
    public long PathHash;
    public string Path;
    public string Name
    {
        get 
        {
            return System.IO.Path.GetFileName(Path);
        }
    }
    public string Extension 
    {
        get
        {
            return System.IO.Path.GetExtension(Path);
        }
    }
    public long Size;

    public BaseFileInfo(long fileHash, long pathHash, string path, long size)
    {
        FileHash = fileHash;
        PathHash = pathHash;
        Path = path;
        Size = size;
    }
}

class SQLFileWorker : SQLWorkerBase 
{
    protected SqlCommand InsertCmd;

    public SQLFileWorker()
    {
        InsertCmd = new SqlCommand( SQLSettings.insertProcedure, connect );
        InsertCmd.CommandType = System.Data.CommandType.StoredProcedure;
        InsertCmd.Parameters.Add( "@pathHash", System.Data.SqlDbType.BigInt );
        InsertCmd.Parameters.Add( "@fileHash", System.Data.SqlDbType.BigInt );
        InsertCmd.Parameters.Add( "@name", System.Data.SqlDbType.NVarChar );
        InsertCmd.Parameters.Add( "@parentFolder", System.Data.SqlDbType.NVarChar);
        InsertCmd.Parameters.Add( "@extension", System.Data.SqlDbType.NVarChar);
        InsertCmd.Parameters.Add( "@size", System.Data.SqlDbType.BigInt );
        InsertCmd.Parameters.Add( "@fileId", System.Data.SqlDbType.BigInt);
        InsertCmd.Parameters["@fileId"].Direction = System.Data.ParameterDirection.Output;
    }

    public long Insert(BaseFileInfo fi)
    {
        string parent = Directory.GetParent(fi.Path).FullName;

        InsertCmd.Parameters["@pathHash"].Value = fi.PathHash;
        InsertCmd.Parameters["@fileHash"].Value = fi.FileHash;
        InsertCmd.Parameters["@name"].Value = fi.Name;
        InsertCmd.Parameters["@parentFolder"].Value = parent;
        InsertCmd.Parameters["@extension"].Value = fi.Extension;
        InsertCmd.Parameters["@size"].Value = fi.Size;
        InsertCmd.ExecuteNonQuery();
        return long.Parse(InsertCmd.Parameters["@fileId"].Value.ToString()); 
    }

    public long FindByPathHash(long Hash)
    {
        SqlCommand selectCmd = new SqlCommand( SQLSettings.checkPathHashProcedure, connect );
        selectCmd.CommandType = System.Data.CommandType.StoredProcedure;
        selectCmd.Parameters.Add( "@pathHash", System.Data.SqlDbType.BigInt );
        selectCmd.Parameters["@pathHash"].Value = Hash;
        selectCmd.Parameters.Add( "@fileId", System.Data.SqlDbType.BigInt);
        selectCmd.Parameters["@fileId"].Direction = System.Data.ParameterDirection.Output;
        selectCmd.ExecuteNonQuery();
        return long.Parse(selectCmd.Parameters["@fileId"].Value.ToString());
    }
}