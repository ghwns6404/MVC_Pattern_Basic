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


        public async void Form1_Load(object sender, EventArgs e)    //메인폼 시작시 게시물 출력
        {
            await RefreshPosts();
        }

        #region Grid View 초기화
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
        #endregion

        #region 사진추가 (AddImageRow)
        public void AddImageRow(Image img, string title)
        {
            dataGridView1.Rows.Add(img, title);
        }
        #endregion

        #region 화면 새로고침
        public void RefreshPostList()  //개시물 목록 갱신 (
        {
            dataGridView1.Rows.Clear();
            for (int i = 0; i < posts.Count; i++)
            {
                var post = posts[i];
                dataGridView1.Rows.Add(post.File.Image, i + 1);
            }
        }
        public async Task RefreshPosts()   //게시물 목록 갱신 (Limit : 5)
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
        #endregion

        #region 버튼 (로그인, 회원가입, 업로드, 로그아웃)
        private async void button1_Click(object sender, EventArgs e)    //로그인
        {
            using (var loginForm = new Login())
            {
                if (loginForm.ShowDialog() == DialogResult.OK)
                {
                    buttonLogin.Visible = false;
                    buttonSignUp.Visible = false;
                    buttonUpload.Visible = true;
                    buttonLogout.Visible = true;
                }
            }
        }
        private void button2_Click(object sender, EventArgs e)  //회원가입
        {
            using (var signUpForm = new SignUp())
            {
                signUpForm.ShowDialog();
            }
        }
        private void button3_Click(object sender, EventArgs e) //업로드
        {
            using (var uploadForm = new Upload())
            {
                uploadForm.Owner = this; // 업로드 성공 시 메인폼 갱신을 위함
                uploadForm.ShowDialog();
            }
        }
        private async void button5_Click(object sender, EventArgs e)    //로그아웃
        {
            try
            {
                var session = await MyClient.Instance.GetSessionWhoamiAsync();
                if (session != null)
                {
                    await MyClient.Instance.DeleteSessionAsync(session.Id.ToString());
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            buttonLogin.Visible = true;
            buttonSignUp.Visible = true;
            buttonUpload.Visible = false;
            buttonLogout.Visible = false;
            MessageBox.Show("로그아웃 되었습니다.");
        }
        #endregion

        #region 관리 버튼 (회원관리, 게시글 관리) 
        private void buttonMemberManage_Click(object sender, EventArgs e)   //회원 관리 모달
        {
            using (var dlg = new MemberForm(this))
            {
                dlg.ShowDialog();
            }
        }

        private void buttonPostManage_Click(object sender, EventArgs e)     //게시글 관리 모달
        {
            using (var dlg = new PostForm(this))
            {
                dlg.ShowDialog();
            }
        }
        #endregion

        #region 사진 더블클릭 (상세보기)
        private async void dataGridView1_CellDoubleClick(object sender, DataGridViewCellEventArgs e)    //사진 더블클릭(따봉, 댓글)
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
        #endregion
    }
}
