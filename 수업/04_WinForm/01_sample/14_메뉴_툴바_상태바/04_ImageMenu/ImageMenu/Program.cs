using System;
using System.Drawing;
using System.Windows.Forms;

public class ImageMenu : Form
{
    ToolStripMenuItem select_item;
    public ImageMenu()
    {
        this.Text = "메뉴 선택 표시와 이미지 넣기";

        // 이미지 개체 준비 
        Bitmap bmp1 = new Bitmap(GetType(), "ImageMenu.image_1.bmp");
        Bitmap bmp2 = new Bitmap(GetType(), "ImageMenu.image_2.bmp");

        MenuStrip menu = new MenuStrip();
        menu.Parent = this;

        // File 항목
        ToolStripMenuItem file_item = new ToolStripMenuItem();
        file_item.Text = "&File";
        file_item.Image = bmp1;         // 메뉴에 출력할 이미지 지정
        menu.Items.Add(file_item);

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

    // 메뉴 체크 표시 핸들러 
    void EventProc(Object sender, EventArgs ea)
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

    static void Main()
    {
        Application.Run(new ImageMenu());
    }

}
