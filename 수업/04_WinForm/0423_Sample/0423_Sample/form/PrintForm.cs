using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0423_Sample
{
    public partial class PrintForm : Form
    {
        public MainForm Main_Form { get; set; } = null;
        public List<Shape> Shapes { get; set; } = null;
        private Shape shape = null;

        public PrintForm()
        {
            InitializeComponent();
        }

        #region  시작시점 , 종료시점
        private void PrintForm_Load(object sender, EventArgs e)
        {
            trackBar1.Maximum = Shapes.Count;
        }
        private void PrintForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Main_Form.PrintFormClose();
        }
        #endregion

        #region 시작 및 도형 출력
        private void button1_Click(object sender, EventArgs e)
        {
            for(int i=0;  i< Shapes.Count; i++)
            {
                shape = Shapes[i];
                trackBar1.Value = i;
                Invalidate();           //******
                this.Update();          //******
                Thread.Sleep(50);
            }            
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (shape == null)
                return;

            Graphics g = e.Graphics; 
            g.FillEllipse(new SolidBrush(shape.ShapeColor), shape.ShapePoint.X, shape.ShapePoint.Y, shape.ShapeWidth, shape.ShapeWidth);
        }
        #endregion

        #region 트랙 이벤트
        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            shape = Shapes[trackBar1.Value];
            Invalidate();
            this.Update();
        }
        #endregion
    }
}
