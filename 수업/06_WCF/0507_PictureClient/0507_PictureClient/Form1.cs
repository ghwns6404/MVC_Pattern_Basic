using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using _0507_PictureClient.ServiceReference1;


namespace _0507_PictureClient
{
    public partial class Form1 : Form
    {
        private PictureClient pic   = new PictureClient();
        private Image picImage      = null; 

        public Form1()
        {
            InitializeComponent();
        }

        private void 그림목록보기ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PicListForm plf = new PicListForm();     
            if (plf.ShowDialog(this) == DialogResult.OK)
            {
                picImage    = Image.FromStream(new MemoryStream(pic.GetPicture(plf.SelectedPic)));
                ClientSize  = picImage.Size; 
                Text = "<파일명 : " + plf.SelectedPic + "> PictureService에서 제공받은 그림파일을 보여주는 클라이언트";
                Invalidate();   // 화면을 갱신한다.
            }
        }

        private void 그림업로드하기ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Stream readStream;

            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "그림파일 (*.bmp;*.jpg;*.gif;*.jpeg;*.png;*.tiff)|*.bmp;*.jpg;*.gif;*.jpeg;*.png;*.tiff)";
            dlg.RestoreDirectory = true;    // 현재 디렉토리를 저장해놓는다.
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                if ((readStream = dlg.OpenFile()) != null)
                {
                    byte[] bytePic;
                    BinaryReader picReader = new BinaryReader(readStream);
                    bytePic     = picReader.ReadBytes(Convert.ToInt32(readStream.Length));
                    FileInfo fi = new FileInfo(dlg.FileName);

                    if (pic.UploadPicture(fi.Name, bytePic))
                    {
                        MessageBox.Show("업로드 성공");
                    }
                    else
                    {
                        MessageBox.Show("업로드 실패");
                    }
                    readStream.Close();
                }
            }
        }

        private void 프로그램종료ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (picImage == null)
                return;

            Graphics g = e.Graphics;
            g.DrawImage(picImage, ClientRectangle);
        }
    }
}
