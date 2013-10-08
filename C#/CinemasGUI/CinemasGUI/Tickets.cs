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
    public partial class Tickets : Form
    {
        TicketsSQLWorker worker = new TicketsSQLWorker();
        
        public Tickets()
        {
            InitializeComponent();
        }

        private void Tickets_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'cinemasDataSet1.Cinemas' table. You can move, or remove it, as needed.
            this.cinemasTableAdapter.Fill( this.cinemasDataSet1.Cinemas );
            // TODO: This line of code loads data into the 'cinemasDataSet.AgeLimits' table. You can move, or remove it, as needed.
            this.ageLimitsTableAdapter.Fill( this.cinemasDataSet.AgeLimits );

        }

        private void OnFilmNeededUpdate(object sender, EventArgs e)
        {
            if (cinemasCombo.SelectedIndex == -1)
                worker.UpdateFilmsCombo(datePicker.Value, -1, filmsCombo);
            else
                worker.UpdateFilmsCombo(datePicker.Value, (int)cinemasCombo.SelectedValue, filmsCombo);
        }

        private void OnHallsNeedUpdate(object sender, EventArgs e)
        {
            if (filmsCombo.SelectedIndex == -1 ||
                cinemasCombo.SelectedIndex == -1)
                worker.UpdateHallsCombo(datePicker.Value, -1, -1, hallsCombo);
            else
                worker.UpdateHallsCombo(datePicker.Value, 
                                        (int)filmsCombo.SelectedValue, 
                                        (int)cinemasCombo.SelectedValue, 
                                        hallsCombo);
        }

        private void hallsCombo_SelectedValueChanged(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            dataGridView1.Columns.Clear();

            if (hallsCombo.SelectedIndex == -1 || hallsCombo.DataSource == null)
            {
                return;
            }

            // rows, seats
            Tuple<int, int> sizes = worker.GetHallSize((int)hallsCombo.SelectedValue);
            for(int i = 0; i < sizes.Item2; ++i)
            {
                DataGridViewColumn column = new DataGridViewTextBoxColumn();
                column.HeaderText = (i+1).ToString();  
                column.AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
                column.HeaderCell.Style.Alignment = DataGridViewContentAlignment.TopCenter;
                dataGridView1.Columns.Add(column);
            }

            for(int i = 0; i < sizes.Item1; ++i)
            {
                DataGridViewRow row = new DataGridViewRow();
                row.HeaderCell.Value = string.Format("{0}", i+1);
                row.Height = dataGridView1.Columns[0].Width;
                dataGridView1.Rows.Add(row);
                DataGridViewRow insertedRow = dataGridView1.Rows[dataGridView1.Rows.Count-1];
                for(int j = 0; j < dataGridView1.Columns.Count; ++j)
                {
                    DataGridViewCell cell = insertedRow.Cells[j];
                    cell.Value = j + 1;
                    cell.Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
                }
            }
            dataGridView1.Height = 40 + dataGridView1.Rows.Count * dataGridView1.Rows[0].Height;
            this.Height = dataGridView1.Height + dataGridView1.Top + 20;

            int filmId = (int)hallsCombo.SelectedValue;

            foreach(Tuple<int, int> pair in worker.BusyPlaces(filmId))
            {
                dataGridView1.Rows[pair.Item1-1].Cells[pair.Item2-1].Style.BackColor = Color.LightGreen;
            }

            foreach(Tuple<int, int> pair in worker.BookedPlaces(filmId))
            {
                dataGridView1.Rows[pair.Item1-1].Cells[pair.Item2-1].Style.BackColor = Color.LightGray;
            }

        }

        private void dataGridView1_CellMouseDoubleClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            int filmId = (int)hallsCombo.SelectedValue;
            int hallId = worker.GetHallId(filmId);
            Int64 seatId = worker.GetSeatId(e.RowIndex+1, e.ColumnIndex+1, hallId);
            BuyTicketForm form = new BuyTicketForm(seatId, filmId);
            DataGridViewCell cell = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex];
            switch(form.ShowDialog())
            {
                // ok = book. Yes = buy
                case System.Windows.Forms.DialogResult.OK:
                    cell.Style.BackColor = Color.LightGray;
                    break;
                case System.Windows.Forms.DialogResult.Yes:
                    cell.Style.BackColor = Color.LightGreen;
                    break;
            }
        }
    }
}
