namespace CinemasGUI
{
    partial class BuyTicketForm
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
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.phoneText = new System.Windows.Forms.TextBox();
            this.nameText = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.ticketTypeCombo = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.seansText = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.priceText = new System.Windows.Forms.TextBox();
            this.filmText = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.buyRadio = new System.Windows.Forms.RadioButton();
            this.bookRadio = new System.Windows.Forms.RadioButton();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(226, 101);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 10;
            this.button1.Text = "ОК";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // button2
            // 
            this.button2.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button2.Location = new System.Drawing.Point(307, 101);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 10;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 85);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(55, 13);
            this.label10.TabIndex = 27;
            this.label10.Text = "Телефон:";
            // 
            // phoneText
            // 
            this.phoneText.Location = new System.Drawing.Point(85, 82);
            this.phoneText.Name = "phoneText";
            this.phoneText.Size = new System.Drawing.Size(121, 20);
            this.phoneText.TabIndex = 25;
            // 
            // nameText
            // 
            this.nameText.Location = new System.Drawing.Point(85, 56);
            this.nameText.Name = "nameText";
            this.nameText.Size = new System.Drawing.Size(121, 20);
            this.nameText.TabIndex = 26;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(12, 59);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(32, 13);
            this.label9.TabIndex = 24;
            this.label9.Text = "Имя:";
            // 
            // ticketTypeCombo
            // 
            this.ticketTypeCombo.FormattingEnabled = true;
            this.ticketTypeCombo.Location = new System.Drawing.Point(85, 6);
            this.ticketTypeCombo.Name = "ticketTypeCombo";
            this.ticketTypeCombo.Size = new System.Drawing.Size(121, 21);
            this.ticketTypeCombo.TabIndex = 16;
            this.ticketTypeCombo.SelectedValueChanged += new System.EventHandler(this.ticketTypeCombo_SelectedValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 9);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 17;
            this.label5.Text = "Тип билета:";
            // 
            // seansText
            // 
            this.seansText.Location = new System.Drawing.Point(282, 33);
            this.seansText.Name = "seansText";
            this.seansText.ReadOnly = true;
            this.seansText.Size = new System.Drawing.Size(100, 20);
            this.seansText.TabIndex = 21;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(229, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(47, 13);
            this.label6.TabIndex = 18;
            this.label6.Text = "Фильм:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(229, 36);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(41, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "Сеанс:";
            // 
            // priceText
            // 
            this.priceText.Location = new System.Drawing.Point(282, 59);
            this.priceText.Name = "priceText";
            this.priceText.ReadOnly = true;
            this.priceText.Size = new System.Drawing.Size(100, 20);
            this.priceText.TabIndex = 22;
            // 
            // filmText
            // 
            this.filmText.Location = new System.Drawing.Point(282, 7);
            this.filmText.Name = "filmText";
            this.filmText.ReadOnly = true;
            this.filmText.Size = new System.Drawing.Size(100, 20);
            this.filmText.TabIndex = 23;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(229, 62);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(36, 13);
            this.label8.TabIndex = 20;
            this.label8.Text = "Цена:";
            // 
            // buyRadio
            // 
            this.buyRadio.AutoSize = true;
            this.buyRadio.Checked = true;
            this.buyRadio.Location = new System.Drawing.Point(15, 33);
            this.buyRadio.Name = "buyRadio";
            this.buyRadio.Size = new System.Drawing.Size(60, 17);
            this.buyRadio.TabIndex = 28;
            this.buyRadio.TabStop = true;
            this.buyRadio.Text = "Купить";
            this.buyRadio.UseVisualStyleBackColor = true;
            // 
            // bookRadio
            // 
            this.bookRadio.AutoSize = true;
            this.bookRadio.Location = new System.Drawing.Point(81, 33);
            this.bookRadio.Name = "bookRadio";
            this.bookRadio.Size = new System.Drawing.Size(56, 17);
            this.bookRadio.TabIndex = 29;
            this.bookRadio.Text = "Бронь";
            this.bookRadio.UseVisualStyleBackColor = true;
            this.bookRadio.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // BuyTicketForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(407, 136);
            this.Controls.Add(this.bookRadio);
            this.Controls.Add(this.buyRadio);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.phoneText);
            this.Controls.Add(this.nameText);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.ticketTypeCombo);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.seansText);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.priceText);
            this.Controls.Add(this.filmText);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Name = "BuyTicketForm";
            this.Text = "BuyTicketForm";
            this.Load += new System.EventHandler(this.BuyTicketForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox phoneText;
        private System.Windows.Forms.TextBox nameText;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox ticketTypeCombo;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox seansText;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox priceText;
        private System.Windows.Forms.TextBox filmText;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.RadioButton buyRadio;
        private System.Windows.Forms.RadioButton bookRadio;

    }
}