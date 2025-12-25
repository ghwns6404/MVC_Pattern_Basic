using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Data.SqlTypes;
using System.Drawing;
using System.Security.Cryptography;
using System.Windows.Forms;
using 관리프로그램Vol_5;

namespace _관리프로그램Vol_5  
{
    public partial class Form1 : Form
    {
        private DataControl D_control = DataControl.Singleton;
        MyDB db = new MyDB();
        Data data = new Data();
        
        #region DB 연결, 종료, Init, Exit
        private const string server_name = "BIT\\SQLEXPRESS";
        private const string db_name = "WB41";
        private const string sql_id = "aaa";
        private const string sql_pw = "1234";

        public SqlConnection scon = null;
        public SqlCommand scmd = null;
        public void Init_DB()
        {
            db.Connect();
        }
        private void Exit_DB()
        {
            db.Close();
        }
        #endregion

        
        public Form1()
        {
            InitializeComponent();  
            Init_DB();
            Init_ListView();
            Print_ListView(db.SelectDB());
            DataControl.Singleton.Init();
        }
        private void Init_ListView()//리스트뷰 초기화  
        {
            listView1.View = View.Details;
            listView1.LabelEdit = true;
            listView1.AllowColumnReorder = true;
            listView1.GridLines = true;

            // Column
            listView1.Columns.Add("Id", 127, HorizontalAlignment.Left);
            listView1.Columns.Add("Pw", 127, HorizontalAlignment.Left);
            listView1.Columns.Add("Name", 127, HorizontalAlignment.Left);
            listView1.Columns.Add("Age", 127, HorizontalAlignment.Left);
            listView1.Columns.Add("Phone", 127, HorizontalAlignment.Left);
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

        #region 1. 각 버튼의 기능들 (모달 X)
        private void button1_Click(object sender, EventArgs e)//회원가입 버튼
        {

            if (!string.IsNullOrWhiteSpace(textBox1.Text))
            {       //빈문자열 검산데 아이디만 검사함 ㅠ
                string id = textBox1.Text;
                string pw = textBox2.Text;
                string name = textBox3.Text;
                string age = textBox4.Text;
                string phone = textBox5.Text;
                db.InsertDB(id, pw, name, age, phone);
               
                MessageBox.Show("데이터가 저장됨.");
                //Print_ListView();    //데이터 넣고 바로 출력되고 싶으면 주석 푸셈ㅇㅇ
            }
            else
            {
                MessageBox.Show("응 빈문자열", "Insert 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }        
        
        private void button6_Click(object sender, EventArgs e)//전체출력
        {
            listView1.Items.Clear();
            foreach (Data data in db.SelectDB())
            {
                ListViewItem item = new ListViewItem(data.Id);
                item.SubItems.Add(data.Pw);
                item.SubItems.Add(data.Name);
                item.SubItems.Add(data.Age);
                item.SubItems.Add(data.Phone);
                listView1.Items.Add(item);
            }
        }

        private void button7_Click_1(object sender, EventArgs e)//데이터 몽땅 삭제
        {
            try
            {
                db.ResetDB();
                MessageBox.Show("데이터 완전 삭제 완료!", "데이터 몽땅 삭제!!!", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch
            {
                MessageBox.Show("데이터삭제 실패");
            }

        }
        #endregion

        #region 2. 로그인 모달 띄우기
        private void button2_Click_1(object sender, EventArgs e)//로그인 모달창 띄우기
        {
            Form LoginForm = new LoginForm();
            LoginForm.ShowDialog();
        }
        #endregion

        #region 3. 유저정보검색  모달 띄우기
        private void button3_Click(object sender, EventArgs e)//유저정보 검색 모달창 띄우기
        {
            Form SearchForm = new SearchForm();
            SearchForm.ShowDialog();
        }
        #endregion

        #region 4. 로그 모달 띄우기
        private void button4_Click(object sender, EventArgs e)
        {
            Form Logs = new LogsForm();
            Logs.ShowDialog();
        }
        #endregion
    }
}
