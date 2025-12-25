//Form 1
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using 관리프로그램Vol_5;

namespace _관리프로그램Vol_5  
{
    public partial class Form1 : Form
    {
        private DataControl D_control = DataControl.Singleton;
        
        Data data = new Data();
        
        public Form1()
        {
            InitializeComponent();  
            
            Init_ListView();
        }

        #region 리스트 뷰 (출력)
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
        #endregion

        #region 1. 각 버튼의 기능들 (모달 X)
        private void button1_Click(object sender, EventArgs e)//회원가입 버튼
        {
            string id = textBox1.Text;
            string pw = textBox2.Text;
            string name = textBox3.Text;
            string age = textBox4.Text;
            string phone = textBox5.Text;
            //데이터들이 빈칸이 있는지 없는지 검사 후 전송
            if (id == string.Empty || pw == string.Empty ||
                name == string.Empty || age == string.Empty || phone == string.Empty)
            {
                MessageBox.Show("회원 정보를 입력하세요");
            }
            else
            {
                DataControl.Singleton. Register(id, pw, name, age, phone);
                MessageBox.Show("회원가입 성공 !");
            }
        }        
        
        private void button6_Click(object sender, EventArgs e)//전체출력
        {
            MessageBox.Show("클라이언트에서는 전체출력 불가!!", "권한없음!!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        private void button7_Click_1(object sender, EventArgs e)//데이터 몽땅 삭제
        {
            MessageBox.Show("클라이언트에서는 삭제 불가!!", "권한없음!!",MessageBoxButtons.OK, MessageBoxIcon.Warning);
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
            MessageBox.Show("클라이언트에서는 데이터 검색 불가!!", "권한없음!!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
        #endregion

        #region 4. 로그 모달 띄우기
        private void button4_Click(object sender, EventArgs e)
        {
            MessageBox.Show("클라이언트에서는 로그확인 불가!!", "권한없음!!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
        #endregion

        #region 5. 회원가입 성공실패 반환 -> 서버
        public void SetRegister(bool isflag, string id, string pw, string name, string age, string phone)
        {
            if (isflag == true)
                DialogResult = DialogResult.OK;
            else
                MessageBox.Show("회원가입 실패");
        }
        #endregion

        #region 시작 / 종료 (이거 참조되는지 잘 봐라)
        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                string mip, cip, id, pw;
                int mport, cport;
                MyInfoFile.ReadMesssage(out mip, out mport, out cip, out cport, out id, out pw);
                data.Id = id;
                data.Pw = pw;

                //network 서ㅓ버 연결
                D_control.Init(mip, mport);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                this.Close();
            }
        }
        private void Form1_Closed(object sender, FormClosedEventArgs e)
        {
            //서버종료
            MyInfoFile.WriteMesssage(data.Id, data.Pw);

            D_control.Exit();
        }
        #endregion

    }
}
