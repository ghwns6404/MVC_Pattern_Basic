using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace MyRoundButtonExam
{
    class RoundButton : Button
    {
        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            GraphicsPath path = new GraphicsPath();
            path.AddEllipse(this.ClientRectangle);
            // 부채꼴 모양
#if PIE
            path.AddPie(this.ClientRectangle, 0, 90);            
#endif            
            // 삼각형 모양
#if Triple
            Point[] point = new Point[3];
            point[0].X = this.ClientRectangle.Right - this.ClientRectangle.Width / 2;
            point[0].Y = this.ClientRectangle.Top;
            point[1].X = this.ClientRectangle.Left;
            point[1].Y = this.ClientRectangle.Bottom;
            point[2].X = this.ClientRectangle.Right;
            point[2].Y = this.ClientRectangle.Bottom;

            path.AddPolygon(point);
#endif

            this.Region = new Region(path);
        }

        protected override void OnPaint(PaintEventArgs pea)
        {
            // 마우스 클릭 확인 루틴
            bool bPress = this.Capture & ((MouseButtons & MouseButtons.Left) != 0)
                          & this.ClientRectangle.Contains(PointToClient(MousePosition));
            int x = bPress ? 2 : 1;

            Graphics grfx = pea.Graphics;
            Rectangle rect = this.ClientRectangle;
            GraphicsPath path = new GraphicsPath();
            path.AddEllipse(rect);
            PathGradientBrush pgbr = new PathGradientBrush(path);
            pgbr.CenterPoint = new PointF(x * (rect.Left + rect.Right) / 3,
                                          x * (rect.Top + rect.Bottom) / 3);
            pgbr.CenterColor = bPress ? Color.Red : Color.White;
            pgbr.SurroundColors = new Color[] { Color.Pink };
            grfx.FillRectangle(pgbr, rect);

            Brush br = new LinearGradientBrush(rect, Color.Black, Color.Brown,
                           LinearGradientMode.ForwardDiagonal);
            Pen pen = new Pen(br, 2);
            grfx.DrawEllipse(pen, rect);

            StringFormat strfmt = new StringFormat();
            strfmt.Alignment = strfmt.LineAlignment = StringAlignment.Center;

            br = Enabled ? SystemBrushes.WindowText : SystemBrushes.GrayText;
            grfx.DrawString(this.Text, this.Font, br, rect, strfmt);

            if (this.Focused)
            {                
                pen = new Pen(Color.Black);
                pen.DashStyle = DashStyle.Dash;
                grfx.DrawEllipse(pen, rect.X+5, rect.Y+5, rect.Width-10, rect.Height-10);
            }
        }
    }
}
