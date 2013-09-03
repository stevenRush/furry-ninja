using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;

namespace FindMyBus
{
    using str_obj = Dictionary<string, object>;
    class Routes
    {
        private Route[] m_Routes;

        public Route this[int index]
        {
            get
            {
                return m_Routes[index];
            }
            set
            {

            }
        }

        public void Parse(string input)
        {
            if (input == "")
                return;

            string json = input;
            JavaScriptSerializer jss = new JavaScriptSerializer();
            str_obj dict = (str_obj)jss.DeserializeObject(json);
            Object[] objects = (Object[])dict["routes"];
            m_Routes = new Route[objects.Count()];
            int i = 0;
            foreach (str_obj item in objects)
            {
                m_Routes[i++] = new Route((string)item["routeNumber"], (string)item["routeName"], (string)item["routeId"]);
            }
        }

        public int RoutesCount()
        {
            if (m_Routes == null)
                return 0;
            return m_Routes.Count();
        }

        public Route GetById(string Id)
        {
            foreach (var route in m_Routes)
                if (route.Id == Id)
                    return route;
            return null;
        }

        public Route GetByNumber(string Number)
        {
            foreach (var route in m_Routes)
                if (route.Number == Number)
                    return route;
            return null;
        }
    }


    enum RouteDirection
    {   
        Forward,
        Backward
    }


    class Route
    {
        private string m_Number;
        private string m_Name;
        private string m_Id;
        private Halt[] m_ForwardHalts;
        private Halt[] m_BackwardHalts;

        public Route(string Number, string Name, string Id)
        {
            m_Number = Number;
            m_Name = Name;
            m_Id = Id;
        }

        public Halt GetHalt(RouteDirection direction, int index)
        {
            return direction == RouteDirection.Forward ? m_ForwardHalts[index] : m_BackwardHalts[index];
        }

        public Halt GetHaltById(string Id)
        {
            foreach (Halt h in m_ForwardHalts)
                if (h.Id == Id)
                    return h;
            foreach (Halt h in m_BackwardHalts)
                if (h.Id == Id)
                    return h;
            return null;
        }

        public int GetHaltCount(RouteDirection direction)
        {
            return direction == RouteDirection.Forward ? m_ForwardHalts.Count() : m_BackwardHalts.Count();
        }

        public void Parse(string input)
        {
            if (input == "")
                return;

            string json = input;
            JavaScriptSerializer jss = new JavaScriptSerializer();
            str_obj dict = (str_obj)jss.DeserializeObject(json);
            dict = (str_obj)dict["route"];
            Object[] objects = (Object[])dict["fwdHaltes"];
            m_ForwardHalts = new Halt[objects.Count()];
            int i = 0;
            foreach (str_obj item in objects)
            {
                m_ForwardHalts[i++] = new Halt(item["haltId"].ToString(), (string)item["haltName"]);
            }

            objects = (Object[])dict["bkwdHaltes"];
            m_BackwardHalts = new Halt[objects.Count()];
            i = 0;
            foreach (str_obj item in objects)
            {
                m_BackwardHalts[i++] = new Halt(item["haltId"].ToString(), (string)item["haltName"]);
            }
        }

        /// <summary>
        /// Отображаемый номер маршрута
        /// </summary>
        public string Number
        {
            get 
            { 
                return m_Number; 
            }
            set { }
        }

        /// <summary>
        /// Название маршрута
        /// </summary>
        public string Name
        {
            get 
            { 
                return m_Name; 
            }
            set { }
        }

        /// <summary>
        /// Идентификатор марштрута
        /// </summary>
        public string Id
        {
            get
            { 
                return m_Id; 
            }
            set { }
        }
    }


    class Halt
    {
        private string m_Name;
        private string m_Id;

        public string Name
        {
            get
            {
                return m_Name;
            }
            set { }
        }

        public string Id
        {
            get
            {
                return m_Id;
            }
            set { }
        }

        public Halt(string Id, string Name)
        {
            m_Name = Name;
            m_Id = Id;
        }
    }

    class TimeTable
    {
        List<DateTime> times;


        public TimeTable()
        {
            times = new List<DateTime>();
        }

        public DateTime this[int index]
        {
            get
            {
                return times[index];
            }
            set { }
        }

        public int GetTimeCount()
        {
            return times.Count();
        }

        public void Parse(string input)
        {
            if (input == "")
                return;

            string json = input;
            JavaScriptSerializer jss = new JavaScriptSerializer();
            str_obj dict = (str_obj)jss.DeserializeObject(json);
            Object[] objects = (Object[])dict["timeTable"];
            times.Clear();
            DateTime td = DateTime.Today;
            foreach (var obj in objects)
            {
                str_obj d = (str_obj)obj;
                int hour = (int)d["hour"];
                Object[] minutes = (Object[])d["minutes"];
                foreach (var min in minutes)
                    times.Add(new DateTime(td.Year, td.Month, td.Day, hour, (int)min, 0));
            }
        }
    }

    enum ErrorType
    {
        etOk,
        etInternetConnection,
        etServer
    }
}
