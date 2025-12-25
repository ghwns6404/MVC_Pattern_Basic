using System;
using System.Drawing;
using System.Windows.Forms;

public class TabControlExam : Form
{
    private TabControl tabControl;
    private TabPage tabPage1;
    private TabPage tabPage2;
    private Button btn_tabpage1;

    public TabControlExam()
    {
        this.Text = "TabControl 예제";

        tabControl = new TabControl();
        tabControl.Selected += new TabControlEventHandler(tabControl_Selected);

        tabPage1 = new TabPage();
        tabPage1.Text = "첫번째 Tab";
        tabPage1.BackColor = Color.Brown;
        btn_tabpage1 = new Button();
        btn_tabpage1.Text = "버튼을 클릭하세요!";
        btn_tabpage1.SetBounds(30, 60, 150, 50);
        btn_tabpage1.BackColor = Color.White;
        btn_tabpage1.Parent = tabPage1;
        btn_tabpage1.Click += new System.EventHandler(btn_tabpage1_Click);

        tabPage2 = new TabPage();
        tabPage2.Text = "두번째 Tab";
        tabPage2.Paint += new PaintEventHandler(tabPage2_Paint);

        // TabControl에 Tabpage를 추가 
        tabControl.Controls.AddRange(new Control[] { tabPage1, tabPage2 });

        // AddRange 대신 속성을 통한 추가 가능
        //tabPage1.Parent = tabControl;
        //tabPage2.Parent = tabControl;

        tabControl.Location = new Point(25, 25);
        tabControl.Size = new Size(250, 250);

        this.ClientSize = new Size(300, 300);

        // TabContrl을 윈폼에 출력하는 코드 
        Controls.AddRange(new Control[] { this.tabControl });

        // 위의 코드와 동일 처리 
        //tabControl.Parent = this;
    }

    private void tabControl_Selected(object sender, TabControlEventArgs e)
    {
        this.Text = e.TabPage.Text + "를 선택!!";
    }

    private void btn_tabpage1_Click(object sender, EventArgs e)
    {
        MessageBox.Show("첫번째 탭에서 버튼을 클릭하셨군요 *^^*");
    }

    private void tabPage2_Paint(object sender, PaintEventArgs e)
    {
        Graphics g = e.Graphics;
        g.FillEllipse(Brushes.Red, 10, 10, 100, 100);
        g.DrawString("TabControl 예제!!", this.Font, Brushes.Black, 50, 50);
    }

    static void Main()
    {
        Application.Run(new TabControlExam());
    }
}

