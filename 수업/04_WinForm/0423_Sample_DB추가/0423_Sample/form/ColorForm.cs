using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0423_Sample
{
    public partial class ColorForm : Form
    {
        public string ColorName
        {
            get { return textBox1.Text;  }
            set { textBox1.Text = value; }
        }
        public int ColorIdx
        {
            get { return  (int)comboBox1.SelectedIndex; }
            set { comboBox1.SelectedIndex = value;      }
        }

        public ColorForm()
        {
            InitializeComponent();
            InitComboBox();
        }

        #region 콤보박스 초기화
        private void InitComboBox()
        {
            Array arr = Enum.GetValues(typeof(KnownColor));
            foreach (KnownColor c in arr)
            {
                comboBox1.Items.Add(c);
            }
            comboBox1.SelectedIndex = 0;
        }
        #endregion

        #region 적용버튼
        private void button1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
        #endregion

        #region 콤보박스 인덱스 변경
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox1.Text =  comboBox1.SelectedItem.ToString();
        }
        #endregion 
    }
}
