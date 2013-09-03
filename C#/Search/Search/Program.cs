using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Security.Cryptography;
using System.Data.SqlClient;

namespace Search
{
    class Program
    {
        static void Main(string[] args)
        {
            string path1 = @"C:\temp";
            string path2 = @"\\natalie.campus\incoming\! Botanstvo";

            Crawler crawler = new Crawler( new FileProcessor(),  path2);
            crawler.DoWork();
        }
    }
}
