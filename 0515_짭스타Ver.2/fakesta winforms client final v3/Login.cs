using System;
using System.Drawing;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class Login : Form
    {
        #region 객체, 생성자
        private readonly MyClient client = MyClient.Instance;
        public Login()
        {
            InitializeComponent();
        }
        #endregion

        private async void buttonLogin_Click(object sender, EventArgs e)
        {
            string email = textBoxEmail.Text.Trim();
            string password = textBoxPw.Text.Trim();

            if (string.IsNullOrEmpty(email) || string.IsNullOrEmpty(password))
            {
                MessageBox.Show("이메일과 비밀번호를 입력하세요.", "입력 오류", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            bool success = await client.LoginMemberAsync(email, password);
            if (success)
            {
                MessageBox.Show("로그인 성공!", "성공", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("로그인 실패", "실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }   //로그인 버튼

        private void buttonCancel_Click(object sender, EventArgs e) //로그인 취소 버튼
        {
            this.Close();
        }
    }
}
