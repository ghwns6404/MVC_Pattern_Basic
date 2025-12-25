//PrintForm.cs
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
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace _0423_FORM연습
{
    public partial class PrintForm : Form
    {
        //자기 자신의 부모인 MainForm을 저장
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
            trackBar1.Maximum = Shapes.Count;   //trackBar1한테 x,y값이 몇칸씩 찍혔는지 알려주는것 그래서 움직여 보이는거임
        }

        private void PrintForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Main_Form.PrintFormClose(); // 폼 닫기
        }

        #endregion

        #region 시작 및 도형 출력
        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < Shapes.Count; i++)
            {
                shape = Shapes[i];  //0번째 자리부터 i가 끝날 때 까지의 갯수를 알려주는것
                trackBar1.Value = i;    //트랙바도 그만큼 움직여야ㅎ함
                Invalidate();           //초기화
                this.Update();          //그걸 다시 업데이트 해준다
                Thread.Sleep(50);       //움직이는 속도 0.05초
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);    //부모의 클래스의 기본 그리기 작업을 호출한다.

            if (shape == null)  //도형이 null값이라면 리턴
                return;

            Graphics g = e.Graphics;    //그래픽의 객체를 가져온다.
            g.FillEllipse(new SolidBrush(shape.ShapeColor), shape.ShapePoint.X, shape.ShapePoint.Y, shape.ShapeWidth, shape.ShapeWidth);//도형을 그린다.
        }
        #endregion

        #region 트랙 이벤트
        private void trackBar1_Scroll_1(object sender, EventArgs e)
        {
            shape = Shapes[trackBar1.Value];    //그냥 연결해주는 개념//슬라이더 값에 따라 Shapes 배열에서 도형 하나 선택해서 shape에 저장
            Invalidate();
            this.Update();
        }
        #endregion


    }
}
