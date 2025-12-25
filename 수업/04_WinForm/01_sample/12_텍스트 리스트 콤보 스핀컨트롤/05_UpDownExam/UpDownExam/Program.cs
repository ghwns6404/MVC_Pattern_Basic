using System;
using System.Drawing;
using System.Windows.Forms;

public class UpDownExam : Form
{
    NumericUpDown numeric;
    DomainUpDown domain;
    string[] str = { "빨강", "주황", "노랑", "초록", "파랑", "남색", "보라" };
    public UpDownExam()
    {
        numeric = new NumericUpDown();
        numeric.Parent = this;
        numeric.SetBounds(20, 20, 100, 30);
        numeric.Minimum = -1000;
        numeric.Maximum = 9000;
        numeric.Value = 1000;
        numeric.Increment = 1000;
        numeric.ThousandsSeparator = true;
        numeric.ValueChanged += new EventHandler(numeric_ValueChanged);

        domain = new DomainUpDown();
        domain.Parent = this;
        domain.SetBounds(20, 50, 100, 30);
        for (int i = 0; i < str.Length; i++)
        {
            domain.Items.Add(str[i]);
        }
        domain.Text = "색상 선택";
        domain.SelectedItemChanged += new EventHandler(domain_SelectedItemChanged);
    }

    private void numeric_ValueChanged(object sender, EventArgs e)
    {
        Graphics g = this.CreateGraphics();
        g.FillRectangle(Brushes.White, new Rectangle(100, 100, 100, 100));
        g.DrawString(this.numeric.Value.ToString(), this.Font, Brushes.Blue, 100, 100);
        g.Dispose();
    }

    private void domain_SelectedItemChanged(object sender, EventArgs e)
    {
        Color[] color = { Color.Red, Color.Orange, Color.Yellow, Color.Green, Color.Blue, Color.Magenta, Color.Violet };
        this.BackColor = color[this.domain.SelectedIndex];
    }

    public static void Main()
    {
        Application.Run(new UpDownExam());
    }
}
