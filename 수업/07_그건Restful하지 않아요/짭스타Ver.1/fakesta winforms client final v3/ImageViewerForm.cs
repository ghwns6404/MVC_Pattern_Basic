using System;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class ImageViewerForm : Form
    {
        private readonly Form1.UIPostFull _post;
        private int _currentIndex = 0;

        public ImageViewerForm(Form1.UIPostFull post)
        {
            InitializeComponent();

            this._post = post;
            ShowPhoto();
        }

        private void ShowPhoto()
        {
            //if (_post.Files.Count == 0)
            //{
            //    pictureBoxLarge.Image = null;
            //    labelIndex.Text = "0 / 0";
            //    buttonPrev.Enabled = false;
            //    buttonNext.Enabled = false;
            //    textBoxDescription.Text = "";
            //    labelLikes.Text = "좋아요: 0";
            //    listBoxComments.Items.Clear();
            //    return;
            //}

            pictureBoxLarge.Image = _post.Files[_currentIndex];
            labelIndex.Text = $"{_currentIndex + 1} / {_post.Files.Count}";
            buttonPrev.Enabled = _post.Files.Count > 1;
            buttonNext.Enabled = _post.Files.Count > 1;

            // 설명, 좋아요, 댓글 모두 게시물 단위
            textBoxDescription.Text = _post.Title;
            labelLikes.Text = "좋아요: 0";

            //listBoxComments.Items.Clear();
            //foreach (var c in _post.Comments)
            //    listBoxComments.Items.Add(c);
        }

        private void buttonPrev_Click(object sender, EventArgs e)
        {
            _currentIndex = (_currentIndex - 1 + _post.Files.Count) % _post.Files.Count;

            ShowPhoto();
        }

        private void buttonNext_Click(object sender, EventArgs e)
        {
            _currentIndex = (_currentIndex + 1) % _post.Files.Count;

            ShowPhoto();
        }

        private void buttonLike_Click(object sender, EventArgs e)
        {
            //_post.Likes++;
            //labelLikes.Text = $"좋아요: {_post.Likes}";
        }

        private void buttonSaveDesc_Click(object sender, EventArgs e)
        {
            //_uiPost.Description = textBoxDescription.Text;
        }

        private async void buttonAddComment_Click(object sender, EventArgs e)
        {
            //var commentText = textBoxComment.Text.Trim();
            //if (!string.IsNullOrEmpty(commentText))
            //{
            //    var member = await MyClient.Instance.GetMemberMeAsync();
            //    var comment = new Form1.CommentItem
            //    {
            //        MemberName = member.Name,
            //        MemberId = member.Id,
            //        Content = commentText
            //    };
            //    _uiPost.Comments.Add(comment);
            //    listBoxComments.Items.Add(comment.ToString());
            //    textBoxComment.Clear();
            //}
        }

        private void pictureBoxLarge_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}

