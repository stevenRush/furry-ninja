using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WindowsFormsApplication1
{
    public partial class Form4 : Form
    {
        HashSet<int> set;
        Form1 owner;
        Color tg = new Color(),
            atr = new Color(),
            tx = new Color(),
            bg = new Color();
        public Form4(Form1 f)
        {
            InitializeComponent();
            owner = f;
            set = new HashSet<int>();
            set.Clear();
            this.Owner = f;
            foreach (string line in WindowsFormsApplication1.Properties.Settings.Default.templates)
                if (line != "HTML")
                    listBox1.Items.Add(line);
            richTextBox1.Font = WindowsFormsApplication1.Properties.Settings.Default.mainFont;
            tg = WindowsFormsApplication1.Properties.Settings.Default.tagColor;
            atr = WindowsFormsApplication1.Properties.Settings.Default.atrColor;
            tx = WindowsFormsApplication1.Properties.Settings.Default.textColor;
            bg = WindowsFormsApplication1.Properties.Settings.Default.bgColor;
   
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedItem == null)
                return;
            StreamReader sr;
            try
            {
                sr = new StreamReader(listBox1.SelectedItem.ToString().Replace(" ", string.Empty) + ".txt");
            }
            catch (Exception e1)
            {
                MessageBox.Show(e1.Message);
                return;
            }
            richTextBox1.Text = sr.ReadToEnd();
            sr.Close();
            owner.LightText(richTextBox1);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            set.Add(listBox1.SelectedIndex + 1);
            listBox1.Items.RemoveAt(listBox1.SelectedIndex);
            richTextBox1.Text = "";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            set.Clear();
            owner.reloadMenu();
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                foreach (int i in set)
                {
                    File.Delete(WindowsFormsApplication1.Properties.Settings.Default.templates[i].Replace(" ", string.Empty) + ".txt");
                    WindowsFormsApplication1.Properties.Settings.Default.templates.RemoveAt(i);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            set.Clear();
            WindowsFormsApplication1.Properties.Settings.Default.Save();
            owner.reloadMenu();
            this.Close();
        }
    }
}
