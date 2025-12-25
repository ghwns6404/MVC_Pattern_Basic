using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace FileExplore
{
    public partial class LoadingWnd : Form
    {
        public LoadingWnd()
        {
            InitializeComponent();
        }

        public void CenterParentFrm(MainWnd frmParent, int count)
        {
            int nX, nY;

            nX = (int)((frmParent.Width - this.Width) / 2) + frmParent.Location.X;
            nY = (int)((frmParent.Height - this.Height) / 2) + frmParent.Location.Y;

            this.Location = new Point(nX, nY);
            this.progressBar1.Maximum = count;
        }

        public void LoadingImage()
        {
            if (progressBar1.Value < progressBar1.Maximum)
                progressBar1.Value = progressBar1.Value + 1;
            else
                progressBar1.Value = 0;

            progressBar1.Update();
        }
        
    }
}
