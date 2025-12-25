using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace MyButton
{
	/// <summary>
	/// RainbowButton3에 대한 요약 설명입니다.
	/// </summary>
	public class RainbowButton3 : System.Windows.Forms.Button
	{
		Color [] colors = { Color.Red, Color.Orange, Color.Yellow, Color.Green,
							  Color.Blue, Color.Magenta, Color.Violet};

		protected override void OnPaint(PaintEventArgs pea)
		{
			base.OnPaint(pea);

			Graphics grfx = pea.Graphics;
			
			LinearGradientBrush lgbrush = null;
			Font font = new Font(this.Font.FontFamily, this.Font.Size, this.Font.Style);
			SolidBrush brush = new SolidBrush(this.ForeColor);

			int cw = (this.Width - 20)/7;
			int ch = this.Height - 20;
			
			for(int i = 0 ; i < 7; i++)
			{
				Rectangle temp = new Rectangle(10+(i*cw), 10, cw, ch );

				if(i < 6)
					lgbrush = new LinearGradientBrush(temp, colors[i], colors[i+1], LinearGradientMode.Horizontal);
				else
					lgbrush = new LinearGradientBrush(temp, colors[i], colors[0], LinearGradientMode.Horizontal);

                lgbrush.WrapMode = WrapMode.Tile;
				grfx.FillRectangle(lgbrush, 10+(i*cw), 10, cw, ch );				
			}

			grfx.DrawString(this.Text, font, brush, this.Width/3, this.Height/2);
		}
	}
}
