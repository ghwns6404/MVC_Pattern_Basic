using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace _0424_ChatClient
{
    public partial class AddMemberForm : Form
    {
        #region 컨트롤 바인딩 AddId, AddPw, AddName, AddColor, AddDate
        public string AddId
        {
            get { return textBox1.Text; }
            set { textBox1.Text = value; }
        }
        public string AddPw
        {
            get { return textBox2.Text; }
            set { textBox2.Text = value; }
        }
        public string AddName
        {
            get { return textBox3.Text; }
            set { textBox3.Text = value; }
        }
        public Color  AddColor
        {
            get { return Color.FromName(comboBox1.SelectedItem.ToString());  }
        }
        public DateTime AddDate
        {
            get { return dateTimePicker1.Value; }
        }
        #endregion

        public AddMemberForm()
        {
            InitializeComponent();

            MemberControl.Singleton._AddMemberForm = this;
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

        #region 입력 정보를 제공
        public void GetAddMemberData(out string id, out string pw, out string name, out Color color, out DateTime date)
        {
            id      = AddId;
            pw      = AddPw;
            name    = AddName;
            color   = AddColor;
            date    = AddDate;
        }
        #endregion 

        #region 회원가입 버튼
        private void button1_Click(object sender, EventArgs e)
        {
            if (AddId == string.Empty || AddPw == string.Empty || AddName == string.Empty)
            {
                MessageBox.Show("회원 정보를 입력하세요");
            }
            else
            {
                MemberControl.Singleton.AddMember(AddId, AddPw, AddName, AddColor, AddDate);
            }
        }
        public void AddMember_Result(bool isflag, string id, string name)
        {
            if (isflag == true)
                DialogResult = DialogResult.OK;
            else
                MessageBox.Show("회원가입 실패");
        }
        #endregion 
    }
}
