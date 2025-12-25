using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0423_Sample
{
    public partial class MainForm : Form
    {
        private ShapeControl _shapecontrol = ShapeControl.Singleton;
        private PrintForm _printform = null;

        public MainForm()
        {
            InitializeComponent();
            InitComboBox();
        }

        #region 콤보박스 초기화
        private void InitComboBox()
        {
            Array arr = Enum.GetValues(typeof(KnownColor));
            foreach (KnownColor c in arr)
            {
                comboBox1.Items.Add(c);
            }
            comboBox1.SelectedIndex = 0;
        }
        #endregion

        #region Load
        private void MainForm_Load(object sender, EventArgs e)
        {
            List<int> list = _shapecontrol.GetShapeIdxList();
            foreach (int item in list)
            {
                comboBox2.Items.Add(item);
            }
            comboBox2.SelectedIndex = 0;
        }
        #endregion
        
        #region 도형의 정보를 얻기 위한 콤보박스 
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            int idx = (int)comboBox2.SelectedItem;

            Shape shape = _shapecontrol.GetShape(idx);

            textBox1.Text = shape.ShapePoint.X.ToString();
            textBox2.Text = shape.ShapePoint.Y.ToString();
            comboBox1.SelectedIndex = shape.ShapeIndex;
            textBox3.Text = shape.ShapeWidth.ToString();
        }
        #endregion

        #region 도형 생성버튼
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                //데이터 획득
                int x = int.Parse(textBox1.Text);
                int y = int.Parse(textBox2.Text);
                Color color = Color.FromName(comboBox1.SelectedItem.ToString());
                int idx = comboBox1.SelectedIndex;
                int width = int.Parse(textBox3.Text);

                //컨트롤에 요청
                _shapecontrol.CreateShape(new Point(x, y), color, idx, width);
                this.Invalidate();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        #endregion

        #region 도형 삭제버튼
        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                //컨트롤에 요청
                _shapecontrol.DeleteShape();
                this.Invalidate();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        #endregion

        #region 무효화 영역 처리
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g = e.Graphics;
            Shape s = _shapecontrol._Shape;

            if (s != null)
                g.FillEllipse(new SolidBrush(s.ShapeColor), s.ShapePoint.X, s.ShapePoint.Y, s.ShapeWidth, s.ShapeWidth);
        }
        #endregion

        #region 컨텍스트 메뉴 - 색상 변경
        private void MainForm_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                int x = e.X + this.Left;
                int y = e.Y + this.Top;
                this.contextMenuStrip1.Show(x, y);
            }
        }
        private void 색상변경UToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorForm form = new ColorForm();
            form.ColorName = _shapecontrol._Shape?.ShapeColor.Name ?? "-";
            form.ColorIdx = _shapecontrol._Shape?.ShapeIndex ?? -1;

            if (form.ShowDialog() == DialogResult.OK)
            {
                _shapecontrol.SetShapeColor(Color.FromName(form.ColorName), form.ColorIdx);

                comboBox1.SelectedIndex = form.ColorIdx;
                this.Invalidate();
            }
        }
        #endregion

        #region 키보드 입력 - 위치 변경
        //Form의 속성값 : KeyPreview  = true
        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                switch (e.KeyCode)
                {
                    case Keys.Up: _shapecontrol.SetShapePoint(this, 0, -5); break;
                    case Keys.Down: _shapecontrol.SetShapePoint(this, 0, 5); break;
                    case Keys.Left: _shapecontrol.SetShapePoint(this, -5, 0); break;
                    case Keys.Right: _shapecontrol.SetShapePoint(this, 5, 0); break;
                }
                textBox1.Text = _shapecontrol._Shape.ShapePoint.X.ToString();
                textBox2.Text = _shapecontrol._Shape.ShapePoint.Y.ToString();
                Invalidate();
            }
            catch(Exception)
            {
            }
        }
        #endregion

        #region 다시보기 - 다시보기 창 생성(모달리스)
        private void button3_Click(object sender, EventArgs e)
        {
            if (_printform == null)
            {
                _printform              = new PrintForm();
                _printform.Size         = this.Size;
                _printform.Main_Form    = this;
                _printform.Shapes       = _shapecontrol.GetShapeList();
                _printform.Show();
            }
            else
            {
                _printform.Focus();
            }
        }
        public void PrintFormClose()
        {
            _printform = null;
        }

        #endregion

        
    }
}
