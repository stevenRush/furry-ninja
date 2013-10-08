using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CinemasGUI
{
    public partial class BuyTicketForm : Form
    {
        private Int64 seatId;
        private int filmId;
        private BuyTicketSQLWorker worker = new BuyTicketSQLWorker();

        public BuyTicketForm(Int64 seatId, int filmId)
        {
            InitializeComponent();
            this.seatId = seatId;
            this.filmId = filmId;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (bookRadio.Checked == true)
            {
                
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
            }
            else
            {
                
                this.DialogResult = System.Windows.Forms.DialogResult.Yes;
            }
            Close();
        }

        private void BuyTicketForm_Load(object sender, EventArgs e)
        {
            worker.UpdateTicketTypesCombo(this.filmId, ticketTypeCombo);
            UpdateBookVisible();
            filmText.Text = worker.GetFilmTitle(this.filmId);
            seansText.Text = worker.GetSeansTitle(this.filmId);
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            UpdateBookVisible();
        }

        private void UpdateBookVisible()
        {
            label9.Visible = bookRadio.Checked;
            label10.Visible = bookRadio.Checked;
            nameText.Visible = bookRadio.Checked;
            phoneText.Visible = bookRadio.Checked;
        }

        private void ticketTypeCombo_SelectedValueChanged(object sender, EventArgs e)
        {
            if (ticketTypeCombo.SelectedIndex == -1)
            {
                priceText.Text = string.Empty;
                return;
            }
            priceText.Text = worker.GetPrice((int)ticketTypeCombo.SelectedValue);
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if (bookRadio.Checked)
            {
                worker.BookSeat(this.seatId, this.filmId, nameText.Text, phoneText.Text);
                DialogResult = System.Windows.Forms.DialogResult.OK;
            }
            else
            {
                worker.BuyTicket(this.seatId, this.filmId, (int)ticketTypeCombo.SelectedValue);
                DialogResult = System.Windows.Forms.DialogResult.Yes;
            }
        }
    }
}
