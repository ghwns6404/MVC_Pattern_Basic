using System;
using System.Drawing;
using System.Windows.Forms;

public class ContextMenuStripExam : Form
{
    ToolStripMenuItem select_item;
    ContextMenuStrip contextmenu;

    public ContextMenuStripExam()
    {
        this.Text = "ContextMenuStrip 예제";

        Bitmap bmp1 = new Bitmap(GetType(), "ContextMenuStripExam.image_1.bmp");
        Bitmap bmp2 = new Bitmap(GetType(), "ContextMenuStripExam.image_2.bmp");


        // 2) 마우스 이벤트 추가 
        this.MouseClick += new MouseEventHandler(ImageMenu_MouseClick);

        // 1) 개체 생성
        contextmenu = new ContextMenuStrip();

        // File 항목
        ToolStripMenuItem file_item = new ToolStripMenuItem();
        file_item.Text = "&File";
        file_item.Image = bmp1;
        contextmenu.Items.Add(file_item);// 컨텍스트 메뉴에 아이템 추가

        select_item = new ToolStripMenuItem();
        select_item.Text = "&Select";
        select_item.Click += EventProc;
        file_item.DropDownItems.Add(select_item);

        // 메뉴 구분선 넣기
        ToolStripSeparator file_item_sep = new ToolStripSeparator();
        file_item.DropDownItems.Add(file_item_sep);

        ToolStripMenuItem close_item = new ToolStripMenuItem();
        close_item.Text = "&Close";
        close_item.Image = bmp2;
        close_item.ShortcutKeys = Keys.Alt | Keys.F4;
        close_item.Click += EventProc;
        file_item.DropDownItems.Add(close_item);

    }

    void EventProc(object sender, EventArgs ea)
    {
        if ((ToolStripMenuItem)sender == select_item)
        {
            select_item.Checked = (select_item.Checked) ? false : true;
        }
        else
        {
            MessageBox.Show("프로그램 종료!!!");
            this.Close();
        }
    }

    //3) 마우스 오른쪽 버튼 클릭시 컨텍스트 메뉴 출력 
    void ImageMenu_MouseClick(object sender, MouseEventArgs ea)
    {
        if (ea.Button == MouseButtons.Right)
        {
            int x = ea.X + this.Left;
            int y = ea.Y + this.Top;

            this.contextmenu.Show(x, y);//컨텍스트 메뉴 출력
        }
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new ContextMenuStripExam());
    }

}

