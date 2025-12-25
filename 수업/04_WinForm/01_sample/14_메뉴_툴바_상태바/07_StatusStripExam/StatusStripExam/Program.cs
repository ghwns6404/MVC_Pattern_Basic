using System;
using System.Drawing;
using System.Windows.Forms;

public class StatusStripExam : Form
{
    ToolStripButton color_btn, comment_btn;
    ToolStripComboBox txt_box;
    ToolStripLabel label;
    ToolStripProgressBar prog_bar;

    public StatusStripExam()
    {
        this.Text = "StatusStrip 예제";

        ImageList imglst = new ImageList();
        imglst.TransparentColor = Color.Black;
        imglst.Images.Add("Color", new Bitmap(GetType(), "StatusStripExam.ColorHS.BMP"));
        imglst.Images.Add("Comment", new Bitmap(GetType(), "StatusStripExam.CommentHS.bmp"));

        StatusStrip status = new StatusStrip();
        status.Parent = this;
        status.ImageList = imglst;

        color_btn = new ToolStripButton();
        color_btn.Image = imglst.Images[0];
        color_btn.Click += EventProc;
        status.Items.Add(color_btn);

        comment_btn = new ToolStripButton();
        comment_btn.ImageKey = "Comment";
        comment_btn.Click += EventProc;
        status.Items.Add(comment_btn);

        // 메뉴 구분선 넣기
        ToolStripSeparator item_sep = new ToolStripSeparator();
        status.Items.Add(item_sep);

        txt_box = new ToolStripComboBox();
        txt_box.ToolTipText = "글꼴 선택";
        txt_box.Text = "글꼴 선택";
        txt_box.Items.Add("궁서체");
        txt_box.Items.Add("돋움체");
        txt_box.Items.Add("바탕체");
        status.Items.Add(txt_box);

        label = new ToolStripLabel();
        label.Size = new Size(100, 10);
        status.Items.Add(label);

        // 프로그래스바 설정
        prog_bar = new ToolStripProgressBar();

        prog_bar.Size = new Size(100, 10);
        prog_bar.Maximum = 100;
        prog_bar.Minimum = 0;
        status.Items.Add(prog_bar);

        Timer timer = new Timer();
        timer.Tick += new EventHandler(TimerProc);
        timer.Interval = 1000;
        timer.Start();
    }

    void EventProc(Object sender, EventArgs ea)
    {
        if ((ToolStripButton)sender == color_btn)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.ShowDialog();
        }
        else if ((ToolStripButton)sender == comment_btn)
        {
            MessageBox.Show("StatusStrip 예제입니다.");
        }
    }


    void TimerProc(Object sender, EventArgs ea)
    {
        // 
        label.Text = DateTime.Now.ToLongTimeString();

        //
        if (prog_bar.Value < 100)
            prog_bar.Value += 10;
        else
            prog_bar.Value = 0;

    }


    [STAThread]
    static void Main()
    {
        Application.Run(new StatusStripExam());
    }

}
