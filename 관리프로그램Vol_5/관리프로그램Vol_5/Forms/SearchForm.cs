using System;
using System.Collections.Generic;
using System.Windows.Forms;


namespace _관리프로그램Vol_5
{
    public partial class SearchForm : Form
    {
        MyDB db = new MyDB();

        public SearchForm()
        {
            InitializeComponent();
            db.Connect();
        }
        private void button2_Click(object sender, EventArgs e)//검색
        {
            string id = textBox10.Text;
            List<Data> temp = db.SelectDB();
            Data d = temp.Find(n => n.Id == id);
            if (d != null)
            {
                textBox9.Text = d.Pw;
                textBox8.Text = d.Name;
                textBox7.Text = d.Age;
                textBox6.Text = d.Phone;
            }
            else
            {
                MessageBox.Show("없는 아이디!!", "검색 실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
        }

        private void button3_Click(object sender, EventArgs e)//삭제
        {
            string id = textBox10.Text;

            db.DeleteDB(id);
            MessageBox.Show("입력한 아이디에 관한 정보들을 삭제 완료");
        }

        private void button4_Click(object sender, EventArgs e)//수정
        {
            string id = textBox10.Text;
            string pw = textBox9.Text;
            string name = textBox8.Text;
            string age = textBox7.Text;
            string phone = textBox6.Text;

            db.UpdateDB(id, pw, name, age, phone);
            MessageBox.Show("수정ㅇㅋ!@!!!@!#");
            db.InsertDB_new(id, pw, name, age, phone);
            MessageBox.Show("수정2 ok");
        }
    }
}
