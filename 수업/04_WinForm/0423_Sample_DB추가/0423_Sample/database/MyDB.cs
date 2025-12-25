//MyDB.cs
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Drawing;

namespace _0423_Sample
{
    internal class MyDB
    {
        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;

        #region 연결, 종료
        public bool Connect()
        {
            try
            {
                string con = string.Format($"Data Source={server_name};Initial Catalog={db_name};User ID={sql_id};Password={sql_pw}");
                scon = new SqlConnection(con);
                scmd = scon.CreateCommand();
                scon.Open();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
        public bool Close()
        {
            try
            {
                scon.Close();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }
        #endregion

        #region Insert, Update, Delete Test
        //insert into Shape values(300, 200, 'Blue', 1, 50);
        public bool Insert_Shape(Point pt, Color color, int idx, int width)
        {
            string sql = string.Format($"insert into Shape values({pt.X}, {pt.Y}, '{color.Name}', {idx}, {width});");
            return ExSqlCommand(sql);
        }
        //delete from Shape where shape_id =1;
        public bool Delete_Shape(int id)
        {
            string sql = string.Format($"delete from Shape where shape_id ={id};");
            return ExSqlCommand(sql);
        }

        //insert into ShapeList values(2,300, 200, 'Blue', 1, 50);
        public bool Insert_ShapeList(int s_idx, Point pt, Color color, int idx, int width)
        {
            string sql = string.Format($"insert into ShapeList values({s_idx},{pt.X}, {pt.Y}, '{color.Name}', {idx}, {width});");
            return ExSqlCommand(sql);
        }




        //update s_product set price = 25000 where pid = 1010;
        public bool Update_Book1(int pid, int price)
        {
            string sql = string.Format($"update s_product set price = {price} where pid = {pid};");
            return ExSqlCommand(sql);
        }
        //update s_product set price = 25000 where pname = 'C언어';
        public bool update_Book2(string name, int price)
        {
            string sql = string.Format($"update s_product set price = {price} where pname = '{name}';");
            return ExSqlCommand(sql);
        }

        //delete from s_product where pname='C#';
        public bool Delete_Book2(string name)
        {
            string sql = string.Format($"delete from s_product where pname='{name}';");
            return ExSqlCommand(sql);
        }
        #endregion

        #region 하나의 값을 반환하는 Select
        //select SUM(Price) from s_product;
        public int Get_TotalPrice()
        {
            string sql = string.Format($"select SUM(Price) from s_product;");
            object obj = ExSqlScalarCommand(sql);
            if (obj == null)
                throw new Exception("오류 발생");
            return (int)obj;
        }
        //select MAX(Price) from s_product;
        public int Get_MaxPrice()
        {
            string sql = string.Format($"select MAX(Price) from s_product;");
            object obj = ExSqlScalarCommand(sql);
            if (obj == null)
                throw new Exception("오류 발생");
            return (int)obj;
        }
        #endregion

        #region  다중로우 데이터를 반환하는 Select
        //select * from ShapeList;
        public List<Shape> SelectAll()
        {
            List<Shape> shapelist = new List<Shape>();

            string sql = string.Format($"select * from ShapeList;");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int shape_idx       = int.Parse(r["shape_idx"].ToString());
                    int point_x         = int.Parse(r["point_x"].ToString());
                    int point_y         = int.Parse(r["point_y"].ToString());
                    string color_name   = r["color_name"].ToString();
                    int color_idx       = int.Parse(r["color_idx"].ToString());
                    int width           = int.Parse(r["width"].ToString());

                    shapelist.Add(new Shape(new Point(point_x, point_y), 
                                        Color.FromName(color_name), color_idx, width));
                }
                r.Close();
            } //cmd.Dispose()
            return shapelist;
        }
        
        //select * from ShapeList where shape_idx = 2;
        public List<Shape> SelectIdxAll(int idx)
        {
            List<Shape> shapelist = new List<Shape>();

            string sql = string.Format($"select * from ShapeList where shape_idx = {idx};");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int shape_idx = int.Parse(r["shape_idx"].ToString());
                    int point_x = int.Parse(r["point_x"].ToString());
                    int point_y = int.Parse(r["point_y"].ToString());
                    string color_name = r["color_name"].ToString();
                    int color_idx = int.Parse(r["color_idx"].ToString());
                    int width = int.Parse(r["width"].ToString());

                    shapelist.Add(new Shape(new Point(point_x, point_y),
                                        Color.FromName(color_name), color_idx, width));
                }
                r.Close();
            } //cmd.Dispose()
            return shapelist;
        }
        
        //select shape_id from Shape;
        public List<int> SelectShapeIdxAll()
        {
            List<int> shapeidxs = new List<int>();

            string sql = string.Format($"select shape_id from Shape");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                while (r.Read())
                {
                    int shape_id = int.Parse(r["shape_id"].ToString());
                    shapeidxs.Add(shape_id);
                }
                r.Close();
            } //cmd.Dispose()
            return shapeidxs;
        }

        //select * from Shape where shape_id = 2;
        public Shape SelectShape(int shape_id)
        {
            string sql = string.Format($"select * from Shape where shape_id = {shape_id};");
            using (SqlCommand cmd = new SqlCommand(sql, scon))
            {
                SqlDataReader r = cmd.ExecuteReader();
                r.Read();
                
                int shape_idx       = int.Parse(r["shape_id"].ToString());
                int point_x         = int.Parse(r["point_x"].ToString());
                int point_y         = int.Parse(r["point_y"].ToString());
                string color_name   = r["color_name"].ToString();
                int color_idx       = int.Parse(r["color_idx"].ToString());
                int width           = int.Parse(r["width"].ToString());

                r.Close();
                return new Shape(new Point(point_x, point_y), Color.FromName(color_name), color_idx, width);
            } //cmd.Dispose()            
        }
        #endregion

        #region DB 명령 함수(ExecuteNonQuery:I,U,D),(ExcecuteScalar:S),(ExcecuteReader:S)
        private bool ExSqlCommand(string sql)
        {
            try
            {
                scmd.Connection = scon;
                scmd.CommandText = sql;

                if (scmd.ExecuteNonQuery() == 0)
                    return false;
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return false;
            }
        }

        private object ExSqlScalarCommand(string sql)
        {
            try
            {
                scmd.Connection = scon;
                scmd.CommandText = sql;

                return scmd.ExecuteScalar();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return null;
            }
        }

        #endregion
    }
}
