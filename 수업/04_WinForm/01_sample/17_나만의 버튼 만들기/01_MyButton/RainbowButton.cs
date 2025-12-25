using System;
using System.Drawing;
using System.Windows.Forms;

namespace MyButton
{
	/// <summary>
	/// RainbowButton에 대한 요약 설명입니다.
	/// </summary>
	public class RainbowButton : System.Windows.Forms.Button
	{
		Color [] colors = { Color.Red, Color.Orange, Color.Yellow, Color.Green,
							  Color.Blue, Color.Magenta, Color.Violet  };

		protected override void OnPaint(PaintEventArgs pea)
		{
			base.OnPaint(pea);

			Graphics grfx = pea.Graphics;
			for(int i = 0 ; i < 7; i++)
			{
				grfx.FillRectangle(new SolidBrush(colors[i]), 20, 20+10*i, 10, 10);
			}
		}
	}
}
