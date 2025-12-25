using _0508_Book_Client.ServiceReference1;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0508_Book_Client
{
    public partial class Form1 : Form , IBookCallback
    {
        BookClient client;

        public Form1()
        {
            InitializeComponent();
            client = new BookClient(new System.ServiceModel.InstanceContext(this));
        }

        private void button1_Click(object sender, EventArgs e) //저장버튼
        {
            try
            {
                int id = int.Parse(textBox1.Text);
                string title = textBox2.Text;
                string author = textBox3.Text;
                int price = int.Parse(textBox4.Text);

                client.Insert(id, title, author, price);

                MessageBox.Show("저장되었습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button4_Click(object sender, EventArgs e) //삭제버튼
        {
            try
            {
                int id = int.Parse(textBox9.Text);
                

                client.Delete(id);

                MessageBox.Show("삭제 오케이");
                listView1.Columns.Clear();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button3_Click(object sender, EventArgs e) //수정버튼
        {
            try
            {
                int id = int.Parse(textBox5.Text);
                string title = textBox6.Text;
                string author = textBox7.Text;
                int price = int.Parse(textBox8.Text);

                client.Update(id, title, author, price);

                MessageBox.Show("수정되었습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button2_Click(object sender, EventArgs e) //검색버튼
        {
            try
            {
                int id = int.Parse(textBox5.Text);

                Book[] books = client.Select(id);

                textBox6.Text = books[0].Title;
                textBox7.Text = books[0].Author;
                textBox8.Text = books[0].Price.ToString();

                MessageBox.Show("검색되었습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        private void button5_Click(object sender, EventArgs e) //전체출력
        {
            try
            {
                Book[] books = client.Select_All();
                listView1.Columns.Clear();  //컬럼지우기 (얘만 쓰면 컬럼은 안보이지만 데이터는 살아있음)
                listView1.Items.Clear();    //데이터를 지워야함

                listView1.Columns.Add("ID", 120);
                listView1.Columns.Add("Title", 120);
                listView1.Columns.Add("Author", 120);
                listView1.Columns.Add("Price", 120);

                foreach (Book book in books)
                {
                    ListViewItem item = new ListViewItem(book.Id.ToString());
                    item.SubItems.Add(book.Title);
                    item.SubItems.Add(book.Author);
                    item.SubItems.Add(book.Price.ToString());

                    listView1.Items.Add(item);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        public void Notify_Print_All(string[] return_All)
        {
            throw new NotImplementedException();
        }

        
    }
}
