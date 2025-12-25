using System;
using System.Drawing;
using System.Windows.Forms;

public class OldToolBar : Form
{
    public OldToolBar()
    {
        this.Text = "ToolBar 예제. 닷넷 1.x 형태";
        ImageList lstlist = new ImageList();
        lstlist.Images.Add(new Bitmap(GetType(), "OldToolBar.대한민국.bmp"));
        lstlist.Images.Add(new Bitmap(GetType(), "OldToolBar.캐나다.bmp"));
        lstlist.Images.Add(new Bitmap(GetType(), "OldToolBar.프랑스.bmp"));

        ToolBar tool = new ToolBar();
        tool.Parent = this;
        tool.ImageList = lstlist;
        tool.ShowToolTips = true;
        tool.BorderStyle = BorderStyle.Fixed3D;

        ToolBarButton[] button = new ToolBarButton[3];
        string[] str = { "대한민국", "캐나다", "프랑스" };
        for (int i = 0; i < 3; i++)
        {
            button[i] = new ToolBarButton();
            button[i].ImageIndex = i;
            button[i].ToolTipText = str[i];
            tool.Buttons.Add(button[i]);
        }
    }

    void Time_Tick(object sender, EventArgs ea)
    {
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new OldToolBar());
    }
}