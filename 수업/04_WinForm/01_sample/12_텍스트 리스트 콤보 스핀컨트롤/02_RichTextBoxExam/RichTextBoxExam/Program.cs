using System;
using System.Drawing;
using System.Windows.Forms;

public class RichTextBoxExam : Form
{
    RichTextBox richTextBox;
    public RichTextBoxExam()
    {
        richTextBox = new RichTextBox();
        richTextBox.Parent = this;
        richTextBox.Dock = DockStyle.Fill;
        this.Load += new EventHandler(RichTextBoxExam_Load);
    }

    private void RichTextBoxExam_Load(object sender, EventArgs e)
    {
        richTextBox.LoadFile("song.rtf");        // RTF 파일 읽기
        richTextBox.Find("소나무 철갑을", RichTextBoxFinds.MatchCase);//검색후 반전

        richTextBox.SelectionFont = new Font("궁서체", 30, FontStyle.Bold);//선택된 문자열 글꼴 변경
        richTextBox.SelectionColor = Color.Blue;// 선택된 문자열 색깔 변경
    }

    public static void Main()
    {
        Application.Run(new RichTextBoxExam());
    }
}
