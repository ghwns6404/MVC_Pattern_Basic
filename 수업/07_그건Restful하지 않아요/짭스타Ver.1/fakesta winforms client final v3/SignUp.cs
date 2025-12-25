using System;
using System.Drawing;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class SignUp : Form
    {
        private readonly MyClient client = MyClient.Instance;

        public SignUp()
        {
            InitializeComponent();

        }

        private async void buttonSignUp_Click(object sender, EventArgs e)
        {
            string email = textBoxEmail.Text.Trim();
            string password = textBoxPw.Text.Trim();
            string name = textBoxName.Text.Trim();

            if (string.IsNullOrEmpty(name) || string.IsNullOrEmpty(email) || string.IsNullOrEmpty(password))
            {
                MessageBox.Show("모든 정보를 입력하세요.", "입력 오류", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            bool success = await client.InsertMemberAsync(email, password, name);
            if (success)
            {
                MessageBox.Show("회원가입 성공!", "성공", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("회원가입 실패", "실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
