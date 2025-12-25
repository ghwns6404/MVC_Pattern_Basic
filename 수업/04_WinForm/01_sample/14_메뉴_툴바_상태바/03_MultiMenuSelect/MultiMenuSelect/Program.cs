using System;
using System.Drawing;
using System.Windows.Forms;

public class MultiMenuSelect : Form
{
    ToolStripMenuItem[] color = new ToolStripMenuItem[7];
    string[] color_txt = { "빨강", "주황", "노랑", "초록", "파랑", "남색", "보라" };
    Color[] color_data = {Color.Red, Color.Orange, Color.Yellow, Color.Green,
                           Color.Blue, Color.Indigo, Color.Violet};
    public MultiMenuSelect()
    {
        this.Text = "메뉴 이벤트 처리하기";

        MenuStrip menu = new MenuStrip();
        menu.Parent = this;

        // Color 항목
        ToolStripMenuItem color_item = new ToolStripMenuItem();
        color_item.Text = "&Color";
        menu.Items.Add(color_item);

        for (int i = 0; i < color.Length; i++)
        {
            color[i] = new ToolStripMenuItem();
            color[i].Text = "&" + color_txt[i];
            color[i].ForeColor = color_data[i];
            color[i].Click += new EventHandler(MultiMenuSelect_Click);
            color_item.DropDownItems.Add(color[i]);
        }
    }

    void MultiMenuSelect_Click(Object sender, EventArgs ea)
    {
        string msg = "색을 선택 하셨군요!";
        if ((ToolStripMenuItem)sender == color[0])
        {
            msg = color_txt[0] + msg;
        }
        else if ((ToolStripMenuItem)sender == color[1])
        {
            msg = color_txt[1] + msg;
        }
        else if ((ToolStripMenuItem)sender == color[2])
        {
            msg = color_txt[2] + msg;
        }
        else if ((ToolStripMenuItem)sender == color[3])
        {
            msg = color_txt[3] + msg;
        }
        else if ((ToolStripMenuItem)sender == color[4])
        {
            msg = color_txt[4] + msg;
        }
        else if ((ToolStripMenuItem)sender == color[5])
        {
            msg = color_txt[5] + msg;
        }
        else if ((ToolStripMenuItem)sender == color[6])
        {
            msg = color_txt[6] + msg;
        }

        MessageBox.Show(msg);
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new MultiMenuSelect());
    }

}
