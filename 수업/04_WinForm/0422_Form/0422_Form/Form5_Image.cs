using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0422_Form
{
    public partial class Form5_Image : Form
    {
        private Image image = null;

        public Form5_Image()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Image imageFile = Image.FromFile("ocean.jpg");
            using (Graphics grfx = Graphics.FromImage(imageFile))      //***
            {
                grfx.DrawString("이미지에 글자쓰기", new Font("돋음", 20), Brushes.Pink, 10, 10);
            }

            imageFile.Save("ocean.gif", System.Drawing.Imaging.ImageFormat.Gif);
            imageFile.Save("ocean.bmp", System.Drawing.Imaging.ImageFormat.Bmp);
            imageFile.Save("ocean.png", System.Drawing.Imaging.ImageFormat.Png);

            
            this.image = Image.FromFile("ocean.gif");
            this.Invalidate(this.ClientRectangle);
        }

        private void Form5_Image_Paint(object sender, PaintEventArgs e)
        {
            Graphics grfx = e.Graphics;
            if (image != null)
                grfx.DrawImage(image, 200, 0);

        }
    }
}
