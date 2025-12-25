using System;
using System.Windows.Forms;
using Zoo_Client.ServiceReference1;

namespace Zoo_Client
{
    public partial class Form1 : Form
    {
        ZooClient client = new ZooClient();

        public Form1()
        {
            InitializeComponent();
            
        }

        private void button1_Click(object sender, EventArgs e) //저장버튼
        {
            try
            {
                int id = int.Parse(textBox1.Text);
                string name = textBox2.Text;
                string country = textBox3.Text;
                int age = int.Parse(textBox4.Text);

                client.Insert(id, name, country, age);

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
                string name = textBox6.Text;
                string country = textBox7.Text;
                int age = int.Parse(textBox8.Text);

                client.Update(id, name, country, age);

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

                Zoo[] zoos = client.Select(id); //서버에선 list로 넘기지만 클라에서 받을땐 배열로 받음
                                                //왜?? 서버에서 list로 넘기면 클라에서 list로 받을 수 없음
                                                //왜?? 서버에서 list는 제네릭타입이라 클라에서 제네릭타입을 못받음
                                                //왜?? 그냥 WCF에서는 리스트타입을 안전하다고 생각하지 않는대.
                                                //걍 안전성땜에 그런거임

                textBox6.Text = zoos[0].Name;
                textBox7.Text = zoos[0].Country;
                textBox8.Text = zoos[0].Age.ToString();

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
                Zoo[] zoos = client.Select_All();
                listView1.Columns.Clear();  //컬럼지우기 (얘만 쓰면 컬럼은 안보이지만 데이터는 살아있음)
                listView1.Items.Clear();    //데이터를 지워야함

                listView1.Columns.Add("ID", 120);
                listView1.Columns.Add("Title", 120);
                listView1.Columns.Add("Author", 120);
                listView1.Columns.Add("Price", 120);

                foreach (Zoo zoo in zoos)
                {
                    ListViewItem item = new ListViewItem(zoo.Id.ToString());
                    item.SubItems.Add(zoo.Name);
                    item.SubItems.Add(zoo.Country);
                    item.SubItems.Add(zoo.Age.ToString());

                    listView1.Items.Add(item);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

    }
}
