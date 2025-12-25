using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0422_Form
{
    public partial class Form2_Color1 : Form
    {
        public Form2_Color1()
        {
            InitializeComponent();

            Array arr               = Enum.GetValues(typeof(KnownColor));
            Form2_Color2_Sub[] frm  = new Form2_Color2_Sub[arr.Length];

            for(int i=0; i<frm.Length; i++)
            {
                frm[i]              = new Form2_Color2_Sub(arr.GetValue(i).ToString());
                frm[i].TextLabel    = arr.GetValue(i).ToString();
                frm[i].BackColor    = Color.FromName(arr.GetValue(i).ToString());
                frm[i].SetBounds(0, 0, 200, 50);
                frm[i].MdiParent    = this;
                frm[i].Show();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(MdiLayout.TileVertical);
        }
    }
}
