using System;
using System.Windows.Forms;

public class MenuExam : Form
{
    static void Main()
    {
        Application.Run(new MenuExam());
    }

    public MenuExam()
    {
        this.Text = "MenuStrip 예제";
        MenuStrip menu = new MenuStrip();
        menu.Parent = this;     // Form에 MenuStrip을 지정

        // File 항목 생성 및 화면 출력
        ToolStripMenuItem file_item = new ToolStripMenuItem();
        file_item.Text = "&File";
        menu.Items.Add(file_item);
       
        ToolStripMenuItem file_open_item = new ToolStripMenuItem();
        file_open_item.Text = "&Open";
        file_item.DropDownItems.Add(file_open_item);

        // 메뉴 구분선 넣기
        ToolStripSeparator file_item_sep = new ToolStripSeparator();
        file_item.DropDownItems.Add(file_item_sep);

        ToolStripMenuItem file_close_item = new ToolStripMenuItem();
        file_close_item.Text = "&Close";
        file_item.DropDownItems.Add(file_close_item);

        // Edit 항목
        ToolStripMenuItem edit_item = new ToolStripMenuItem();
        edit_item.Text = "&Edit";
        menu.Items.Add(edit_item);

        ToolStripMenuItem edit_1_item = new ToolStripMenuItem();
        edit_1_item.Text = "&&Copy";   // & 를 화면에 출력
        edit_item.DropDownItems.Add(edit_1_item);

        ToolStripMenuItem edit_2_item = new ToolStripMenuItem();
        edit_2_item.Text = "Pa&st";    // s 에 밑줄 넣기
        edit_1_item.DropDownItems.Add(edit_2_item);
        
    }
}
