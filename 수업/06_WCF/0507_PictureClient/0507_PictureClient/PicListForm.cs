using _0507_PictureClient.ServiceReference1;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0507_PictureClient
{
    public partial class PicListForm : Form
    {
        private PictureClient pic = new PictureClient();

        public string SelectedPic
        {
            get
            {
                return listBox1.SelectedItem.ToString();
            }
        }

        public PicListForm()
        {
            InitializeComponent();

            string[] strPicList = pic.GetPictureList();
            listBox1.DataSource = strPicList;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void listBox1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }
    }
}
