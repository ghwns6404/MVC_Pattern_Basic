using FakeStar;
using System;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace FakeStar
{
    public partial class MemberForm : Form
    {
        #region 객체 , 생성자
        public MemberForm()
        {
            InitializeComponent();
        }

        private Form1 _form1;
        public MemberForm(Form1 form1)
        {
            InitializeComponent();
            _form1 = form1;
        }
        #endregion

        private async void buttonAllMembers_Click(object sender, EventArgs e)   //회원 [전체]출력
        {
            listViewMembers.Items.Clear();
            var members = await MyClient.Instance.GetMembersAsync();
            foreach (var m in members)
            {
                var item = new ListViewItem(new string[] { m.Id, m.Name, m.CreatedAt });
                listViewMembers.Items.Add(item);
            }
        }
        private async void buttonSearch_Click(object sender, EventArgs e)   //회원 검색
        {
            listViewMembers.Items.Clear();
            string id = textBoxId.Text.Trim();
            if (string.IsNullOrEmpty(id)) return;
            try
            {
                var m = await MyClient.Instance.GetMemberAsync(id);
                if (m != null)
                {
                    var item = new ListViewItem(new string[] { m.Id, m.Name, m.CreatedAt });
                    listViewMembers.Items.Add(item);
                }
                else
                {
                    MessageBox.Show("해당 ID의 회원이 없습니다.");
                }
            }
            catch
            {
                MessageBox.Show("회원 조회 실패");
            }
        }
        private void buttonMyInfo_Click(object sender, EventArgs e)     //내 정보 확인
        {
            using (var dlg = new MyInfoForm())
            {
                dlg.OnMemberDeleted += () =>
                {
                    _form1.buttonLogin.Visible = true;
                    _form1.buttonSignUp.Visible = true;
                    _form1.buttonUpload.Visible = false;
                    _form1.buttonLogout.Visible = false;
                };
                dlg.ShowDialog(this);
            }
        }
    }
}

