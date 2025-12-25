using System;
using System.Drawing;
using System.Windows.Forms;

public class OldMenuExam : Form
{
    public OldMenuExam()
    {
        this.Text = "Menu 예제. 닷넷 1.x 형태";

        MenuItem file_open = new MenuItem("&Open", new EventHandler(MenuEvent), Shortcut.CtrlO);
        MenuItem file_sep = new MenuItem("-");
        MenuItem file_close = new MenuItem("&Close", new EventHandler(MenuEvent), Shortcut.AltF4);

        MenuItem file = new MenuItem("&File", new MenuItem[] { file_open, file_sep, file_close });


        MenuItem edit_copy = new MenuItem("&Copy", new EventHandler(MenuEvent), Shortcut.CtrlC);
        MenuItem edit_paste = new MenuItem("Pa&st", new EventHandler(MenuEvent), Shortcut.CtrlShift5);

        MenuItem edit = new MenuItem("&Edit", new MenuItem[] { edit_copy, edit_paste });

        MainMenu menu = new MainMenu(new MenuItem[] { file, edit });
        this.Menu = menu;

    }

    void MenuEvent(object sender, EventArgs ea)
    {
        MessageBox.Show(((MenuItem)sender).Text + " : 메뉴 선택");
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new OldMenuExam());
    }
}