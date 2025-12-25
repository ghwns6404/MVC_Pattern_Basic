//mydb.cs

using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.IO;


namespace _0507_PictureService
{
    public class MyDB
    {

        #region 싱글톤
        public static MyDB Singleton { get; } = null;
        static MyDB() { Singleton = new MyDB(); }
        private MyDB() { }
        #endregion


        //사진 가져오기
        public byte[] Get_Picture(string name) //사진 보여주기
        {
            string sql = string.Format($"select * from picture where name = '{name}';");
            SqlDataReader r = null;
            string _path;
            try
            {
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    r = cmd.ExecuteReader();
                    r.Read(); //잘 닫자

                    _path = r["path"].ToString();
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex);
                
                return new byte[0];
            }
            finally
            {
                r.Close();
            }

            byte[]bytePic = { 0 }; // 바이트 배열을 하나 만든다.
            try
            {
                // 해당 이미지 파일을 스트림 형식으로 오픈한다.
                FileStream picFileStream = new FileStream(_path , FileMode.Open,
                FileAccess.Read, FileShare.Read);

                // 이미지 파일 스트림을 읽을 객체를 하나 만든다, 리더 객체 생성(보조 객체)
                BinaryReader picReader = new BinaryReader(picFileStream);

                // 이미지 파일을 바이트 배열에 넣는다, 파일 전체를 읽어서 바이트 배열화
                bytePic = picReader.ReadBytes(Convert.ToInt32(picFileStream.Length));

                // 파일스트림을 닫는다.
                picFileStream.Close();

                // 이미지 파일이 들어있는 바이트 배열을 리턴한다.
                return bytePic; //<- 이미지가 담겨잇다
            }
            catch
            {
                // 초기값을 그냥 리턴한다. 이거 리턴 byte[] bytePic = { 0 };
                return bytePic;
            }  
        }
        public string[] Select_Picture_List() //사진 목록을 출력
        {
            List<string> list_path = new List<string>();

            string sql = string.Format($"select * from picture;");
            SqlDataReader r = null;
            try
            {
                using (SqlCommand cmd = new SqlCommand(sql, scon))
                {
                    r = cmd.ExecuteReader();
                    while (r.Read())
                    {
                       
                       string _path = r["path"].ToString();

                        list_path.Add(_path);
                    }                    
                }
            }
            catch
            {
                Console.WriteLine("Select_Picture_List :error");
                return new string[0];
            }
            finally
            {
                r.Close();
            }
            
            // 파일 경로를 뺀 파일 이름만 다시 추출한다.
            for (int i = 0; i < list_path.Count; i++)
            {
                FileInfo fi = new FileInfo(list_path[i]);
                list_path[i] = fi.Name; //추출후에 다시 대입
            }

            // 이 이미지 파일 이름들을 리턴한다.
            return list_path.ToArray(); ;
        }
        public bool Upload_Picture(string FileName, byte[] bytePic) //사진 업로드
        {
            string path = @"C:\aaa\" + FileName;
            string sql = string.Format($"insert into picture values ('{FileName}','{path}');");
            try
            {
               
                // 주어진 이미지 파일의 이름으로 파일을 하나 만든다.
                FileStream writeFileStream = new FileStream(path, FileMode.Create, FileAccess.Write);

                // 이 파일에 바이너리를 넣기 위해 BinaryWriter 객체 생성
                BinaryWriter picWriter = new BinaryWriter(writeFileStream);

                // 바이트 배열로 받은 이미지를 파일에 쓴다.
                picWriter.Write(bytePic);

                // 파일스트림을 닫는다.
                writeFileStream.Close();

                return ExSqlCommand(sql);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }

            
        }


        #region DB연결부
        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        private SqlConnection scon = null;
        private SqlCommand scmd = null;
        #endregion

        #region Connect, Close
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

        #region DB 명령 함수(ExecuteNonQuery:I,U,D),(ExcecuteScalar:S),(ExcecuteReader:S)
        public bool ExSqlCommand(string sql)
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

        public object ExSqlScalarCommand(string sql)
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
