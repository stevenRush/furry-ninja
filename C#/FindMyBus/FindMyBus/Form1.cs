using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;

namespace FindMyBus
{
    public partial class Form1 : Form
    {
        const string pathToJson = @"http://www.map.gptperm.ru/json";
        Routes rt;
        Route activeRoute;
        RouteDirection direction;
        Halt h;
        TimeTable timeTable;
        bool m_LockTimeTable = false;


        public Form1()
        {
            InitializeComponent();
            Bitmap bmp = FindMyBus.Properties.Resources.avtobus;
            Icon = Icon.FromHandle(bmp.GetHicon());
        }

        private WebExceptionStatus CheckServer()
        {
            string req = String.Format("{0}/{1}/{2}/{3}/", pathToJson, "routes", GetToday(), GetTransp());
            WebRequest.DefaultWebProxy = new WebProxy();
            try
            {
                WebRequest reqGET = WebRequest.Create(req);
                reqGET.Timeout = 1000;
                WebResponse resp = reqGET.GetResponse();
            }
            catch (WebException e)
            {
                return e.Status;
            }
            return WebExceptionStatus.Success;
        }

        private string GetToday()
        {
            return DateTime.Today.ToShortDateString();
        }

        private string GetTransp()
        {
            if (radioButton1.Checked)
                return "0";
            if (radioButton2.Checked)
                return "1";
            if (radioButton3.Checked)
                return "2";
            return null;
        }

        private string GetRoutes()
        {
            string req = String.Format("{0}/{1}/{2}/{3}/", pathToJson, "routes", GetToday(), GetTransp());
            WebRequest.DefaultWebProxy = new WebProxy();
            WebRequest reqGET = WebRequest.Create(req);
            WebResponse resp = reqGET.GetResponse();
            Stream stream = resp.GetResponseStream();
            StreamReader sr = new StreamReader(stream);
            return sr.ReadToEnd();
        }

        private void LoadRoutes()
        {
            m_LockTimeTable = true;
            rt = new Routes();
            rt.Parse(GetRoutes());
            dataGridView1.Rows.Clear();
            for (int i = 0, c = rt.RoutesCount(); i < c; i++)
            {
                Route r = rt[i];
                dataGridView1.Rows.Add(r.Id, r.Number, r.Name);
                dataGridView1.Rows[i].Resizable = DataGridViewTriState.False;
            }
            m_LockTimeTable = false;
        }

        private void TranspChanged(object sender, EventArgs e)
        {
            LoadRoutes();
        }

        private void UpdateDirection()
        {
            direction = radioButton5.Checked ? RouteDirection.Forward : RouteDirection.Backward;
        }

        private void UpdateActiveRoute()
        {
            if (dataGridView1.SelectedRows.Count == 0)
                return;
            string routeId = (string)dataGridView1.SelectedRows[0].Cells["Id"].Value;
            activeRoute = rt.GetById(routeId);
        }

        private void UpdateHalt()
        {
            if (dataGridView2.SelectedRows.Count == 0)
                return;
            string haltId = (string)dataGridView2.SelectedRows[0].Cells["HaltId"].Value;
            h = activeRoute.GetHaltById(haltId);
        }

        private void RouteChanged(object sender, EventArgs e)
        {
            UpdateActiveRoute();
            string req = String.Format("{0}/{1}/{2}/{3}/", pathToJson, "route", GetToday(), activeRoute.Id);
            WebRequest.DefaultWebProxy = new WebProxy();
            WebRequest reqGET = WebRequest.Create(req);
            WebResponse resp = reqGET.GetResponse();
            Stream stream = resp.GetResponseStream();
            StreamReader sr = new StreamReader(stream);
            string json = sr.ReadToEnd();
            activeRoute.Parse(json);
            string start, finish;
            try
            {
                start = activeRoute.GetHalt(RouteDirection.Forward, 0).Name;
                finish = activeRoute.GetHalt(RouteDirection.Forward, activeRoute.GetHaltCount(RouteDirection.Forward) - 1).Name;
                radioButton5.Text = String.Format("{0} - {1}", start, finish);

                start = activeRoute.GetHalt(RouteDirection.Backward, 0).Name;
                finish = activeRoute.GetHalt(RouteDirection.Backward, activeRoute.GetHaltCount(RouteDirection.Backward) - 1).Name;
                radioButton6.Text = String.Format("{0} - {1}", start, finish);
                radioButton5.Enabled = true;
                radioButton6.Enabled = true;
                radioButton5.Checked = true;
            }
            catch
            {
                radioButton5.Text = "Остановок нет";
                radioButton6.Text = "Остановок нет";
                radioButton5.Enabled = false;
                radioButton6.Enabled = false;
            }
            UpdateDirection();
            LoadHalts();
        }

        private void LoadHalts()
        {
            if (activeRoute.GetHaltCount(direction) == 0)
                return;
            m_LockTimeTable = true;
            dataGridView2.Rows.Clear();
            for (int i = 0, c = activeRoute.GetHaltCount(direction); i < c; i++)
            {
                Halt h = activeRoute.GetHalt(direction, i);
                dataGridView2.Rows.Add(h.Id, h.Name);
                dataGridView2.Rows[i].Resizable = DataGridViewTriState.False;
            }
            m_LockTimeTable = false;
            LoadTimeTable();
        }

        private void DirectionChanged(object sender, EventArgs e)
        {
            UpdateDirection();
            LoadHalts();
        }

        private void LoadTimeTable()
        {
            if (h == null || activeRoute == null || m_LockTimeTable)
                return;
            string req = String.Format("{0}/{1}/{2}/{3}/{4}/{5}", pathToJson, "time-table", activeRoute.Id, h.Id, GetToday(), direction == RouteDirection.Forward ? "0" : "1");
            WebRequest.DefaultWebProxy = new WebProxy();
            WebRequest reqGET = WebRequest.Create(req);
            WebResponse resp = reqGET.GetResponse();
            Stream stream = resp.GetResponseStream();
            StreamReader sr = new StreamReader(stream);
            string json = sr.ReadToEnd();
            timeTable = new TimeTable();
            timeTable.Parse(json);
            listView1.Items.Clear();
            if (!listView1.Visible)
                listView1.Visible = true;
            listView1.BeginUpdate();
            for (int i = 0, c = timeTable.GetTimeCount(); i < c; i++)
                listView1.Items.Add(timeTable[i].ToShortTimeString());
            listView1.EndUpdate();
        }

        private bool AllowLoadTime()
        {
            return dataGridView2.Visible;
        }

        private void HaltChanged(object sender, EventArgs e)
        {
            UpdateHalt();
            LoadTimeTable();
        }

        private void Initialize()
        {
            if (CheckServer() != WebExceptionStatus.Success)
            {
                MessageBox.Show("Невозможно подключиться к удаленному серверу.");
                Application.Exit();
                return;
            }
            radioButton1.Checked = true;
            LoadRoutes();
            UpdateHalt();
            UpdateDirection();
            groupBox1.Show();
            dataGridView1.Show();
            groupBox2.Show();
            dataGridView2.Show();
            listView1.Show();
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            Initialize();
        }
    }
}
