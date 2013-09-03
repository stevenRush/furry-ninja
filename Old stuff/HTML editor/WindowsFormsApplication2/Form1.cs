using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        StreamReader f;
        string[][] lst;
        string[] items;
        string st, path = "C:\\list.txt", 
            tmp, buf;
        int n, beg, selected;
        bool flag, flagEnd = false,
            flagBuf = false, flagTag = false;
        char key;
        public Form1()
        {
            string tmp;
            InitializeComponent();
            webBrowser1.Navigate("about:blank");
            f = new StreamReader(path);
            while (!f.EndOfStream)
            {
                f.ReadLine();
                n++;
            }
            f.Close();
            f = new StreamReader(path);
            lst = new string[n][];
            items = new string[n];
            for (int i = 0; i < n; i++)
            {
                tmp = f.ReadLine();
                lst[i] = new string[tmp.Split().Length-1];
                items[i] = tmp.Split(' ')[0];
                for (int j = 0; j < lst[i].Length; j++)
                    lst[i][j] = tmp.Split(' ')[j+1];
            }           
        }

        private void button1_Click(object sender, EventArgs e)
        {
            webBrowser1.DocumentText = textBox1.Text;
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (flagBuf)
            {
                if (e.KeyChar == buf[0])
                {
                    buf = buf.Substring(1);
                    textBox1.Paste(e.KeyChar.ToString() + buf);
                    textBox1.SelectionStart = textBox1.SelectionStart - buf.Length;
                    textBox1.SelectionLength = buf.Length;
                    if (buf.Length == 0)
                        flagBuf = false;
                    e.Handled = true;
                }
                else
                    flagBuf = false;
                if ((int)e.KeyChar == 13)
                {
                    textBox1.SelectionStart = textBox1.TextLength;
                    flagBuf = false;
                    e.Handled = true;
                }
            }
            switch (e.KeyChar)
            {
                case '<':
                    flag = true;
                    beg = textBox1.SelectionStart + 1;
                    flagTag = true;
                    break;
                case '>':
                    if (flag)
                    {
                        tmp = textBox1.Text.Substring(beg, textBox1.SelectionStart - beg);
                        flag = false;
                    }
                    flagTag = false;
                    break;
                case ' ':
                    if (flag)
                    {
                        tmp = textBox1.Text.Substring(beg, textBox1.SelectionStart - beg);
                        flag = false;
                    }
                    if (flagTag)
                    {
                        for (int i = 0; i < items.Length; i++)
                            if (items[i] == tmp)
                            {
                                listBox1.Items.Clear();
                                listBox1.Items.AddRange(lst[i]);
                                listBox1.Location = new Point(textBox1.GetPositionFromCharIndex(textBox1.SelectionStart - 1).X + textBox1.Location.X + 10,
                                                              textBox1.GetPositionFromCharIndex(textBox1.SelectionStart - 1).Y + textBox1.Location.Y + textBox1.Font.Height + 5);
                                listBox1.Visible = true;
                                listBox1.SelectedIndex = 0;
                                listBox1.Focus();
                                st = "";
                                selected = i;
                            }
                    }
                    break;
                case '=':
                    break;
                case '/':
                    if (textBox1.TextLength > 1 && textBox1.Text[textBox1.SelectionStart - 1] == '<')
                    {
                        flagEnd = true;
                    }
                    if (flag)
                        flag = false;
                    break;
            }

        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            textBox1.Focus();
        }

        private void listBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (char.IsLetterOrDigit(e.KeyChar))
            {
                textBox1.AppendText(e.KeyChar.ToString());
                st = st + e.KeyChar.ToString();
                listBox1.Items.Clear();
                for(int i=0; i < lst[selected].Length; i++)
                {
                    if (lst[selected][i].StartsWith(st))
                        listBox1.Items.Add(lst[selected][i]);
                }
                listBox1.SelectedIndex = listBox1.FindString(st);
                e.KeyChar = ' ';
            }
            if ((int)e.KeyChar == 0x8)
            {
                if (st != "") st = st.Substring(0, st.Length - 1);
                if (textBox1.Text != "") textBox1.Text = textBox1.Text.Substring(0, textBox1.TextLength - 1);
                listBox1.Items.Clear();
                for (int i = 0; i < lst[selected].Length; i++)
                {
                    if (lst[selected][i].StartsWith(st))
                        listBox1.Items.Add(lst[selected][i]);
                }
                listBox1.SelectedIndex = listBox1.FindString(st);
                if (st == "")
                {
                    listBox1.Visible = false;
                    textBox1.Focus();
                }
            }
            if ((int)e.KeyChar == 0x0D || e.KeyChar == '=')
            {
                if (textBox1.Text != "") textBox1.Text = textBox1.Text.Substring(0, textBox1.TextLength - st.Length);
                textBox1.AppendText(listBox1.SelectedItem.ToString());
                if (e.KeyChar == '=') textBox1.AppendText(e.KeyChar.ToString());
                listBox1.Visible = false;
                textBox1.Focus();
            }

        
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string temp;
            int k = 0, i;
            
            if (flagEnd)
            {
                flagEnd = false;
                temp = textBox1.Text.Substring(0, textBox1.TextLength - 2);
                for (i = temp.Length - 1; i >= 0; i--)
                {
                    if (temp[i] == '<')
                    {
                        if (i < temp.Length - 1 && temp[i + 1] == '/')
                            k++;
                        if (k == 0)
                            break;
                        else
                            k--;
                    }
                }
                k = i;
                try
                {
                    while (temp[++k] != '>' && temp[k] != ' ')
                        if (temp[k] == '<')
                            throw new IndexOutOfRangeException();
                }
                catch (IndexOutOfRangeException)
                {
                    return;
                }
                temp = temp.Substring(++i, k - i);
                k = textBox1.SelectionStart;
                buf = temp + ">";
                textBox1.Paste(buf);
                textBox1.SelectionStart = k;
                textBox1.SelectionLength = buf.Length;
                flagBuf = true;
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (richTextBox1.SelectedText != "")
            {
                richTextBox1.SelectionColor = Color.Red;
            }
        }
    }
}
