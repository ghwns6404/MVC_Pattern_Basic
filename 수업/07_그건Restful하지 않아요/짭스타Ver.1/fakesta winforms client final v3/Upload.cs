using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace FakeStar
{
    public partial class Upload : Form
    {
        private readonly MyClient client = MyClient.Instance;
        private List<string> selectedFilePaths = new List<string>();

        public Upload()
        {
            InitializeComponent();
            openFileDialog1.Multiselect = true;
        }

        private void buttonFile_Click(object sender, EventArgs e)
        {
            openFileDialog1.Title = "업로드할 파일 선택";
            openFileDialog1.Filter = "이미지 파일 (*.jpg;*.jpeg;*.png;*.bmp;*.gif)|*.jpg;*.jpeg;*.png;*.bmp;*.gif|모든 파일 (*.*)|*.*";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                selectedFilePaths.Clear();
                selectedFilePaths.AddRange(openFileDialog1.FileNames);
                labelFile.Text = string.Join(", ", Array.ConvertAll(openFileDialog1.FileNames, Path.GetFileName));

                // 첫 번째 이미지 미리보기
                try
                {
                    using (var img = Image.FromFile(selectedFilePaths[0]))
                    {
                        pictureBoxPreview.Image = new Bitmap(img);
                    }
                }
                catch
                {
                    pictureBoxPreview.Image = null;
                }
            }
        }

        private async void buttonUpload_Click(object sender, EventArgs e)
        {
            string title = textBoxTitle.Text.Trim();
            string content = textBoxContent.Text.Trim();

            if (string.IsNullOrEmpty(title) || string.IsNullOrEmpty(content))
            {
                MessageBox.Show("제목과 내용을 입력하세요.", "입력 오류", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            bool allSuccess = true;
            foreach (var filePath in selectedFilePaths)
            {
                bool result = await client.InsertPostAsync(title, content, filePath);
                allSuccess &= result;

                // 업로드 성공 시 메인폼에 이미지와 제목 추가
                if (result && Owner is Form1 mainForm && !string.IsNullOrEmpty(filePath))
                {
                    Image img = null;
                    try { img = Image.FromFile(filePath); } catch { }
                    mainForm.AddImageRow(img, title);
                }
            }

            if (allSuccess)
            {
                MessageBox.Show("게시글이 업로드되었습니다.", "성공", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("업로드에 실패한 파일이 있습니다.", "실패", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
