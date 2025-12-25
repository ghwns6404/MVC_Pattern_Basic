using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace 관리프로그램Vol._2
{
    public partial class Logs : Form
    {
        MyDB db = new MyDB();
        public Logs()
        {
            InitializeComponent();
            db.Connect();
        }

       

        private void button1_Click(object sender, EventArgs e)
        {
            Print_ListView(db.SelectDB_new());
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
    }
}
