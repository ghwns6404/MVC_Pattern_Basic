using System;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace FakeStar
{
    public partial class MemberForm : Form
    {
        public MemberForm()
        {
            InitializeComponent();
        }

        private async void buttonAllMembers_Click(object sender, EventArgs e)
        {
            listViewMembers.Items.Clear();
            var members = await MyClient.Instance.GetMembersAsync();
            foreach (var m in members)
            {
                var item = new ListViewItem(new string[] { m.Id, m.Name, m.CreatedAt });
                listViewMembers.Items.Add(item);
            }
        }


        






        private async void buttonSearch_Click(object sender, EventArgs e)
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

        private void buttonMyInfo_Click(object sender, EventArgs e)
        {
            using (var dlg = new MyInfoForm())
            {
                dlg.ShowDialog();
            }
        }
    }
}