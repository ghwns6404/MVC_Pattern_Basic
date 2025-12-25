using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0423_Sample
{
    internal class ShapeControl
    {
        private List<Shape> shapes = new List<Shape>();
        public List<Shape> Shapes { get { return shapes; } }

        private Shape _shape = null;
        public Shape _Shape { get { return _shape; } }

        #region 싱글톤 패턴
        public static ShapeControl Singleton { get; } = null;
        static ShapeControl() { Singleton = new ShapeControl(); }
        private ShapeControl()
        {
        }
        #endregion

        #region 도형 생성과 삭제
        public void CreateShape(Point pt, Color color, int idx, int width)
        {
            if (_shape != null)
                throw new Exception("이미 도형이 생성되어 있습니다.");

            _shape = new Shape(pt, color, idx, width);
            if (_shape == null)
                throw new Exception("도형 생성이 실패했습니다.");
        }
        public void DeleteShape()
        {
            if (_shape == null)
                throw new Exception("생성된 도형이 없습니다.");

            _shape = null;      //가비지로 만들어 줌.
        }
        #endregion

        #region 도형 색상 변경
        public void SetShapeColor(Color color, int idx)
        {
            _shape.ShapeColor = color;
            _shape.ShapeIndex = idx;
        }
        #endregion

        #region 도형 좌표 변경
        public void SetShapePoint(MainForm form, int x, int y)
        {            
            int xx      = _shape.ShapePoint.X;
            int yy      = _shape.ShapePoint.Y;
            Size size   = form.ClientSize;
            //Size size = new Size();

            if (x < 0 || y < 0)
                _shape.ShapePoint = new Point(Math.Max(xx + x, 0), Math.Max(yy + y, 0));
            else if (x > 0 || y > 0)
                _shape.ShapePoint = new Point(Math.Min(xx + x, size.Width - _shape.ShapeWidth),
                                              Math.Min(yy + y, size.Height - _shape.ShapeWidth));
           
            else
                throw new Exception("변경 안됨");

            shapes.Add(new Shape(_shape.ShapePoint, _shape.ShapeColor, _shape.ShapeIndex, _shape.ShapeWidth));
        }
        #endregion
    }
}
