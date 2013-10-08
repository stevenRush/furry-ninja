using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Data;
using System.Windows.Forms;

namespace CinemasGUI
{
    public enum DatabaseTables
    {
        dtAgeLimit,
        dtGenres,
        dtCinemas,
        dtFormats,
        dtHallTypes,
        dtHalls,
        dtTicketTypes,
        dtSeatTypes,
        dtFilms,
        dtPricePolicies
    };

    abstract class SQLWorkerBase
    {
        private string ConnectionString = @"Server=LAPTOP\SQLEXPRESS;Database=Cinemas;Trusted_Connection=True;";
        protected SqlConnection connection;
        protected DataSet dataSet;
        protected SqlDataAdapter dataAdapter;
        protected SqlCommandBuilder commandBuilder;
        protected string simpleTableQuery;
        protected string tableName;
        protected string tableTitle;

        public SQLWorkerBase()
        {
            connection = new SqlConnection( ConnectionString );
        }

        public virtual void InitializeDataGridView(DataGridView dataGrid)
        {
            dataAdapter = new SqlDataAdapter( simpleTableQuery, connection );
            commandBuilder = new SqlCommandBuilder( dataAdapter );
            dataSet = new DataSet();
            dataAdapter.Fill( dataSet );
            dataGrid.DataSource = dataSet.Tables[0];
            Prettify(dataGrid);
        }

        protected virtual void Prettify(DataGridView dataGrid)
        {
            dataGrid.Columns["id"].Visible = false;
        }

        public virtual void UpdateData(DataGridView dataGrid)
        {
            dataAdapter.Update( dataSet );
        }

        public string GetTableName()
        {
            return tableTitle;
        }
    }

    class SimpleSQLWorker : SQLWorkerBase
    {
        public SimpleSQLWorker(string query, string title) :
                base()
        {
            simpleTableQuery = query;
            tableTitle = title;
        }
    }

    class SmartSQLWorker : SQLWorkerBase
    {
        // Key - id, Value - combobox
        protected Dictionary<string, string> fieldsToUpdate;
       
        public SmartSQLWorker() :
            base()
        {
            fieldsToUpdate = new Dictionary<string, string>();
        }

        public void OnDataLoad(object sender, DataGridViewBindingCompleteEventArgs e)
        {
            DataGridView dataGrid = (DataGridView)sender;
            foreach (DataGridViewRow row in dataGrid.Rows)
            {
                foreach (KeyValuePair<string, string> pair in fieldsToUpdate)
                {
                    row.Cells[pair.Value].Value = row.Cells[pair.Key].Value;
                }
            }
            dataGrid.DataBindingComplete -= OnDataLoad;
        }

        public void BeforeUpdateData(DataGridView dataGrid)
        {
            foreach ( DataGridViewRow row in dataGrid.Rows )
            {
                foreach ( KeyValuePair<string, string> pair in fieldsToUpdate )
                {
                    row.Cells[pair.Key].Value = row.Cells[pair.Value].Value;
                }
            }
        }

        public override void UpdateData(DataGridView dataGrid)
        {
            BeforeUpdateData( dataGrid );
            dataAdapter.Update( dataSet, tableName );
        }

        protected override void Prettify(DataGridView dataGrid)
        {
            dataGrid.DataBindingComplete += OnDataLoad;
            foreach(KeyValuePair<string, string> pair in fieldsToUpdate)
            {
                dataGrid.Columns[pair.Key].Visible = false;
            }
            dataGrid.Columns["id"].Visible = false;
        }
    }

    class HallsSQLWorker : SmartSQLWorker
    {
        public HallsSQLWorker() :
            base()
        {
            fieldsToUpdate.Add( "halltype_id", "halltype" );
            fieldsToUpdate.Add("cinema_id", "cinema");
            tableName = "Halls";
            tableTitle = "Залы";
        }

        public override void InitializeDataGridView(DataGridView dataGrid)
        {
            string hallsQuery = @"SELECT    id, cinema_id, title AS 'Название', 
                                            rows AS 'Рядов', seat_count AS 'Мест в ряду', 
                                            halltype_id 
                                  FROM Halls";
            string hallTypesQuery = "SELECT * FROM HallTypes";
            string cinemasQuery = "SELECT * FROM Cinemas";
            dataAdapter = new SqlDataAdapter(hallsQuery, connection);
            SqlDataAdapter hallTypesAdapter = new SqlDataAdapter(hallTypesQuery, connection);
            SqlDataAdapter cinemasAdapter = new SqlDataAdapter(cinemasQuery, connection);
            commandBuilder = new SqlCommandBuilder(dataAdapter);
            dataSet = new DataSet();
            dataAdapter.Fill( dataSet, tableName );
            hallTypesAdapter.Fill(dataSet, "HallTypes");
            cinemasAdapter.Fill(dataSet, "Cinemas");
            dataGrid.DataSource = dataSet.Tables[tableName];

            DataGridViewComboBoxColumn hallTypeColumn = new DataGridViewComboBoxColumn();
            {
                hallTypeColumn.Name = "halltype";
                hallTypeColumn.HeaderText = "Тип зала";
                hallTypeColumn.DataSource = dataSet.Tables["HallTypes"];
                hallTypeColumn.DisplayMember = "hall_type";
                hallTypeColumn.ValueMember = "id";
                dataGrid.Columns.Add( hallTypeColumn );
            }

            DataGridViewComboBoxColumn cinemaColumn = new DataGridViewComboBoxColumn();
            {
                cinemaColumn.Name = "cinema";
                cinemaColumn.HeaderText = "Кинотеатр";
                cinemaColumn.DataSource = dataSet.Tables["Cinemas"];
                cinemaColumn.DisplayMember = "title";
                cinemaColumn.ValueMember = "id";
                dataGrid.Columns.Insert( 3, cinemaColumn );
            }            
            dataGrid.DataBindingComplete += OnDataLoad;

            Prettify(dataGrid);
        }
    }

    class FilmsSQLWorker : SmartSQLWorker
    {
        public FilmsSQLWorker() :
                base()
        {
            fieldsToUpdate.Add("agelimit_id", "age_limit");
            fieldsToUpdate.Add("genre_id", "genre");
            tableName = "Films";
            tableTitle = "Фильмы";
        }

        public override void InitializeDataGridView(DataGridView dataGrid)
        {
            string filmQuery = @"SELECT     id, title AS 'Название', length AS 'Длительность', 
                                            genre_id, agelimit_id FROM Films";
            string ageLimitQuery = @"SELECT * FROM AgeLimits";
            string genreQuery = @"SELECT * FROM Genres";

            dataAdapter = new SqlDataAdapter(filmQuery, connection);
            commandBuilder = new SqlCommandBuilder(dataAdapter);
            SqlDataAdapter ageLimitAdapter = new SqlDataAdapter(ageLimitQuery, connection);
            SqlDataAdapter genreAdapter = new SqlDataAdapter(genreQuery, connection);

            dataSet = new DataSet();
            dataAdapter.Fill(dataSet, tableName);
            ageLimitAdapter.Fill(dataSet, "AgeLimits");
            genreAdapter.Fill(dataSet, "Genres");
            dataGrid.DataSource = dataSet.Tables[tableName];

            using ( DataGridViewComboBoxColumn column = new DataGridViewComboBoxColumn() )
            {
                column.Name = "age_limit";
                column.HeaderText = "Возрастные ограничения";
                column.DataSource = dataSet.Tables["AgeLimits"];
                column.DisplayMember = "age_limit";
                column.ValueMember = "id";
                dataGrid.Columns.Add( column );
            }

            using ( DataGridViewComboBoxColumn column = new DataGridViewComboBoxColumn() )
            {
                column.Name = "genre";
                column.HeaderText = "Жанр";
                column.DataSource = dataSet.Tables["Genres"];
                column.DisplayMember = "genre";
                column.ValueMember = "id";
                dataGrid.Columns.Add( column );
            }

            Prettify(dataGrid);
        }
    }

    class PricePoliciesSQLWorker : SmartSQLWorker 
    {
        public PricePoliciesSQLWorker() :
            base()
        {
            fieldsToUpdate.Add("seattype_id", "seattype");
            fieldsToUpdate.Add("tickettype_id", "tickettype");
            tableName = "PricePolicies";
            tableTitle = "Ценовые политики";
        }

        public override void InitializeDataGridView(DataGridView dataGrid)
        {
            string pricePoliciesQuery = @"SELECT    id, seattype_id, tickettype_id, 
                                                    title AS 'Название', price AS 'Цена'
                                          FROM PricePolicies";
            string seatTypeQuery = @"SELECT * FROM SeatTypes";
            string ticketTypeQuery = @"SELECT * FROM TicketTypes";

            dataAdapter = new SqlDataAdapter(pricePoliciesQuery, connection);
            commandBuilder = new SqlCommandBuilder(dataAdapter);
            SqlDataAdapter seatTypesAdapter = new SqlDataAdapter(seatTypeQuery, connection);
            SqlDataAdapter ticketTypesAdapter = new SqlDataAdapter(ticketTypeQuery, connection);

            dataSet = new DataSet();
            dataAdapter.Fill(dataSet, tableName);
            seatTypesAdapter.Fill(dataSet, "SeatTypes");
            ticketTypesAdapter.Fill(dataSet, "TicketTypes");
            dataGrid.DataSource = dataSet.Tables[tableName];

            using ( DataGridViewComboBoxColumn column = new DataGridViewComboBoxColumn() )
            {
                column.Name = "seattype";
                column.HeaderText = "Тип места";
                column.DataSource = dataSet.Tables["SeatTypes"];
                column.DisplayMember = "seat_type";
                column.ValueMember = "id";
                dataGrid.Columns.Insert( 4, column );
            }

            using ( DataGridViewComboBoxColumn column = new DataGridViewComboBoxColumn() )
            {
                column.Name = "tickettype";
                column.HeaderText = "Тип билета";
                column.DataSource = dataSet.Tables["TicketTypes"];
                column.DisplayMember = "ticket_type";
                column.ValueMember = "id";
                dataGrid.Columns.Insert(4, column );
            }

            Prettify(dataGrid);
        }
    }

    class SQLWorkerManager
    {
        static Dictionary<DatabaseTables, string> queries = null;
        static Dictionary<DatabaseTables, string> tableTitles = null;

        static public SQLWorkerBase GetWorker( DatabaseTables table )
        {
            if (queries == null)
                InitializeDicts();
            switch ( table )
            {
                case DatabaseTables.dtHalls:
                    return new HallsSQLWorker();
                case DatabaseTables.dtFilms:
                    return new FilmsSQLWorker();
                case DatabaseTables.dtPricePolicies:
                    return new PricePoliciesSQLWorker();
                case DatabaseTables.dtAgeLimit:
                case DatabaseTables.dtCinemas:
                case DatabaseTables.dtFormats:
                case DatabaseTables.dtGenres:
                case DatabaseTables.dtHallTypes:
                case DatabaseTables.dtSeatTypes:
                case DatabaseTables.dtTicketTypes:
                    return new SimpleSQLWorker(queries[table], tableTitles[table]);
                default:
                    throw new NotImplementedException();
            }
        }

        static private void InitializeDicts()
        {
            queries = new Dictionary<DatabaseTables,string>();
            queries.Add(DatabaseTables.dtAgeLimit, 
                        "SELECT id, age_limit AS 'Возрастное ограничение' " +
                        "FROM AgeLimits");

            queries.Add(DatabaseTables.dtGenres, 
                        "SELECT id, Genre AS 'Жанр'" +
                        "FROM Genres");

            queries.Add(DatabaseTables.dtCinemas,
                        "SELECT id, address AS 'Адрес', phone AS 'Контактный телефон', title AS 'Название'" +
                        "FROM Cinemas");
            queries.Add(DatabaseTables.dtFormats,
                        "SELECT id, format AS 'Формат' " +
                        "FROM Formats");     
            queries.Add(DatabaseTables.dtHallTypes, 
                        "SELECT id, hall_type AS 'Тип зала' " + 
                        "FROM HallTypes");
            queries.Add(DatabaseTables.dtTicketTypes, 
                        "SELECT id, ticket_type AS 'Тип билета' " + 
                        "FROM TicketTypes");
            queries.Add(DatabaseTables.dtSeatTypes, 
                        "SELECT id, seat_type AS 'Тип места' " +
                        "FROM SeatTypes");

            tableTitles = new Dictionary<DatabaseTables, string>();
            tableTitles.Add( DatabaseTables.dtAgeLimit, "Возрастные ограничения" );
            tableTitles.Add( DatabaseTables.dtCinemas, "Кинотеатры" );
            tableTitles.Add( DatabaseTables.dtFormats, "Форматы" );
            tableTitles.Add( DatabaseTables.dtGenres, "Жанры" );
            tableTitles.Add( DatabaseTables.dtHallTypes, "Типы залов" );
            tableTitles.Add(DatabaseTables.dtTicketTypes, "Типы билетов");
            tableTitles.Add(DatabaseTables.dtSeatTypes, "Типы мест");
        }
    }

    class TicketsSQLWorker
    {
        private string ConnectionString = @"Server=LAPTOP\SQLEXPRESS;Database=Cinemas;Trusted_Connection=True;";
        SqlConnection connection;

        public TicketsSQLWorker()
        {
            connection = new SqlConnection(ConnectionString);
        }

        public IEnumerable<Tuple<int, int>> BusyPlaces(int filmTableId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT row, seat_num
                                    FROM Tickets a INNER JOIN Seats b ON a.seat_id = b.id
                                    WHERE film_timetable_id = @film_table_id";
            command.Parameters.Add("@film_table_id", SqlDbType.Int);
            command.Parameters["@film_table_id"].Value = filmTableId;
            command.Connection = connection;
            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            while(reader.Read())
            {
                Tuple<int, int> result = new Tuple<int,int>(reader.GetInt32(0), reader.GetInt32(1));
                yield return result;
            }
            reader.Close();
            connection.Close();
        }

        public IEnumerable<Tuple<int, int>> BookedPlaces(int filmTableId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT row, seat_num
                                    FROM (	Seats a INNER JOIN 
		                                    BookPlaces b ON a.id = b.seat_id) 
				                                    INNER JOIN
		                                    Booking c ON b.book_id = c.id
                                    WHERE c.film_timetable_id = @film_table_id";
            command.Parameters.Add( "@film_table_id", SqlDbType.Int);
            command.Parameters["@film_table_id"].Value = filmTableId;
            command.Connection = connection;
            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            while ( reader.Read() )
            {
                Tuple<int, int> result = new Tuple<int, int>( reader.GetInt32( 0 ), reader.GetInt32( 1 ) );
                yield return result;
            }
            reader.Close();
            connection.Close();
        }

        public void UpdateFilmsCombo(DateTime date, int cinemaId, ComboBox combo)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT DISTINCT b.id AS 'filmId', c.title + ' ' + d.format AS 'filmTitle'
                                    FROM (((	FilmTimetable a INNER JOIN 
		                                        FilmFormats b 
			                                        ON a.film_format_id = b.id) 
						                                        INNER JOIN 
		                                        Films c ON film_id = c.id) 
						                                        INNER JOIN
		                                        Formats d ON format_id = d.id) 
						                                        INNER JOIN
		                                        Halls e ON e.id = a.hall_id			
                                        WHERE CAST(date AS DATE) = CAST(@date AS DATE) AND cinema_id = @cinema_id";
            command.Parameters.Add("@date", SqlDbType.NVarChar);
            command.Parameters["@date"].Value = date.ToString("dd-MM-yyyy");
            command.Parameters.Add("@cinema_id", SqlDbType.Int);
            command.Parameters["@cinema_id"].Value = cinemaId;
            command.Connection = connection;
            SqlDataAdapter dataAdapter = new SqlDataAdapter(command);
            DataSet dataSet = new DataSet();
            dataAdapter.Fill(dataSet, "Films");

            combo.DataSource = dataSet.Tables["Films"];
            combo.DisplayMember = "filmTitle";
            combo.ValueMember = "filmId";
        }

        public void UpdateHallsCombo(DateTime date, int filmId, int cinemaId, ComboBox combo)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT  a.id AS 'timeTableId', 
                                            d.title + ' - ' + SUBSTRING(CONVERT(VARCHAR, date, 8), 0, 6) AS 'filmTitle'
                                    FROM ((	FilmTimetable a	
			                                    INNER JOIN
		                                    FilmFormats b ON a.film_format_id = b.id)
			                                    INNER JOIN
		                                    Films c ON b.film_id = c.id)
			                                    INNER JOIN
		                                    Halls d ON a.hall_id = d.id
                                    WHERE	CAST(date AS DATE) = CAST(@date AS DATE) AND
		                                    d.cinema_id = @cinema_id AND
		                                    b.id = @film_id
                                    ORDER BY d.title, CAST(date AS TIME)";
            command.Parameters.Add("@date", SqlDbType.NVarChar);
            command.Parameters.Add("@film_id", SqlDbType.Int);
            command.Parameters.Add("@cinema_id", SqlDbType.Int);
            command.Parameters["@date"].Value = date.ToString( "dd-MM-yyyy" );
            command.Parameters["@film_id"].Value = filmId;
            command.Parameters["@cinema_id"].Value = cinemaId;
            command.Connection = connection;
            SqlDataAdapter dataAdapter = new SqlDataAdapter(command);
            DataSet dataSet = new DataSet();
            dataAdapter.Fill(dataSet, "Halls");

            combo.DataSource = dataSet.Tables["Halls"];
            combo.DisplayMember = "filmTitle";
            combo.ValueMember = "timeTableId";
        }

        public Tuple<int, int> GetHallSize(int hallId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT a.rows, a.seat_count
                                    FROM Halls a INNER JOIN FilmTimetable b ON a.id = b.hall_id
                                    WHERE b.id = @hall_id";
            command.Parameters.Add("@hall_id", SqlDbType.Int);
            command.Parameters["@hall_id"].Value = hallId;
            command.Connection = connection;
            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            reader.Read();
            Tuple<int, int> result = new Tuple<int,int>(reader.GetInt32(0), reader.GetInt32(1));
            reader.Close();
            connection.Close();
            return result;
        }

        public int GetHallId(int filmTableId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT a.id
                                    FROM Halls a INNER JOIN FilmTimetable b ON a.id = b.hall_id
                                    WHERE b.id = @hall_id";
            command.Parameters.Add( "@hall_id", SqlDbType.Int );
            command.Parameters["@hall_id"].Value = filmTableId;
            command.Connection = connection;
            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            reader.Read();
            int result = reader.GetInt32(0);
            reader.Close();
            connection.Close();
            return result;
        }

        public Int64 GetSeatId(int row, int seat, int hallId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT id 
                                    FROM Seats 
                                    WHERE   row = @row AND 
                                            seat_num = @seat AND
                                            hall_id = @hall_id";
            command.Connection = connection;
            command.Parameters.Add("@row", SqlDbType.Int);
            command.Parameters.Add("@seat", SqlDbType.Int);
            command.Parameters.Add("@hall_id", SqlDbType.Int);
            command.Parameters["@row"].Value = row;
            command.Parameters["@seat"].Value = seat;
            command.Parameters["@hall_id"].Value = hallId;
            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            reader.Read();
            Int64 result = reader.GetInt64(0);
            reader.Close();
            connection.Close();
            return result;
        }
    }

    class TimeTableSQLWorker
    {
        private string ConnectionString = @"Server=LAPTOP\SQLEXPRESS;Database=Cinemas;Trusted_Connection=True;";
        SqlConnection connection;
        SqlDataAdapter dataAdapter;
        DataSet dataSet;

        public TimeTableSQLWorker()
        {
            connection = new SqlConnection(ConnectionString);
        }

        public void UpdateDataGridView(DataGridView dataGrid)
        {
            string TimetableQuery = @"  SELECT id, film_format_id, hall_id, date AS 'Дата/Время' 
                                        FROM FilmTimetable
                                        ORDER BY CAST(date AS TIME)";
            dataAdapter = new SqlDataAdapter(TimetableQuery, connection);
            SqlCommandBuilder cb = new SqlCommandBuilder(dataAdapter);
            dataSet = new DataSet();
            dataAdapter.Fill( dataSet, "FilmTable" );
            dataGrid.DataSource = dataSet.Tables["FilmTable"];
            dataGrid.Columns["id"].Visible = false;
            dataGrid.Columns["film_format_id"].Visible = false;
            dataGrid.Columns["hall_id"].Visible = false;
            dataGrid.Columns["Дата/Время"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
        }

        public void UpdateHallsCombo(ComboBox combo, int cinema_id)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT id, title FROM Halls WHERE cinema_id = @cinema_id";
            command.Parameters.Add("@cinema_id", SqlDbType.Int);
            command.Parameters["@cinema_id"].Value = cinema_id; 
            command.Connection = connection;
            SqlDataAdapter dataAdapter = new SqlDataAdapter(command);
            DataSet dataSet = new DataSet();
            dataAdapter.Fill(dataSet, "Halls");
            combo.DataSource = dataSet.Tables["Halls"];
            combo.DisplayMember = "title";
            combo.ValueMember = "id";
        }

        public void UpdateFilmsCombo(ComboBox combo)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT a.id AS filmId, b.title + ' ' + c.format AS filmTitle
                                    FROM (FilmFormats a INNER JOIN Films b ON a.film_id = b.id) 
                                                        INNER JOIN Formats c ON a.format_id = c.id";
            command.Connection = connection;
            SqlDataAdapter dataAdapter = new SqlDataAdapter(command);
            DataSet dataSet = new DataSet();
            dataAdapter.Fill(dataSet, "Films");
            combo.DataSource = dataSet.Tables["Films"];
            combo.DisplayMember = "filmTitle";
            combo.ValueMember = "filmId";
        }

        public void FilterDataGrid(DataGridView dataGrid, int film_id, int hall_id)
        {
            if (dataGrid.DataSource == null)
                return;
            ( dataGrid.DataSource as DataTable ).DefaultView.RowFilter = string.Format( "hall_id = '{0}' AND film_format_id = {1}", hall_id, film_id);
        }

        public void UpdateData()
        {
            dataAdapter.Update( dataSet, "FilmTable" );
        }
    }

    class BuyTicketSQLWorker
    {
        private string ConnectionString = @"Server=LAPTOP\SQLEXPRESS;Database=Cinemas;Trusted_Connection=True;";
        SqlConnection connection;

        public BuyTicketSQLWorker()
        {
            connection = new SqlConnection(ConnectionString);
        }

        public void UpdateTicketTypesCombo(int filmId, ComboBox combo)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT  price_policy_id AS 'ppId', title
                                    FROM    Prices a INNER JOIN 
                                            PricePolicies b ON a.price_policy_id = b.id 
                                    WHERE film_timetable_id = @filmId";
            command.Parameters.Add("@filmId", SqlDbType.Int);
            command.Parameters["@filmId"].Value = filmId;
            command.Connection = connection;
            SqlDataAdapter dataAdapter = new SqlDataAdapter(command);
            DataSet dataSet = new DataSet();
            dataAdapter.Fill( dataSet, "ticketTypes" );
            combo.DisplayMember = "title";
            combo.ValueMember = "ppId";
            combo.DataSource = dataSet.Tables["ticketTypes"];
        }

        public string GetFilmTitle(int filmId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT title + ' ' + d.format
                                    FROM ((	FilmTimetable a INNER JOIN
		                                    FilmFormats b ON a.film_format_id = b.id)
						                                    INNER JOIN
		                                    Films c ON b.film_id = c.id)
						                                    INNER JOIN
		                                    Formats d ON b.format_id = d.id
                                    WHERE a.id = @film_id";
            command.Connection = connection;
            command.Parameters.Add("@film_id", SqlDbType.Int);
            command.Parameters["@film_id"].Value = filmId;

            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            reader.Read();
            string result = reader.GetString(0);
            reader.Close();
            connection.Close();
            return result;
        }

        public string GetSeansTitle(int filmId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT SUBSTRING(CONVERT(VARCHAR, date, 113), 0, 18)
                                    FROM FilmTimetable
                                    WHERE id = @film_id";
            command.Connection = connection;
            command.Parameters.Add("@film_id", SqlDbType.Int);
            command.Parameters["@film_id"].Value = filmId;

            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            reader.Read();
            string result = reader.GetString(0);
            reader.Close();
            connection.Close();
            return result;   
        }

        public string GetPrice(int pricePolicyId)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"SELECT price FROM PricePolicies WHERE id = @price_policy_id";
            command.Parameters.Add("@price_policy_id", SqlDbType.Int);
            command.Parameters["@price_policy_id"].Value = pricePolicyId;
            command.Connection = connection;
            connection.Open();
            SqlDataReader reader = command.ExecuteReader();
            reader.Read();
            string result = reader.GetInt32(0).ToString();
            reader.Close();
            connection.Close();
            return result;
        }

        public void BookSeat(Int64 seatId, int filmId, string booker, string bookerPhone)
        {
            SqlCommand command = new SqlCommand("BookPlace", connection);
            command.CommandType = CommandType.StoredProcedure;
            command.Parameters.Add("@seat_id", SqlDbType.BigInt);
            command.Parameters.Add("@film_id", SqlDbType.Int);
            command.Parameters.Add("@booker", SqlDbType.NVarChar);
            command.Parameters.Add("@booker_phone", SqlDbType.NVarChar);
            command.Parameters["@seat_id"].Value = seatId;
            command.Parameters["@film_id"].Value = filmId;
            command.Parameters["@booker"].Value = booker;
            command.Parameters["@booker_phone"].Value = bookerPhone;
            connection.Open();
            command.ExecuteNonQuery();
            connection.Close();
        }

        public void BuyTicket(Int64 seatId, int filmId, int pricePolicyType)
        {
            SqlCommand command = new SqlCommand();
            command.CommandText = @"INSERT INTO Tickets (tickettype_id, film_timetable_id, seat_id) 
                                                VALUES  (   (SELECT tickettype_id FROM PricePolicies WHERE id = @price_policy_type), 
                                                            @film_id, @seat_id)";
            command.Parameters.Add( "@price_policy_type", SqlDbType.Int );
            command.Parameters.Add("@film_id", SqlDbType.Int);
            command.Parameters.Add("@seat_id", SqlDbType.BigInt);
            command.Parameters["@price_policy_type"].Value = pricePolicyType;
            command.Parameters["@film_id"].Value = filmId;
            command.Parameters["@seat_id"].Value = seatId;
            command.Connection = connection;
            connection.Open();
            command.ExecuteNonQuery();
            connection.Close();
        }
    }
}
