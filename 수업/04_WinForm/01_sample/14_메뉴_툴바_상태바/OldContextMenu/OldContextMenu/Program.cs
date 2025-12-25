using System;
using System.Drawing;
using System.Windows.Forms;

public class OldContextMenu : Form
{
    public OldContextMenu()
    {
        this.Text = "Menu 예제. 닷넷 1.x 형태";

        ContextMenu menu = new ContextMenu();
        this.ContextMenu = menu;

        MenuItem country = new MenuItem("&국가");
        menu.MenuItems.Add(country);

        MenuItem korea = new MenuItem();
        korea.Text = "&대한민국";
        korea.Click += new EventHandler(MenuEvent);
        korea.Shortcut = Shortcut.CtrlK;
        country.MenuItems.Add(korea);

        MenuItem canada = new MenuItem("&캐나다", new EventHandler(MenuEvent), Shortcut.CtrlC);
        country.MenuItems.Add(canada);
        MenuItem france = new MenuItem("&프랑스", new EventHandler(MenuEvent), Shortcut.CtrlF);
        country.MenuItems.Add(france);

        MenuItem sep = new MenuItem("-");
        menu.MenuItems.Add(sep);

        MenuItem coninent = new MenuItem("대륙");
        menu.MenuItems.Add(coninent);

        MenuItem asia = new MenuItem("&아시아", new EventHandler(MenuEvent), Shortcut.Alt1);
        coninent.MenuItems.Add(asia);
        MenuItem euro = new MenuItem("유&럽", new EventHandler(MenuEvent), Shortcut.Alt2);
        coninent.MenuItems.Add(euro);


    }

    void MenuEvent(object sender, EventArgs ea)
    {
        MessageBox.Show(((MenuItem)sender).Text + " : 메뉴 선택");
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new OldContextMenu());
    }
}