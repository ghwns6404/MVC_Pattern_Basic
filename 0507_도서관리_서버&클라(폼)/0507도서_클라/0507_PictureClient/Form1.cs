using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.ServiceModel.Channels;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using _0507_PictureClient.ServiceReference1;


namespace _0507_PictureClient
{
    public partial class Form1 : Form
    {
        public BookClient bc = new BookClient();
        List<_0507_PictureClient.ServiceReference1.Book> books = new List<_0507_PictureClient.ServiceReference1.Book>();
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)  //저장 오케이  
        {
            try
            {
                int isbn = int.Parse(textBox1.Text);
                string title = textBox2.Text;
                string author = textBox3.Text;
                string publisher = textBox4.Text;
                int price = int.Parse(textBox5.Text);

                bc.C_Insert(isbn, title, author, publisher, price);

                MessageBox.Show("저장되었습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void button2_Click(object sender, EventArgs e)  //검색 오케이
        {
            int isbn = int.Parse(textBox10.Text);

            string message = bc.C_Select_Isbn(isbn).ToString();

            string[] sp = message.Split('#');
           
            isbn = int.Parse(sp[0]);
            string title = sp[1];
            string author = sp[2];
            string publisher = sp[3];
            int price = int.Parse(sp[4]);

            textBox9.Text = title;
            textBox8.Text = author;
            textBox7.Text = publisher;
            textBox6.Text = price.ToString();
        }
        private void button3_Click(object sender, EventArgs e)  //수정
        {
            try
            {
                int isbn = int.Parse(textBox10.Text);
                string title = textBox9.Text;
                string author = textBox8.Text;
                string publisher = textBox7.Text;
                int price = int.Parse(textBox6.Text);

                bc.C_Update(isbn, title, author, publisher, price);

                MessageBox.Show("수정되었습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        private void button4_Click(object sender, EventArgs e)  //삭제 오케이 
        {
            try
            {
                int isbn = int.Parse(textBox11.Text);


                bc.C_Delete(isbn);

                MessageBox.Show("삭제되었습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        
        //private void button5_Click(object sender, EventArgs e)  //전체출력    310번
        //{
        //    listView1.Items.Clear();
        //    foreach (Book book in bc.C_Select_All())
        //    {
        //        ListViewItem item = new ListViewItem(book.Isbn.ToString());

        //        item.SubItems.Add(book.Title);
        //        item.SubItems.Add(book.Author);
        //        item.SubItems.Add(book.Publisher);
        //        item.SubItems.Add(book.Price.ToString());

        //        listView1.Items.Add(item);
        //    }
        //}
    }
}
