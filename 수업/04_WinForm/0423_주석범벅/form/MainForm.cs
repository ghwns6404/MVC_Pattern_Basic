//MainForm.cs
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0423_FORM연습
{
    public partial class MainForm : Form
    {
        private ShapeControl _shapecontrol = ShapeControl.Singleton;
        private PrintForm _printform = null;    //모달리스트 창이 한번만 뜰 수 있게 해준다//제일 상단에 놔야함.
        public MainForm()
        {
            InitializeComponent();
            InitComboBox(); //콤보박스에 색 이름을 넣기 위해 생성
        }

        #region 콤보박스 초기화
        private void InitComboBox()
        {
            Array arr = Enum.GetValues(typeof(KnownColor));
            //KnownColor를 통해 미리 정의된 색 이름들을 알고, Enum.GetValues을 통해 모든 항목을 배열 형태로 반환해준다. 그래서 arr에는 색 이름들이 있는 배열이다.
            foreach (KnownColor c in arr)
            {
                comboBox1.Items.Add(c);//여러가지 색감의 플레임
            }
            comboBox1.SelectedIndex = 0;    //0번째 인덱스로 기본색상
        }
        #endregion 

        #region 도형 생성버튼
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                //데이터를 획득한다.
                int x = int.Parse(textBox1.Text);//textBox1에서 x좌표를 입력받음
                int y = int.Parse(textBox2.Text);//textBox2에서 y좌표를 입력받음
                Color color = Color.FromName(comboBox1.SelectedItem.ToString());    //comboBox1에서 선택한 색을 Color객체로 변환
                int idx = comboBox1.SelectedIndex;  //comboBox1에서 선택한 색의 순번(인덱스) 가져오기
                int width = int.Parse(textBox3.Text);//textBox3에서 너비(width)를 입력받음

                //컨트롤에 요청하기 //UI의 역할은 여기까지
                _shapecontrol.CreateShape(new Point(x, y), color, idx, width);  //새로운 도형을 생성(얘로 인해 상수값을 변수로 바꿀 수 있다!=값 변경 X => 갑 변경 O)
                this.Invalidate();  //폼을 새로 그려서 도형을 화면에 반영한다.
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
                _shapecontrol.DeleteShape();//도형 삭제 요청
                this.Invalidate();//화면 새로고침
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
        #endregion

        #region 무효화 영역 처리
        protected override void OnPaint(PaintEventArgs e)//OnPaint는 기본적인 일들만 할 수 있기 때문에 부모인 Control에서 호출했기 때문에 초기화를 해서 우리가 하고싶은대로 만들어야한다 그래서 초기화를 한다.
        {
            base.OnPaint(e);//부모의 클래스의 기본 그리기 작업을 호출한다.

            Graphics g = e.Graphics;    //그래픽의 객체를 가져온다.
            Shape s = _shapecontrol._Shape; //그릴 도형의 객체를 가져온다.

            if(s != null)   //도형이 있을 때만 가능하게
                g.FillEllipse(new SolidBrush(s.ShapeColor), s.ShapePoint.X, s.ShapePoint.Y, s.ShapeWidth, s.ShapeWidth);//도형을 그린다.
        }

        #endregion

        #region 컨텍스트 메뉴 - 색상 변경

        #endregion
        private void MainForm_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                //x+y를 통해 나온 위치에 창이 딱
                int x = e.X + this.Left;
                int y = e.Y + this.Top;
                this.contextMenuStrip1.Show(x, y);  //마우스 우클릭시 뜨는 창
            }
        }

        private void 색상변경UToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorForm form = new ColorForm();   //얘가 모달
            form.ColorName = _shapecontrol._Shape?.ShapeColor.Name ?? "-";  //?=> 앞에가 null이면 null이 되고, ??=> 앞에가 null이면 뒤에있는 문자를 출력함
            form.ColorIdx = _shapecontrol._Shape?.ShapeIndex ?? -1; //?=> 앞에가 null이면 null이 되고, ??=> 앞에가 null이면 뒤에있는 문자를 출력함

            if (form.ShowDialog() == DialogResult.OK) //ShowDialog가 버튼을 눌렀다면
            {
                _shapecontrol.SetShapeColor(Color.FromName(form.ColorName), form.ColorIdx);
                //_shapecontrol._Shape.ShapeColor = Color.FromName(form.ColorName); //색상 변경
                //_shapecontrol._Shape.ShapeIndex = form.ColorIdx;    //우클릭 후 내가 원하는 색상을 고르면 화면 바뀌는거

                comboBox1.SelectedIndex = form.ColorIdx;    //메인 화면 바뀌는것
                this.Invalidate();  //초기화
            }
        }

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
            catch (Exception)
            {
            }
        }
        #endregion

        #region 다시보기 - 다시보기 창 생성(모달리스)
        private void button3_Click(object sender, EventArgs e)
        {
            if (_printform == null)
            {
                _printform = new PrintForm();
                _printform.Size = this.Size;
                _printform.Main_Form = this;
                _printform.Shapes = _shapecontrol.Shapes;
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

        private void MainForm_Load(object sender, EventArgs e)
        {

        }
    }
}
