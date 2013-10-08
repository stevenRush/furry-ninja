using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace CinemasGUI
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load( object sender, EventArgs e )
        {
        }

        private void button1_Click( object sender, EventArgs e )
        {

        }

        private void cinemasToolStripMenuItem_Click( object sender, EventArgs e )
        {
            showTablesForm(DatabaseTables.dtCinemas);
        }

        private void showTablesForm(DatabaseTables table)
        {
            TablesForm form = new TablesForm( table );
            form.MdiParent = this;
            form.Show();
        }

        private void ageLimitsToolStripMenuItem_Click( object sender, EventArgs e )
        {
            showTablesForm(DatabaseTables.dtAgeLimit);
        }

        private void genresToolStripMenuItem_Click( object sender, EventArgs e )
        {
            showTablesForm(DatabaseTables.dtGenres);
        }

        private void formatsToolStripMenuItem_Click( object sender, EventArgs e )
        {
            showTablesForm(DatabaseTables.dtFormats);
        }

        private void типыЗаловToolStripMenuItem_Click( object sender, EventArgs e )
        {
            showTablesForm(DatabaseTables.dtHallTypes);
        }

        private void залыToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showTablesForm(DatabaseTables.dtHalls);
        }

        private void типыБилетовToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showTablesForm(DatabaseTables.dtTicketTypes);
        }

        private void типыМестToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showTablesForm(DatabaseTables.dtSeatTypes);
        }

        private void фильмыToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showTablesForm(DatabaseTables.dtFilms);
        }

        private void ценовыеПолитикиToolStripMenuItem_Click(object sender, EventArgs e)
        {
            showTablesForm(DatabaseTables.dtPricePolicies);
        }

        private void местаToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Tickets ticketsForm = new Tickets();
            ticketsForm.MdiParent = this;
            ticketsForm.Show();
        }

        private void расписаниеФильмовToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TimeTableForm form = new TimeTableForm();
            form.MdiParent = this;
            form.Show();
        }

        private void reportsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void фильмыToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            showTablesForm(DatabaseTables.dtFilms);
        }
    }
}
