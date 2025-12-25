using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace 관리프로그램Vol._2
{
    public partial class Form1 : Form
    {
        private const string server_name = "DESKTOP-0I86BTV\\SQLEXPRESS";
        private const string db_name = "wb41";
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

        //데이터 여러개가 들어갈거니까 리스트로 넣자
        public List<Data> datas = new List<Data>();
        
        public void Temp()
        {
            datas.Add(new Data("abc", "0000", "홍길동", "20", "010-1234-5678"));
            datas.Add(new Data("def", "1234", "응우엔길동", "25", "010-2345-6789"));
            datas.Add(new Data("ggg", "1212", "제갈길동", "30", "010-3456-7890"));
        }
        public Form1()
        {
            InitializeComponent();  //폼에 쓰이는 라벨, 버튼..
            Init_ListView();
            Temp();
            Print_ListView(datas);

        }
        private void Init_ListView() //리스트뷰 초기화
        {
            listView1.View = View.Details;
            listView1.LabelEdit = true;
            listView1.AllowColumnReorder = true;
            listView1.GridLines = true;

            // Column
            listView1.Columns.Add("Id", 100, HorizontalAlignment.Left);
            listView1.Columns.Add("Password", 100, HorizontalAlignment.Left);
            listView1.Columns.Add("Name", 100, HorizontalAlignment.Left);
            listView1.Columns.Add("Age", 100, HorizontalAlignment.Left);
            listView1.Columns.Add("Phone", 100, HorizontalAlignment.Left);
            listView1.Items.AddRange(new ListViewItem[] { });
        }

        public void Print_ListView(List<Data> datas)
        {
            listView1.Items.Clear();
            foreach (var data in datas)
            {
                ListViewItem item = new ListViewItem(data.Id);
                item.SubItems.Add(data.Pw);
                item.SubItems.Add(data.Name);
                item.SubItems.Add(data.Age);
                item.SubItems.Add(data.Phone);
                listView1.Items.Add(item);
            }
        } 


        #region Data의 멤버필드 & 생성자
        public class Data
        {
            public string Id { get; set; }
            public string Pw { get; set; }
            public string Name { get; set; }
            public string Age { get; set; }
            public string Phone { get; set; }
            public Data(string id, string pw, string name, string age, string phone)
            {
                Id = id;
                Pw = pw;
                Name = name;
                Age = age;
                Phone = phone;
            }
            //public Data(string id, int pw)
            //{
            //    Id = id;
            //    Pw = pw;
            //}
            public override string ToString()
            {
                return $"{Id} {Pw} {Name} {Age} {Phone}";
            }
        }
        #endregion

        #region 각 버튼의 기능들 no DB
        private void button1_Click(object sender, EventArgs e)//회원가입 버튼
        {
            try
            {
                string id = textBox1.Text;
                string pw = textBox2.Text;
                string name = textBox3.Text;
                string age = textBox4.Text;
                string phone = textBox5.Text;
                datas.Add(new Data(id, pw, name, age, phone));
                MessageBox.Show("데이터 들어감");
            }
            catch
            {
                MessageBox.Show("데이터 안들어갔음");
            }
            
        }

        private void button5_Click(object sender, EventArgs e)//로그인 버튼
        {
            string id = textBox12.Text;
            string pw = textBox11.Text;
            bool LoginCheck = false;

            foreach (Data data in datas)
            {
                if (data.Id == id && data.Pw == pw)
                    LoginCheck = true;
                else
                    LoginCheck = false;
            }
            if (LoginCheck == true)
                MessageBox.Show("로그인 성공");
            else
                MessageBox.Show("로그인 실패", "로그인 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void button2_Click(object sender, EventArgs e)//검색 버튼
        {
            string id = textBox10.Text;
            Data d = datas.Find(n => n.Id == id);
            if (d == null)
            {
                MessageBox.Show("없는 아이디!!", "검색 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else
            {
                textBox9.Text = d.Pw;
                textBox8.Text = d.Name;
                textBox7.Text = d.Age;
                textBox6.Text = d.Phone;
            }

        }

        private void button3_Click(object sender, EventArgs e)// 삭제
        {
            string id = textBox10.Text;
            Data d = datas.Find(n => n.Id == id);
            if (d == null)
            {
                MessageBox.Show("없는 아이디!!", "삭제 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else
            {
                datas.RemoveAll(n => n.Id == id);
                MessageBox.Show("삭제완료");
                listView1.Items.Clear();
                Print_ListView(datas);
            }
        }

        private void button4_Click(object sender, EventArgs e)//수정
        {
            string id = textBox10.Text;
            Data d = datas.Find(n => n.Id == id);
            if (d == null)
            {
                MessageBox.Show("없는 아이디!!", "수정 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else
            {
                string pw = textBox9.Text;
                string name = textBox8.Text;
                string age = textBox7.Text;
                string phone = textBox6.Text;
                datas.RemoveAll(n => n.Id == id);
                datas.Add(new Data(id, pw, name, age, phone));

                listView1.Items.Clear();
                Print_ListView(datas);
                MessageBox.Show("수정완료");
            }

        }
        
        private void button6_Click(object sender, EventArgs e)//전체출력
        {
            listView1.Items.Clear();
            foreach (var data in datas)
            {
                ListViewItem item = new ListViewItem(data.Id);
                item.SubItems.Add(data.Pw);
                item.SubItems.Add(data.Name);
                item.SubItems.Add(data.Age);
                item.SubItems.Add(data.Phone);
                listView1.Items.Add(item);
            }
        }
        #endregion

       

        private void button7_Click(object sender, EventArgs e)
        {
            
        }
    }
}
