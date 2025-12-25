using System;
using System.Drawing;
using System.Windows.Forms;

public class OldStatusBar : Form
{
    StatusBarPanel sbp1 = null;
    public OldStatusBar()
    {
        this.Text = "StatusBar 예제. 닷넷 1.x 형태";

        StatusBar sb = new StatusBar();
        sb.Parent = this;
        sb.ShowPanels = true;

        sbp1 = new StatusBarPanel();
        sbp1.Text = DateTime.Now.ToLongTimeString();
        sb.Panels.Add(sbp1);

        StatusBarPanel sbp2 = new StatusBarPanel();
        sbp2.Text = "상태바 예제~";
        sbp2.Width = 200;
        sbp2.ToolTipText = "상태바 툴팁";
        sbp2.BorderStyle = StatusBarPanelBorderStyle.Raised;
        sb.Panels.Add(sbp2);

        Timer time = new Timer();
        time.Tick += new EventHandler(Time_Tick);
        time.Interval = 1000;
        time.Start();
    }

    void Time_Tick(object sender, EventArgs ea)
    {
        sbp1.Text = DateTime.Now.ToLongTimeString();
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new OldStatusBar());
    }
}