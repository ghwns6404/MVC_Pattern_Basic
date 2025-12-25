//LogIn Form
using System;
using System.Windows.Forms;
using 관리프로그램Vol_5;

namespace _관리프로그램Vol_5
{
    public partial class LoginForm : Form
    {
        private DataControl D_control = DataControl.Singleton;
        public LoginForm()
        {
            InitializeComponent();
            D_control._LoginForm = this; //이새끼 잘봐라 ㅋㅋ모르면 코드 다시 읽기
        }

        #region 로그인 버튼 클릭할때
        private void button5_Click(object sender, EventArgs e)
        {
            string id = textBox12.Text;
            string pw = textBox11.Text;
            if (id == string.Empty || pw == string.Empty)
            {
                MessageBox.Show("로그인정보를 똑바로 입력하셈");
            }
            else
            {
                DataControl.Singleton.Login(id, pw);
            }
        }
        #endregion

        #region 로그인 성공실패 유무 반환
        public void SetLogin(bool isflag, string id, string pw)
        {
            if (isflag == true)
                MessageBox.Show("로그인 성공");
            else
                MessageBox.Show("로그인 실패");
        }
        #endregion

    }
}
