using System;
using System.Drawing;
using System.Windows.Forms;

public class ToolTipExam : Form
{
    ToolTip tooltip;
    Button btn;
    TextBox txt;

    public ToolTipExam()
    {
        tooltip = new ToolTip();

        btn = new Button();
        btn.Parent = this;
        btn.SetBounds(10, 10, 100, 30);
        btn.Text = "버튼";
        btn.Click += new EventHandler(btn_Click);

        txt = new TextBox();
        txt.Parent = this;
        txt.SetBounds(10, 50, 200, 100);
        txt.Text = "이름을 입력하세요>>";

        tooltip.SetToolTip(btn, "버튼을 클릭해 보세요!!");  // 컨트롤과 툴팁연결
        tooltip.SetToolTip(txt, "이름을 입력하는 부분입니다.");
    }

    private void btn_Click(object sender, EventArgs e)
    {
        tooltip.BackColor = Color.Black;    // 툴팁 배경
        tooltip.ForeColor = Color.White;    // 툴팁 글자 

        tooltip.SetToolTip(txt, null);      // TextBox에 할당했던 ToolTip 정보 제가
        tooltip.SetToolTip(btn, "버튼을\n 클릭하셨네요 \n ^^");//Button개체에 툴팁 재설정
    }

    public static void Main()
    {
        Application.Run(new ToolTipExam());
    }
}
