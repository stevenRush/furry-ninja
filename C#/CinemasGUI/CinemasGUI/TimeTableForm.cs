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
    public partial class TimeTableForm : Form
    {
        TimeTableSQLWorker worker = new TimeTableSQLWorker();

        public TimeTableForm()
        {
            InitializeComponent();
            worker.UpdateFilmsCombo( filmsCombo );
            worker.UpdateDataGridView( dataGridView1 );
        }

        private void TimeTableForm_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'cinemasDataSet1.Cinemas' table. You can move, or remove it, as needed.
            this.cinemasTableAdapter.Fill( this.cinemasDataSet1.Cinemas );
        }

        private void cinemasCombo_SelectedValueChanged(object sender, EventArgs e)
        {
            if (cinemasCombo.SelectedIndex == -1)
                worker.UpdateHallsCombo(hallsCombo, -1);
            else
                worker.UpdateHallsCombo( hallsCombo, ( int ) cinemasCombo.SelectedValue );
        }

        private void OnFilterNeeded(object sender, EventArgs e)
        {
            if (filmsCombo.SelectedIndex == -1 ||
                hallsCombo.SelectedIndex == -1)
            {
                worker.FilterDataGrid(dataGridView1, -1, -1);
            }
            else
            {
                worker.FilterDataGrid(  dataGridView1, 
                                        (int)filmsCombo.SelectedValue, 
                                        (int)hallsCombo.SelectedValue);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            worker.UpdateData();
            Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void dataGridView1_UserAddedRow(object sender, DataGridViewRowEventArgs e)
        {
            if ( filmsCombo.SelectedIndex == -1 ||
                hallsCombo.SelectedIndex == -1 )
            {
                MessageBox.Show("Выберите фильм и зал!");
                dataGridView1.Rows.Remove(e.Row);
                return;
            }
            DataGridViewRow row = dataGridView1.Rows[e.Row.Index-1];
            row.Cells["film_format_id"].Value = filmsCombo.SelectedValue;
            row.Cells["hall_id"].Value = hallsCombo.SelectedValue;
        }
    }
}
