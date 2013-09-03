using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        Form1 owner;
        public Form2(Form1 f)
        {
            InitializeComponent();
            this.Owner = f;
            owner = f;
            webBrowser1.DocumentText = f.docText;
        }

        public void setWebBrowserText(string st)
        {
            webBrowser1.DocumentText = st;
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            Size scr = SystemInformation.PrimaryMonitorSize;
            if (scr.Width - (this.Owner.Left + this.Owner.Width) > this.Width + 30)
            {
                this.Left = this.Owner.Left + this.Owner.Width + 30;
                this.Top = this.Owner.Top;
            }
            else if (scr.Height - (this.Owner.Top + this.Owner.Right) > this.Height + 30)
            {
                this.Top = this.Owner.Top + this.Owner.Height + 30;
                this.Left = this.Owner.Left;
            }
            else if (this.Owner.Left - this.Width > 30)
            {
                this.Left = this.Owner.Left - 30 - this.Width;
                this.Top = this.Owner.Top;
            }
            else if (this.Owner.Top - this.Height > 30)
            {
                this.Top = this.Owner.Top - 30 - this.Height;
                this.Left = this.Owner.Left;
            }
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            owner.setStatus("Готово");
        }

        private void Form2_Activated(object sender, EventArgs e)
        {
            owner.setStatus("Просмотр страницы");
        }
    }
}
