namespace CinemasGUI
{
    partial class Tickets
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
            this.label1 = new System.Windows.Forms.Label();
            this.datePicker = new System.Windows.Forms.DateTimePicker();
            this.label2 = new System.Windows.Forms.Label();
            this.cinemasCombo = new System.Windows.Forms.ComboBox();
            this.cinemasBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.cinemasDataSet1 = new CinemasGUI.CinemasDataSet1();
            this.label3 = new System.Windows.Forms.Label();
            this.hallsCombo = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.filmsCombo = new System.Windows.Forms.ComboBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.cinemasDataSet = new CinemasGUI.CinemasDataSet();
            this.ageLimitsBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.ageLimitsTableAdapter = new CinemasGUI.CinemasDataSetTableAdapters.AgeLimitsTableAdapter();
            this.cinemasTableAdapter = new CinemasGUI.CinemasDataSet1TableAdapters.CinemasTableAdapter();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasDataSet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasDataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ageLimitsBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Дата:";
            // 
            // datePicker
            // 
            this.datePicker.Location = new System.Drawing.Point(54, 12);
            this.datePicker.Name = "datePicker";
            this.datePicker.Size = new System.Drawing.Size(145, 20);
            this.datePicker.TabIndex = 1;
            this.datePicker.ValueChanged += new System.EventHandler(this.OnFilmNeededUpdate);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(218, 15);
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
            this.cinemasCombo.Location = new System.Drawing.Point(287, 12);
            this.cinemasCombo.Name = "cinemasCombo";
            this.cinemasCombo.Size = new System.Drawing.Size(121, 21);
            this.cinemasCombo.TabIndex = 3;
            this.cinemasCombo.ValueMember = "id";
            this.cinemasCombo.SelectedValueChanged += new System.EventHandler(this.OnFilmNeededUpdate);
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
            this.label3.Location = new System.Drawing.Point(610, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Зал:";
            // 
            // hallsCombo
            // 
            this.hallsCombo.FormattingEnabled = true;
            this.hallsCombo.Location = new System.Drawing.Point(645, 11);
            this.hallsCombo.Name = "hallsCombo";
            this.hallsCombo.Size = new System.Drawing.Size(121, 21);
            this.hallsCombo.TabIndex = 5;
            this.hallsCombo.SelectedValueChanged += new System.EventHandler(this.hallsCombo_SelectedValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(414, 15);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Фильм:";
            // 
            // filmsCombo
            // 
            this.filmsCombo.FormattingEnabled = true;
            this.filmsCombo.Location = new System.Drawing.Point(467, 12);
            this.filmsCombo.Name = "filmsCombo";
            this.filmsCombo.Size = new System.Drawing.Size(121, 21);
            this.filmsCombo.TabIndex = 7;
            this.filmsCombo.DataSourceChanged += new System.EventHandler(this.OnHallsNeedUpdate);
            this.filmsCombo.SelectedValueChanged += new System.EventHandler(this.OnHallsNeedUpdate);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToResizeColumns = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(12, 54);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.Size = new System.Drawing.Size(754, 397);
            this.dataGridView1.TabIndex = 8;
            this.dataGridView1.CellMouseDoubleClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView1_CellMouseDoubleClick);
            // 
            // cinemasDataSet
            // 
            this.cinemasDataSet.DataSetName = "CinemasDataSet";
            this.cinemasDataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // ageLimitsBindingSource
            // 
            this.ageLimitsBindingSource.DataMember = "AgeLimits";
            this.ageLimitsBindingSource.DataSource = this.cinemasDataSet;
            // 
            // ageLimitsTableAdapter
            // 
            this.ageLimitsTableAdapter.ClearBeforeFill = true;
            // 
            // cinemasTableAdapter
            // 
            this.cinemasTableAdapter.ClearBeforeFill = true;
            // 
            // Tickets
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(774, 463);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.filmsCombo);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.hallsCombo);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cinemasCombo);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.datePicker);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Tickets";
            this.Text = "Tickets";
            this.Load += new System.EventHandler(this.Tickets_Load);
            ((System.ComponentModel.ISupportInitialize)(this.cinemasBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasDataSet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cinemasDataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ageLimitsBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DateTimePicker datePicker;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cinemasCombo;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox hallsCombo;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox filmsCombo;
        private System.Windows.Forms.DataGridView dataGridView1;
        private CinemasDataSet cinemasDataSet;
        private System.Windows.Forms.BindingSource ageLimitsBindingSource;
        private CinemasDataSetTableAdapters.AgeLimitsTableAdapter ageLimitsTableAdapter;
        private CinemasDataSet1 cinemasDataSet1;
        private System.Windows.Forms.BindingSource cinemasBindingSource;
        private CinemasDataSet1TableAdapters.CinemasTableAdapter cinemasTableAdapter;
    }
}