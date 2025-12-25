using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class Form1 : Form
    {
        #region 멤버필드 | 생성자
        private readonly MyClient client = MyClient.Instance;

        private List<UIPostItem> posts = new List<UIPostItem>();

        public Form1()
        {
            InitializeComponent();
            buttonUpload.Visible = false;
            buttonDelete.Visible = false;
            buttonLogout.Visible = false;
            dataGridView1.AutoGenerateColumns = false;
            InitDataGridView();
            RefreshPostList();
        }
        #endregion

        #region 객체 정의
        public class PhotoItem
        {
            public Image Image { get; set; }
        }

        public class CommentItem
        {
            public string MemberName { get; set; }
            public string MemberId { get; set; }
            public string Content { get; set; }
            public override string ToString()
            {
                return $"{MemberName}({MemberId}): {Content}";
            }
        }

        public class UIPostItem
        {
            public string Title { get; set; } = "";

            public string Content { get; set; } = "";

            public PhotoItem File { get; set; }

            public List<int> FileIds { get; set; }
        }

        public class UIPostFull
        {
            public string Title { get; set; }

            public string Content { get; set; }

            public List<Image> Files { get; set; }
        }
        #endregion

        private void InitDataGridView()
        {
            dataGridView1.Columns.Clear();

            var imgCol = new DataGridViewImageColumn
            {
                Name = "Thumbnail",
                HeaderText = "썸네일",
                Width = 100,
                ImageLayout = DataGridViewImageCellLayout.Zoom
            };
            dataGridView1.Columns.Add(imgCol);

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "PostIndex",
                HeaderText = "게시물 번호",
                Width = 80
            });

            dataGridView1.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dataGridView1.ReadOnly = true;
            dataGridView1.MultiSelect = false;
            dataGridView1.RowTemplate.Height = 80;
        }

        private void RefreshPostList()
        {
            dataGridView1.Rows.Clear();
            for (int i = 0; i < posts.Count; i++)
            {
                var post = posts[i];
                dataGridView1.Rows.Add(post.File.Image, i + 1);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            using (var uploadForm = new Upload())
            {
                uploadForm.Owner = this; // 업로드 성공 시 메인폼 갱신을 위함
                uploadForm.ShowDialog();
            }
        }

        private async void button4_Click(object sender, EventArgs e) //삭제버튼
        {
            if (dataGridView1.SelectedRows.Count == 0)
            {
                MessageBox.Show("삭제할 게시물을 선택하세요.");
                return;
            }
            int idx = dataGridView1.SelectedRows[0].Index;
            if (idx >= 0 && idx < posts.Count)
            {
                var confirm = MessageBox.Show("정말 삭제하시겠습니까?", "확인", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                var session = await MyClient.Instance.GetSessionWhoamiAsync();
                if (session != null)
                {
                    await MyClient.Instance.DeletePostAsync(session.Id.ToString());
                }

                if (confirm == DialogResult.Yes)
                {
                    posts.RemoveAt(idx);
                    RefreshPostList();
                }
            }
        }

        private async void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex < 0 || e.RowIndex >= posts.Count) return;

            var post = posts[e.RowIndex];
            var fullPost = new UIPostFull()
            {
                Title = post.Title,
                Content = post.Content,
                Files = new List<Image>()
            };
            foreach (int fileId in post.FileIds)
            {
                Image img;
                try
                {
                    img = await client.GetPostFileAsync(fileId);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return;
                }

                if (img == null)
                    return;

                fullPost.Files.Add(img);
            }

            using (var viewer = new ImageViewerForm(fullPost))
            {
                viewer.ShowDialog();
            }
        }

        private async void button1_Click(object sender, EventArgs e)
        {
            using (var loginForm = new Login())
            {
                if (loginForm.ShowDialog() == DialogResult.OK)
                {
                    buttonLogin.Visible = false;
                    buttonSignUp.Visible = false;
                    buttonUpload.Visible = true;
                    buttonDelete.Visible = true;
                    buttonLogout.Visible = true;
                }
            }
        }


        private void button2_Click(object sender, EventArgs e)
        {
            using (var signUpForm = new SignUp())
            {
                signUpForm.ShowDialog();
            }
        }

        private async void button5_Click(object sender, EventArgs e)
        {
            try
            {
                var session = await MyClient.Instance.GetSessionWhoamiAsync();
                if (session != null)
                {
                    await MyClient.Instance.DeleteSessionAsync(session.Id.ToString());
                }
            }
            catch
            {
            }

            buttonLogin.Visible = true;
            buttonSignUp.Visible = true;
            buttonUpload.Visible = false;
            buttonDelete.Visible = false;
            buttonLogout.Visible = false;
            MessageBox.Show("로그아웃 되었습니다.");
        }

        private void buttonMemberManage_Click(object sender, EventArgs e)
        {
            using (var dlg = new MemberForm())
            {
                dlg.ShowDialog();
            }
        }

        private void buttonPostManage_Click(object sender, EventArgs e)
        {
            using (var dlg = new PostForm())
            {
                dlg.ShowDialog();
            }
        }

        public void AddImageRow(Image img, string title)
        {
            dataGridView1.Rows.Add(img, title);
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }


        public async void Form1_Load(object sender, EventArgs e)    //메인폼 시작시 게시물 출력
        {
            await RefreshPosts();
        }

        public async Task RefreshPosts()
        {
            var postList = await client.GetPostsAsync();

            posts.Clear();
            int count = 0;
            foreach (var post in postList)
            {
                if (count > 5)
                    break;

                Image img;
                try
                {
                    img = await client.GetPostFileAsync(post.FileIds[0]);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return;
                }

                if (img == null)
                    return;

                posts.Add(new UIPostItem
                {
                    Title = post.Title,
                    Content = post.Content,
                    File = new PhotoItem() { Image = img },
                    FileIds = post.FileIds
                });

                count++;
            }

            RefreshPostList();
        }
    }
}
