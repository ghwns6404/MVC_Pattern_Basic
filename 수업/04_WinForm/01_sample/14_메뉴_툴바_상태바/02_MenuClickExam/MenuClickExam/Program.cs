using System;
using System.Windows.Forms;

public class MenuClickExam : Form
{
    [STAThread]
    static void Main()
    {
        Application.Run(new MenuClickExam());
    }

    public MenuClickExam()
    {
        this.Text = "MenuStrip 예제";
        MenuStrip menu = new MenuStrip();
        menu.Parent = this;

        // File 항목
        ToolStripMenuItem file_item = new ToolStripMenuItem();
        file_item.Text = "&File";
        menu.Items.Add(file_item);

        ToolStripMenuItem file_open_item = new ToolStripMenuItem();
        file_open_item.Text = "&Open...";
        file_open_item.ShortcutKeys = Keys.Control | Keys.O;
        file_open_item.Click += FileOpenDlg;
        file_item.DropDownItems.Add(file_open_item);

        // 메뉴 구분선 넣기
        ToolStripSeparator file_item_sep = new ToolStripSeparator();
        file_item.DropDownItems.Add(file_item_sep);

        ToolStripMenuItem file_close_item = new ToolStripMenuItem();
        file_close_item.Text = "&Close";
        file_close_item.ShortcutKeys = Keys.Alt | Keys.F4;
        file_close_item.Click += CloseProgram;
        file_item.DropDownItems.Add(file_close_item);
    }

    void FileOpenDlg(object obj, EventArgs args)
    {
        OpenFileDialog dlg = new OpenFileDialog();
        dlg.Title = "파일 선택";
        if (dlg.ShowDialog() == DialogResult.OK)
        {
            MessageBox.Show(dlg.FileName + " : 파일 선택");
        }
    }

    void CloseProgram(object obj, EventArgs args)
    {
        this.Close();
    }
}
