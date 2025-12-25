//ColorForm.cs
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace _0423_FORM연습
{
    public partial class ColorForm : Form
    {
        public string ColorName
        {
            get { return textBox1.Text; }   //textBo1에서 받은 값을 리턴해주고,
            set { textBox1.Text = value; }  //그 값을 value에 넣어줬다
        }
        public int ColorIdx
        {
            get { return (int)comboBox1.SelectedIndex; }    //comboBox1에서 받은 색상을 int를 통해 숫자로 바꿔주고,
            set { comboBox1.SelectedIndex = value; }    //value에 넣어줌
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
                comboBox1.Items.Add(c);//여러가지 색감의 플레임
            }
            comboBox1.SelectedIndex = 0;    //0번째 인덱스로 기본색상
        }
        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;//대화상자에서 확인을 눌렀다고 시스템한테 알려줌
            this.Close();   ////하든 안하든 값을 주면 알아서 죽음 걍 추가적인 느낌
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBox1.Text = comboBox1.SelectedItem.ToString();  //comboBox1에 있는 색을 textBox1에 써줌
        }
    }
}
