using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace MyButton
{
	/// <summary>
	/// RainbowButton2에 대한 요약 설명입니다.
	/// </summary>
	public class RainbowButton2 : System.Windows.Forms.Button
	{
		
		protected override void OnPaint(PaintEventArgs pea)
		{
			base.OnPaint(pea);

			Graphics grfx = pea.Graphics;
			Point p1 = new Point(10, 10);
			Point p2 = new Point(this.Width - 20, this.Height - 10);
			
			LinearGradientBrush lgbrush = new LinearGradientBrush(p1, p2, Color.Red, Color.Blue);

			grfx.FillRectangle(lgbrush, 10, 10, this.Width - 20, this.Height - 20);
            grfx.DrawString(this.Text, this.Font, Brushes.White, this.Width/3, this.Height/2);
		}
	}
}
