using _05059_시험_Client.ServiceReference1;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace _05059_시험_Client
{
    public partial class Form1 : Form
    {
        AccountClient client = new AccountClient();
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)//저장
        {
            try
            {
                int id = int.Parse(textBox1.Text);
                string name = textBox2.Text;
                int balance = int.Parse(textBox3.Text);
                

                client.InsertAccount(id, name, balance);

                MessageBox.Show("저장요청 성공");

                #region 화면갱신 노가다방법
                listView1.Items.Clear();
                Account[] accounts = client.SelectAllAccount();
                foreach (Account account in accounts)
                {
                    ListViewItem item = new ListViewItem(account.Id.ToString());
                    item.SubItems.Add(account.Name);
                    item.SubItems.Add(account.Balance.ToString());

                    listView1.Items.Add(item);
                }
                #endregion
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            
        }

        private void button2_Click(object sender, EventArgs e)// 검색
        {
            try
            {
                int id = int.Parse(textBox4.Text);

                Account accounts = client.SelectAccount(id);

                textBox5.Text = accounts.Name;
                textBox6.Text = accounts.Balance.ToString();
               
                MessageBox.Show("검색요청 성공");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button3_Click(object sender, EventArgs e)//삭제
        {
            try
            {
                int id = int.Parse(textBox7.Text);


                client.DeleteAccount(id);

                MessageBox.Show("삭제요청 성공");

                #region 화면갱신 노가다방법
                listView1.Items.Clear();
                Account[] accounts = client.SelectAllAccount();
                foreach (Account account in accounts)
                {
                    ListViewItem item = new ListViewItem(account.Id.ToString());
                    item.SubItems.Add(account.Name);
                    item.SubItems.Add(account.Balance.ToString());

                    listView1.Items.Add(item);
                }
                #endregion

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        } 

        private void button4_Click(object sender, EventArgs e) //전체출력
        {
            try
            {
                Account[] accounts = client.SelectAllAccount();
                listView1.Columns.Clear();  
                listView1.Items.Clear();    

                listView1.Columns.Add("ID[아이디]", 120);
                listView1.Columns.Add("Name[이름]", 120);
                listView1.Columns.Add("Balance[잔액]", 120);
                

                foreach (Account account in accounts)
                {
                    ListViewItem item = new ListViewItem(account.Id.ToString());
                    item.SubItems.Add(account.Name);
                    item.SubItems.Add(account.Balance.ToString());

                    listView1.Items.Add(item);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button5_Click(object sender, EventArgs e) //사실 수정이었던거임
        {
            try
            {
                int id = int.Parse(textBox8.Text);
                int money = int.Parse(textBox9.Text);

                client.InputMoney(id, money);

                MessageBox.Show("입금요청 성공");

                #region 화면갱신 노가다방법
                listView1.Items.Clear();
                Account[] accounts = client.SelectAllAccount();
                foreach (Account account in accounts)
                {
                    ListViewItem item = new ListViewItem(account.Id.ToString());
                    item.SubItems.Add(account.Name);
                    item.SubItems.Add(account.Balance.ToString());

                    listView1.Items.Add(item);
                }
                #endregion

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
