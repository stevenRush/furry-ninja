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
    public partial class TablesForm : Form
    {
        private SQLWorkerBase worker;

        public TablesForm(DatabaseTables table)
        {  
            InitializeComponent();
            worker = SQLWorkerManager.GetWorker(table);
            this.Text = worker.GetTableName();
        }

        private void button1_Click( object sender, EventArgs e )
        {
            worker.UpdateData(dataGridView1);
            Close();
        }

        private void button2_Click( object sender, EventArgs e )
        {
            Close();
        }

        private void TablesForm_Load(object sender, EventArgs e)
        {
            worker.InitializeDataGridView( dataGridView1 );
        }
    }
}
