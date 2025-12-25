using System;
using System.Drawing;
using System.Windows.Forms;

public class TextBoxExam : Form
{
    Label lbl_txt = new Label();
    TextBox txt1 = new TextBox();  // 단일 라인
    TextBox txt2 = new TextBox();  // 멀티 라인     
    MaskedTextBox mask_txt = new MaskedTextBox();
    ToolTip toolTip1 = new ToolTip();

    public TextBoxExam()
    {
        lbl_txt.Text = "TextBox 예제";
        lbl_txt.Parent = this;
        lbl_txt.BorderStyle = BorderStyle.Fixed3D;
        lbl_txt.SetBounds(10, 10, 500, 20);

        txt1.Parent = this;
        txt1.TextChanged += new EventHandler(txt_TextChanged);
        txt1.BorderStyle = BorderStyle.Fixed3D;
        txt1.SetBounds(10, 50, 500, 20);
        txt1.BackColor = Color.Black;
        txt1.ForeColor = Color.White;

        txt2.Parent = this;
        txt2.TextChanged += new EventHandler(txt_TextChanged);
        txt2.BorderStyle = BorderStyle.Fixed3D;
        txt2.Multiline = true;              // 멀티라인
        txt2.ScrollBars = ScrollBars.Both;  // 스크롤바 설정
        txt2.SetBounds(10, 100, 500, 100);  

        mask_txt.Parent = this;
        mask_txt.TextChanged += new EventHandler(txt_TextChanged);
        mask_txt.BorderStyle = BorderStyle.Fixed3D;
        mask_txt.SetBounds(10, 220, 500, 20);
        mask_txt.PasswordChar = '*';  // 패스워드 문자를 등록
        mask_txt.Mask = "0000/00/00";  // 마스크 형식 지정
        mask_txt.MaskInputRejected += new MaskInputRejectedEventHandler(maskedTextBox1_MaskInputRejected);
        mask_txt.KeyDown += new KeyEventHandler(maskedTextBox1_KeyDown);    // 키 입력 이벤트

        this.Size = new Size(550, 300);
    }

    private void txt_TextChanged(object sender, EventArgs e)
    {
        string str = "";
        if ((TextBoxBase)sender == txt1)
        {
            this.Text = "단일라인 문자열 입력";
            str = txt1.Text;
        }
        else if ((TextBoxBase)sender == txt2)
        {
            this.Text = "멀티라인 문자열 입력";
            str = txt2.Text;
        }
        else if ((TextBoxBase)sender == mask_txt)
        {
            this.Text = "마스크 문자열 입력";
            str = mask_txt.Text;
        }
        lbl_txt.Text = str;
    }

    void maskedTextBox1_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
    {
        if (mask_txt.MaskFull)
        {
            toolTip1.ToolTipTitle = "너무 많은 데이터 입력!";
            toolTip1.Show("입력 가능한 데이터는 8개까지입니다.", mask_txt, mask_txt.Location.X, 20, 5000);
        }
        else
        {
            toolTip1.ToolTipTitle = "입력 문자가 잘못됨";
            toolTip1.Show("입력 가능한 문자는 0~9사이입니다.", mask_txt, mask_txt.Location.X, 20, 5000);
        }
    }

    void maskedTextBox1_KeyDown(object sender, KeyEventArgs e)
    {
        toolTip1.Hide(mask_txt); // 키 입력이 발생하면 툴팁 지움
    }


    public static void Main()
    {
        Application.Run(new TextBoxExam());
    }
}
