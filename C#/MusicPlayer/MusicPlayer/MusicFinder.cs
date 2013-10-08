using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HtmlAgilityPack;

namespace MusicPlayer
{
    class MusicFinder
    {
        static string url = "http://allter.mipt.ru/msearch.cgi";
        static string nothingFoundString = "Sorry. It looks like we found";

        private static string POST(string Url, string Data)
        {
            System.Net.WebRequest req = System.Net.WebRequest.Create(Url);
            req.Method = "POST";
            req.Timeout = 100000;
            req.ContentType = "application/x-www-form-urlencoded";
            byte[] sentData = Encoding.GetEncoding(1251).GetBytes(Data);
            req.ContentLength = sentData.Length;
            System.IO.Stream sendStream = req.GetRequestStream();
            sendStream.Write(sentData, 0, sentData.Length);
            sendStream.Close();
            System.Net.WebResponse res = req.GetResponse();
            System.IO.Stream ReceiveStream = res.GetResponseStream();
            System.IO.StreamReader sr = new System.IO.StreamReader(ReceiveStream, Encoding.UTF8);
            //Кодировка указывается в зависимости от кодировки ответа сервера
            Char[] read = new Char[256];
            int count = sr.Read(read, 0, 256);
            string Out = String.Empty;
            while (count > 0)
            {
                String str = new String(read, 0, count);
                Out += str;
                count = sr.Read(read, 0, 256);
            }
            return Out;
        }

        public static bool FindLinks(string Artist, string Song, ref List<Song> songs)
        {
            string html = POST(url, "artist=" + Artist + "&title=" + Song + "&album=&comment=&genre=all&year=&min_bitrate=");
            HtmlDocument doc = new HtmlDocument();
            doc.LoadHtml(html);
            var tags = doc.DocumentNode.ChildNodes[0].ChildNodes[3].ChildNodes[13].ChildNodes.Where(x => x.Name == "tr");
            //var Table = doc.DocumentNode.ChildNodes[3];
            string res = "";
            int i = 0;

            HashSet<string> set = new HashSet<string>();

            List<Song> list = new List<Song>();
            if (html.IndexOf(nothingFoundString) > -1)
            {
                songs = null;
                return false;
            }

            foreach (var tag in tags)
            {
                var cell = tag.ChildNodes[1].ChildNodes[1].ChildNodes[3].ChildNodes[1];
                //res += cell.InnerHtml + "\n";
                var links = cell.ChildNodes.Where(x => x.Name == "a");
                if (links != null)
                    foreach (var link in links)
                        if (link.Attributes["class"] != null && link.Attributes["class"].Value == "path")
                        {
                            res += link.InnerHtml;
                            if (i % 2 == 1)
                            {
                                string songName = link.InnerHtml;
                                res = res.Replace(@"\\NATALIE", @"\\natalie.campus");
                                Song song = new Song(res, songName);
                                if (!set.Contains(songName) && songName.IndexOf("mp3") > -1)
                                {
                                    list.Add(song);
                                    set.Add(songName);
                                }
                                res = string.Empty;
                            }
                            ++i;
                        }
            }
            songs = list;
            return true;
        }
    }

    class Song
    {
        public string Url;
        public string Name;
        public string Duration;

        public Song(string Url, string Name)
        {
            this.Url = Url;
            this.Name = Name;
        }
    }
}
