using System;
using System.Drawing;
using System.Windows.Forms;

public class ToolStripExam : Form
{
    ToolStripButton copy_btn, copyfolder_btn, cut_btn;
    ToolStripComboBox txt_box;
    int count = 0;

    public ToolStripExam()
    {
        this.Text = "ToolStrip 예제";

        // 이미지리스트 초기화
        ImageList imglst = new ImageList();
        imglst.TransparentColor = Color.Black;
        imglst.Images.Add("Copy", new Bitmap(GetType(), "ToolStripExam.CopyHS.bmp"));
        imglst.Images.Add("CopyFolder", new Bitmap(GetType(), "ToolStripExam.CopyFolderHS.bmp"));
        imglst.Images.Add("Cut", new Bitmap(GetType(), "ToolStripExam.CutHS.bmp"));

        // 툴바 생성
        ToolStrip tool = new ToolStrip();
        tool.Parent = this;     // 부모 
        tool.ImageList = imglst;// 이미지 리스트 설정

        // 1. 툴바에 버튼 생성
        copy_btn = new ToolStripButton();
        copy_btn.ToolTipText = "복사";
        copy_btn.Image = imglst.Images[0];  // 이미지 설정1
        copy_btn.Click += EventProc;
        tool.Items.Add(copy_btn);

        // 2. 툴바에 버튼 생성
        copyfolder_btn = new ToolStripButton();
        copyfolder_btn.ToolTipText = "디렉토리 복사";
        copyfolder_btn.ImageKey = "CopyFolder"; //이미지설정2
        copyfolder_btn.Click += EventProc;
        tool.Items.Add(copyfolder_btn);

        // 3. 메뉴 구분선 넣기
        ToolStripSeparator item_sep = new ToolStripSeparator();
        tool.Items.Add(item_sep);

        // 4. 툴바에 버튼 생성 
        cut_btn = new ToolStripButton();
        cut_btn.ToolTipText = "오려두기";
        cut_btn.ImageIndex = 2;         // 이미지설정3
        cut_btn.Click += EventProc;
        tool.Items.Add(cut_btn);

        // 5. 툴바에 콤보박스 추가 
        txt_box = new ToolStripComboBox();
        txt_box.ToolTipText = "글꼴 선택";
        txt_box.Text = "글꼴 선택";
        txt_box.Items.Add("궁서체");
        txt_box.Items.Add("돋움체");
        txt_box.Items.Add("바탕체");
        txt_box.Click += FontDrawProc;
        tool.Items.Add(txt_box);
    }

    void EventProc(Object sender, EventArgs ea)
    {
        string msg = "";
        if ((ToolStripButton)sender == copy_btn)
        {
            msg = "복사하기 선택";
        }
        else if ((ToolStripButton)sender == copyfolder_btn)
        {
            msg = "디렉토리 복사하기 선택";
        }
        else if ((ToolStripButton)sender == cut_btn)
        {
            msg = "오려두기 선택";
        }
        MessageBox.Show(msg);
    }


    void FontDrawProc(Object sender, EventArgs ea)
    {
        if (count != txt_box.SelectedIndex)
            count = txt_box.SelectedIndex;
        else
            return;

        if (count > 0 && count < 4)
            MessageBox.Show("선택한 글꼴은 " + txt_box.SelectedText);
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new ToolStripExam());
    }

}
