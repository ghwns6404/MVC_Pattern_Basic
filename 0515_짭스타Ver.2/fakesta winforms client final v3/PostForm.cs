using System;
using System.Security.Cryptography;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class PostForm : Form
    {

        private Form1 _form1;
        public PostForm(Form1 form1)
        {
            InitializeComponent();
            _form1 = form1;

        }
       

        #region 버튼 (전체 게시물 출력, 검색, 삭제)
        private async void buttonAllPosts_Click(object sender, EventArgs e) //전체게시물 출력
        {
            listViewPosts.Items.Clear();
            var posts = await MyClient.Instance.GetPostsAsync();
            foreach (var p in posts)
            {
                var item = new ListViewItem(new string[] { p.Id.ToString(), p.Title, p.Content });
                listViewPosts.Items.Add(item);
            }
        }
        private async void buttonSearch_Click(object sender, EventArgs e)   //게시물 검색
        {
            listViewPosts.Items.Clear();
            string id = textBoxId.Text.Trim();
            if (string.IsNullOrEmpty(id)) return;
            try
            {
                var p = await MyClient.Instance.GetPostAsync(id);
                if (p != null)
                {
                    var item = new ListViewItem(new string[] { p.Id.ToString(), p.Title, p.Content });
                    listViewPosts.Items.Add(item);
                }
                else
                {
                    MessageBox.Show("해당 ID의 게시물이 없습니다.");
                }
            }
            catch
            {
                MessageBox.Show("게시물 조회 실패");
            }
        }
        private async void button1_Click(object sender, EventArgs e)    //게시물 삭제
        {
            if (listViewPosts.SelectedItems.Count > 0)
            {
                int rowIndex = listViewPosts.SelectedItems[0].Index;

                // 선택된 행에서 ID 추출
                string id = listViewPosts.SelectedItems[0].SubItems[0].Text;
                MessageBox.Show("삭제 성공!");
                // 서버에서 삭제 요청
                bool success = await MyClient.Instance.DeletePostAsync(id);

                if (success == true)
                {
                    // UI에서 행 삭제
                    listViewPosts.Items.RemoveAt(rowIndex);
                    await _form1.RefreshPosts();
                    //메인폼에 있는 refresh함수 호출
                }
                else
                {
                    MessageBox.Show("삭제 실패");
                }
            }
        }
        #endregion

    }
}
