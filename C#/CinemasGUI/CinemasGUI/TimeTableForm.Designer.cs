namespace CinemasGUI
{
    partial class TimeTableForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
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
            this.components = new System.ComponentModel.Container();
            this.filmsCombo = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cinemasCombo = new System.Windows.Forms.ComboBox();
            this.cinemasBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.cinemasDataSet1 = new CinemasGUI.CinemasDataSet1();
            this.label3 = new System.Windows.Forms.Label();
            this.hallsCombo = new System.Windows.Forms.ComboBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.cinemasTableAdapter = new CinemasGUI.CinemasDataSet1TableAdapters.CinemasTableAdapter();
            this.filmFormatsBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.filmFormatsTableAdapter = new CinemasGUI.CinemasDataSet2TableAdapters.FilmFormatsTableAdapter();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasDataSet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.filmFormatsBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // filmsCombo
            // 
            this.filmsCombo.FormattingEnabled = true;
            this.filmsCombo.Location = new System.Drawing.Point(86, 12);
            this.filmsCombo.Name = "filmsCombo";
            this.filmsCombo.Size = new System.Drawing.Size(157, 21);
            this.filmsCombo.TabIndex = 0;
            this.filmsCombo.SelectedValueChanged += new System.EventHandler(this.OnFilterNeeded);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Фильм:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 42);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Кинотеатр:";
            // 
            // cinemasCombo
            // 
            this.cinemasCombo.DataSource = this.cinemasBindingSource;
            this.cinemasCombo.DisplayMember = "title";
            this.cinemasCombo.FormattingEnabled = true;
            this.cinemasCombo.Location = new System.Drawing.Point(86, 39);
            this.cinemasCombo.Name = "cinemasCombo";
            this.cinemasCombo.Size = new System.Drawing.Size(157, 21);
            this.cinemasCombo.TabIndex = 3;
            this.cinemasCombo.ValueMember = "id";
            this.cinemasCombo.SelectedValueChanged += new System.EventHandler(this.cinemasCombo_SelectedValueChanged);
            // 
            // cinemasBindingSource
            // 
            this.cinemasBindingSource.DataMember = "Cinemas";
            this.cinemasBindingSource.DataSource = this.cinemasDataSet1;
            // 
            // cinemasDataSet1
            // 
            this.cinemasDataSet1.DataSetName = "CinemasDataSet1";
            this.cinemasDataSet1.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(11, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Зал:";
            // 
            // hallsCombo
            // 
            this.hallsCombo.FormattingEnabled = true;
            this.hallsCombo.Location = new System.Drawing.Point(86, 66);
            this.hallsCombo.Name = "hallsCombo";
            this.hallsCombo.Size = new System.Drawing.Size(157, 21);
            this.hallsCombo.TabIndex = 5;
            this.hallsCombo.SelectedValueChanged += new System.EventHandler(this.OnFilterNeeded);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(249, 12);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(168, 250);
            this.dataGridView1.TabIndex = 6;
            this.dataGridView1.UserAddedRow += new System.Windows.Forms.DataGridViewRowEventHandler(this.dataGridView1_UserAddedRow);
            // 
            // cinemasTableAdapter
            // 
            this.cinemasTableAdapter.ClearBeforeFill = true;
            // 
            // filmFormatsTableAdapter
            // 
            this.filmFormatsTableAdapter.ClearBeforeFill = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(87, 239);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 7;
            this.button1.Text = "OK";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(168, 239);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 8;
            this.button2.Text = "Отмена";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // TimeTableForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(429, 272);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.hallsCombo);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cinemasCombo);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.filmsCombo);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "TimeTableForm";
            this.Text = "TimeTableForm";
            this.Load += new System.EventHandler(this.TimeTableForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.cinemasBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasDataSet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.filmFormatsBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox filmsCombo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cinemasCombo;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox hallsCombo;
        private System.Windows.Forms.DataGridView dataGridView1;
        private CinemasDataSet1 cinemasDataSet1;
        private System.Windows.Forms.BindingSource cinemasBindingSource;
        private CinemasDataSet1TableAdapters.CinemasTableAdapter cinemasTableAdapter;
        private System.Windows.Forms.BindingSource filmFormatsBindingSource;
        private CinemasDataSet2TableAdapters.FilmFormatsTableAdapter filmFormatsTableAdapter;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
    }
}