using System;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class MyInfoForm : Form
    {
        #region 객체, 생성자
        public event Action OnMemberDeleted;
        public MyInfoForm()
        {
            InitializeComponent();
        }
        #endregion


        private async void MyInfoForm_Load(object sender, EventArgs e)  //내 정보 조회
        {
            // 1. 내 회원 정보 조회
            var member = await MyClient.Instance.GetMemberMeAsync();
            if (member != null)
            {
                textBoxId.Text = member.Id;
                textBoxEmail.Text = member.Email;
                textBoxName.Text = member.Name;
                textBoxPw.Text = member.Pw;
            }
            else
            {
                MessageBox.Show("내 정보를 불러올 수 없습니다.");
                this.Close();
                return;
            }

            // 2. 내 세션 정보 조회
            try
            {
                var session = await MyClient.Instance.GetSessionWhoamiAsync();
                if (session != null)
                {
                    textBoxSessionId.Text = session.Id.ToString();
                    textBoxMemId.Text = session.MemId.ToString();
                    textBoxSessionCreated.Text = session.CreatedAt;
                    textBoxSessionUpdated.Text = session.UpdatedAt;
                }
                else
                {
                    textBoxSessionId.Text = "";
                    textBoxMemId.Text = "";
                    textBoxSessionCreated.Text = "";
                    textBoxSessionUpdated.Text = "";
                }
            }
            catch
            {
                textBoxSessionId.Text = "";
                textBoxMemId.Text = "";
                textBoxSessionCreated.Text = "";
                textBoxSessionUpdated.Text = "";
            }
        }

        private async void buttonChangePw_Click(object sender, EventArgs e) //비밀번호 변경 버튼
        {
            string oldPw = textBoxPw.Text.Trim();
            string newPw = textBoxNewPw.Text.Trim();
            if (string.IsNullOrEmpty(newPw))
            {
                MessageBox.Show("새 비밀번호를 입력하세요.");
                return;
            }
            bool result = await MyClient.Instance.UpdateMemberPwAsync(oldPw, newPw);
            if (result)
            {
                MessageBox.Show("비밀번호가 변경되었습니다.");
                textBoxPw.Text = newPw;
                textBoxNewPw.Clear();
            }
            else
            {
                MessageBox.Show("비밀번호 변경 실패");
            }
        }
        private async void buttonDelete_Click(object sender, EventArgs e)   //회원 탈퇴 버튼
        {
            var confirm = MessageBox.Show("정말 회원 탈퇴하시겠습니까?", "확인", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
            if (confirm != DialogResult.Yes) return;

            bool result = await MyClient.Instance.DeleteMemberMeAsync();
            if (result == true)
            {
                OnMemberDeleted.Invoke();
                MessageBox.Show("회원 탈퇴가 완료되었습니다.");
                this.Close();
            }
            else
            {
                MessageBox.Show("회원 탈퇴 실패");
            }
        }
        

    }
}
