using System;
using System.Drawing;
using System.Windows.Forms;

public class SplitterExam : Form
{
    bool state = false;   // Splitter 이벤트 상태
    Splitter split;
    Panel panel;          // 좌측에 위치한 패널
    public SplitterExam()
    {
        split = new Splitter();
        split.Parent = this;
        split.Dock = DockStyle.Left;
        split.LocationChanged += new EventHandler(split_Resize);

        panel = new Panel();
        panel.Parent = this;
        panel.Dock = DockStyle.Left;
        panel.BackColor = Color.Red;

        this.BackColor = Color.Blue;
    }

    private void split_Resize(object sender, EventArgs e)
    {
        state = (state) ? false : true;
        Invalidate();
    }

    protected override void OnPaint(PaintEventArgs e)
    {
        Graphics g = e.Graphics;
        if (state)
        {
            g.FillRectangle(Brushes.Red, this.ClientRectangle);
            panel.BackColor = Color.Blue;
        }
        else
        {
            g.FillRectangle(Brushes.Blue, this.ClientRectangle);
            panel.BackColor = Color.Red;
        }
    }

    public static void Main()
    {
        Application.Run(new SplitterExam());
    }
}
