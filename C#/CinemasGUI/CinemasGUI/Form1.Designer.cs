namespace CinemasGUI
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose( bool disposing )
        {
            if ( disposing && ( components != null ) )
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.reportsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.serviceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cinemasToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.типыЗаловToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.залыToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.типыБилетовToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.типыМестToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.местаToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.расписаниеФильмовToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.formatsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.genresToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ageLimitsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.фильмыToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ценовыеПолитикиToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.serviceToolStripMenuItem,
            this.reportsToolStripMenuItem,
            this.местаToolStripMenuItem,
            this.расписаниеФильмовToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(729, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // reportsToolStripMenuItem
            // 
            this.reportsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.фильмыToolStripMenuItem,
            this.formatsToolStripMenuItem,
            this.genresToolStripMenuItem,
            this.ageLimitsToolStripMenuItem,
            this.ценовыеПолитикиToolStripMenuItem});
            this.reportsToolStripMenuItem.Name = "reportsToolStripMenuItem";
            this.reportsToolStripMenuItem.Size = new System.Drawing.Size(66, 20);
            this.reportsToolStripMenuItem.Text = "Фильмы";
            this.reportsToolStripMenuItem.Click += new System.EventHandler(this.reportsToolStripMenuItem_Click);
            // 
            // serviceToolStripMenuItem
            // 
            this.serviceToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cinemasToolStripMenuItem,
            this.залыToolStripMenuItem,
            this.toolStripSeparator1,
            this.типыЗаловToolStripMenuItem,
            this.типыБилетовToolStripMenuItem,
            this.типыМестToolStripMenuItem});
            this.serviceToolStripMenuItem.Name = "serviceToolStripMenuItem";
            this.serviceToolStripMenuItem.Size = new System.Drawing.Size(111, 20);
            this.serviceToolStripMenuItem.Text = "Инфраструктура";
            // 
            // cinemasToolStripMenuItem
            // 
            this.cinemasToolStripMenuItem.Name = "cinemasToolStripMenuItem";
            this.cinemasToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.cinemasToolStripMenuItem.Text = "Кинотеатры";
            this.cinemasToolStripMenuItem.Click += new System.EventHandler(this.cinemasToolStripMenuItem_Click);
            // 
            // типыЗаловToolStripMenuItem
            // 
            this.типыЗаловToolStripMenuItem.Name = "типыЗаловToolStripMenuItem";
            this.типыЗаловToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.типыЗаловToolStripMenuItem.Text = "Типы залов";
            this.типыЗаловToolStripMenuItem.Click += new System.EventHandler(this.типыЗаловToolStripMenuItem_Click);
            // 
            // залыToolStripMenuItem
            // 
            this.залыToolStripMenuItem.Name = "залыToolStripMenuItem";
            this.залыToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.залыToolStripMenuItem.Text = "Залы";
            this.залыToolStripMenuItem.Click += new System.EventHandler(this.залыToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(175, 6);
            // 
            // типыБилетовToolStripMenuItem
            // 
            this.типыБилетовToolStripMenuItem.Name = "типыБилетовToolStripMenuItem";
            this.типыБилетовToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.типыБилетовToolStripMenuItem.Text = "Типы билетов";
            this.типыБилетовToolStripMenuItem.Click += new System.EventHandler(this.типыБилетовToolStripMenuItem_Click);
            // 
            // типыМестToolStripMenuItem
            // 
            this.типыМестToolStripMenuItem.Name = "типыМестToolStripMenuItem";
            this.типыМестToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
            this.типыМестToolStripMenuItem.Text = "Типы мест";
            this.типыМестToolStripMenuItem.Click += new System.EventHandler(this.типыМестToolStripMenuItem_Click);
            // 
            // местаToolStripMenuItem
            // 
            this.местаToolStripMenuItem.Name = "местаToolStripMenuItem";
            this.местаToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.местаToolStripMenuItem.Text = "Места";
            this.местаToolStripMenuItem.Click += new System.EventHandler(this.местаToolStripMenuItem_Click);
            // 
            // расписаниеФильмовToolStripMenuItem
            // 
            this.расписаниеФильмовToolStripMenuItem.Name = "расписаниеФильмовToolStripMenuItem";
            this.расписаниеФильмовToolStripMenuItem.Size = new System.Drawing.Size(138, 20);
            this.расписаниеФильмовToolStripMenuItem.Text = "Расписание фильмов";
            this.расписаниеФильмовToolStripMenuItem.Click += new System.EventHandler(this.расписаниеФильмовToolStripMenuItem_Click);
            // 
            // formatsToolStripMenuItem
            // 
            this.formatsToolStripMenuItem.Name = "formatsToolStripMenuItem";
            this.formatsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.formatsToolStripMenuItem.Text = "Форматы";
            this.formatsToolStripMenuItem.Click += new System.EventHandler(this.formatsToolStripMenuItem_Click);
            // 
            // genresToolStripMenuItem
            // 
            this.genresToolStripMenuItem.Name = "genresToolStripMenuItem";
            this.genresToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.genresToolStripMenuItem.Text = "Жанры";
            this.genresToolStripMenuItem.Click += new System.EventHandler(this.genresToolStripMenuItem_Click);
            // 
            // ageLimitsToolStripMenuItem
            // 
            this.ageLimitsToolStripMenuItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.ageLimitsToolStripMenuItem.Name = "ageLimitsToolStripMenuItem";
            this.ageLimitsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.ageLimitsToolStripMenuItem.Text = "Возр. ограничения";
            this.ageLimitsToolStripMenuItem.Click += new System.EventHandler(this.ageLimitsToolStripMenuItem_Click);
            // 
            // фильмыToolStripMenuItem
            // 
            this.фильмыToolStripMenuItem.Name = "фильмыToolStripMenuItem";
            this.фильмыToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.фильмыToolStripMenuItem.Text = "Список фильмов";
            this.фильмыToolStripMenuItem.Click += new System.EventHandler(this.фильмыToolStripMenuItem_Click_1);
            // 
            // ценовыеПолитикиToolStripMenuItem
            // 
            this.ценовыеПолитикиToolStripMenuItem.Name = "ценовыеПолитикиToolStripMenuItem";
            this.ценовыеПолитикиToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.ценовыеПолитикиToolStripMenuItem.Text = "Ценовые политики";
            this.ценовыеПолитикиToolStripMenuItem.Click += new System.EventHandler(this.ценовыеПолитикиToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(729, 394);
            this.Controls.Add(this.menuStrip1);
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Cinemas";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem reportsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem serviceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cinemasToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem типыЗаловToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem залыToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem типыБилетовToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem типыМестToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem местаToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem расписаниеФильмовToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem фильмыToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem formatsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem genresToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ageLimitsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ценовыеПолитикиToolStripMenuItem;
    }
}

