using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace MusicPlayer
{
    class MainPlaylist
    {
        public List<Song> songs;
        private string FileName = "MainPlaylist.pls";
        public bool bIsLoaded
        {
            get;
            set;
        }

        public MainPlaylist()
        {
            songs = new List<Song>();
        }

        public void Save()
        {
            if (songs.Count == 0)
                return;
            StreamWriter sw;
            string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), FileName);
            sw = new StreamWriter(path);
            foreach (var s in songs)
                sw.WriteLine(s.Url);
            sw.Close();
        }

        public void Load()
        {
            StreamReader sr;
            Song[] result;
            string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), FileName);
            if (!File.Exists(path))
                return;
            sr = new StreamReader(path);
            List<string> list = new List<string>();
            while (!sr.EndOfStream)
                list.Add(sr.ReadLine());
            result = new Song[list.Count];
            for (int i = 0; i < list.Count; ++i)
            {
                string str = list[i];
                string Name = str.Split('\\')[str.Split('\\').Length - 1];
                songs.Add(new Song(str, Name));
            }
        }

        public void AddSong(Song song)
        {
            songs.Add(song);
        }
    }
}
