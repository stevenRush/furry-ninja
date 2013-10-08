using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Media;
using System.IO;
using WMPLib;

namespace MusicPlayer
{
    public partial class Form1 : Form
    {
        List<Song> songs;
        int currentPos;
        Form2 form2;
        AboutBox1 aboutForm;
        MainPlaylist mainList;
        bool ChangeLock;

        public Form1()
        {
            InitializeComponent();
            form2 = new Form2();
            axWindowsMediaPlayer1.MediaChange += axWindowsMediaPlayer1_MediaChange;
            backgroundWorker1.DoWork += backgroundWorker1_DoWork;
            backgroundWorker1.RunWorkerCompleted += backgroundWorker1_RunWorkerCompleted;
            aboutForm = new AboutBox1();
            mainList = new MainPlaylist();
            mainList.Load();
            songs = mainList.songs;
            LoadSongs();
            mainList.bIsLoaded = true;
            ChangeLock = false;
        }

        void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            form2.Close();

            if (songs == null)
            {
                MessageBox.Show("Ничего не найдено!");
                return;
            }
            LoadSongs();
            mainList.bIsLoaded = false;
        }

        private void LoadSongs()
        {
            if (songs == null)
                return;
            listBox1.Items.Clear();
            IWMPPlaylist plist = axWindowsMediaPlayer1.currentPlaylist;
            plist.clear();
            foreach (var song in songs)
            {
                plist.appendItem(axWindowsMediaPlayer1.newMedia(song.Url));
                listBox1.Items.Add(song.Name);
            }
            currentPos = 0;
            axWindowsMediaPlayer1.Ctlcontrols.stop();
        }

        void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            MusicFinder.FindLinks(textBox1.Text, textBox2.Text, ref songs);
        }

        void axWindowsMediaPlayer1_MediaChange(object sender, AxWMPLib._WMPOCXEvents_MediaChangeEvent e)
        {
            if (ChangeLock)
                return;

            IWMPPlaylist plist = axWindowsMediaPlayer1.currentPlaylist;
            IWMPControls cont = axWindowsMediaPlayer1.Ctlcontrols;
            IWMPMedia curTrack = cont.currentItem;
            for (int i = 0; i < plist.count; ++i)
            {
                if (plist.Item[i].isIdentical[curTrack])
                {
                    currentPos = i;
                    break;
                }
            }
            if (currentPos != listBox1.SelectedIndex)
                listBox1.SelectedIndex = currentPos;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StartSearch();
        }

        private void StartSearch()
        {
            backgroundWorker1.RunWorkerAsync();
            form2.ShowDialog();
        }

        private void ChangeSong()
        {
            axWindowsMediaPlayer1.Ctlcontrols.stop();
            if (listBox1.SelectedIndex == -1)
                return;
            int ind = listBox1.SelectedIndex;
            IWMPPlaylist plist = axWindowsMediaPlayer1.currentPlaylist;
            axWindowsMediaPlayer1.Ctlcontrols.playItem(plist.Item[ind]);
        }

        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void оПрограммеToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            aboutForm.ShowDialog();
        }

        private void RemoveTrack()
        {
            int index = listBox1.SelectedIndex;
            IWMPPlaylist plist = axWindowsMediaPlayer1.currentPlaylist;
            plist.removeItem(plist.Item[index]);
            listBox1.Items.RemoveAt(index);
            songs.RemoveAt(index);
            if (listBox1.Items.Count == 0)
                return;
            listBox1.SelectedIndex = index == 0 ? 0 : index - 1;
        }

        private void listBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
                RemoveTrack();
        }

        private void axWindowsMediaPlayer1_KeyDownEvent(object sender, AxWMPLib._WMPOCXEvents_KeyDownEvent e)
        {
            if (e.nKeyCode == 46)
                RemoveTrack();
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
                RemoveTrack();
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                StartSearch();
        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                StartSearch();
        }

        private void SavePlaylist()
        {
            if (songs == null)
            {
                MessageBox.Show("Список воспроизведения пуст!");
                return;
            }
            StreamWriter sw;
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.DefaultExt = "pls";
            sfd.Filter = "Playlist files (*.pls)|*.pls";
            sfd.FileName = "My Playlist";
            if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                sw = new StreamWriter(sfd.FileName);
                for (int i = 0; i < songs.Count; i++)
                    sw.WriteLine(songs[i].Url);
                sw.Close();
            }
        }

        private void OpenPlaylist()
        {
            StreamReader sr;
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.DefaultExt = "pls";
            ofd.Filter = "Playlist files (*.pls)|*.pls";
            ofd.Multiselect = false;
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                sr = new StreamReader(ofd.FileName);
                List<string> list = new List<string>();
                while (!sr.EndOfStream)
                    list.Add(sr.ReadLine());
                LoadSongsFromString(list.ToArray());
                sr.Close();
            }
        }

        private void LoadSongsFromString(string[] star)
        {
            songs = new List<Song>();
            int i = 0;
            foreach (var str in star)
            {
                string Name = str.Split('\\')[str.Split('\\').Length - 1];
                songs.Add(new Song(str, Name));
            }
            LoadSongs();
        }

        private void сохранитьСписокToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SavePlaylist();
        }

        private void открытьСписокToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenPlaylist();
            mainList.bIsLoaded = false;
        }

        private void listBox1_DoubleClick(object sender, EventArgs e)
        {
            ChangeLock = true;
            ChangeSong();
            ChangeLock = false;
        }

        private void открытьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeSong();
        }

        private void удалитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mainList.AddSong(songs[listBox1.SelectedIndex]);
        }

        private void удалитьToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            RemoveTrack();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            mainList.Save();
        }

        private void listBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Right)
                listBox1.SelectedIndex = listBox1.IndexFromPoint(e.Location);
        }

        private void моиАудиозаписиToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mainList.bIsLoaded)
                return;
            songs = mainList.songs;
            LoadSongs();
        }
    }
}
