using System;
using System.Drawing;
using System.Windows.Forms;

class ListViewExam2 : Form
{
    ListView lst = null;
    ImageList small_img = null;
    ImageList large_img = null;
    Panel panel = null;
    Button[] btn = new Button[5];

    public ListViewExam2()
    {
        this.Text = "리스트뷰 이벤트 & 이미지";

        LoadImage();

        lst = new ListView();
        lst.Parent = this;
        lst.Dock = DockStyle.Fill;      
        lst.TabIndex = 0;
        lst.View = View.Details;        // 자세히 보기 
        lst.MultiSelect = true;
        lst.HideSelection = false;
        lst.HeaderStyle = ColumnHeaderStyle.Nonclickable;
        lst.SmallImageList = small_img;
        lst.LargeImageList = large_img;
        lst.SelectedIndexChanged += new EventHandler(lst_SelectedIndexChanged);

        // 컬럼 해더 추가 속성 지정
        ColumnHeader columnHeader1 = new ColumnHeader();
        columnHeader1.Text = "국가";
        columnHeader1.TextAlign = HorizontalAlignment.Left;
        columnHeader1.Width = 70;

        ColumnHeader columnHeader2 = new ColumnHeader();
        columnHeader2.Text = "수도";
        columnHeader2.TextAlign = HorizontalAlignment.Center;
        columnHeader2.Width = 70;

        ColumnHeader columnHeader3 = new ColumnHeader();
        columnHeader3.Text = "대륙";
        columnHeader3.TextAlign = HorizontalAlignment.Right;
        columnHeader3.Width = 70;

        // 컬럼 해더 추가 
        lst.Columns.Add(columnHeader1);
        lst.Columns.Add(columnHeader2);
        lst.Columns.Add(columnHeader3);


        // 데이터 추가 
        ListViewItem lvi_korea = new ListViewItem("대한민국");
        lvi_korea.ImageIndex = 0;
        lvi_korea.SubItems.Add("서울");
        lvi_korea.SubItems.Add("아시아");
        lst.Items.Add(lvi_korea);

        ListViewItem lvi_canada = new ListViewItem("캐나다");
        lvi_canada.ImageIndex = 1;
        lvi_canada.SubItems.Add("오타와");
        lvi_canada.SubItems.Add("아메리카");
        lst.Items.Add(lvi_canada);

        ListViewItem lvi_france = new ListViewItem("프랑스");
        lvi_france.ImageIndex = 2;
        lvi_france.SubItems.Add("파리");
        lvi_france.SubItems.Add("유럽");
        lst.Items.Add(lvi_france);

        // 패널 객체 생성
        panel = new Panel();
        panel.Dock = DockStyle.Bottom;  // 패널을 바닥에 붙임 
        panel.Parent = this;

        string[] view_str = { "Details", "LargeIcon", "List", "SmallIcon", "Tile" };
        for (int i = 0; i < 5; i++)
        {
            btn[i] = new Button();
            btn[i].Text = view_str[i];
            btn[i].SetBounds(10 + 55 * i, 20, 55, 40);
            btn[i].Click += new EventHandler(btn_Click);
            panel.Controls.Add(btn[i]);
        }

    }

    void lst_SelectedIndexChanged(object sender, EventArgs ea)
    {
        ListView.SelectedListViewItemCollection select = lst.SelectedItems;

        string msg = "";

        foreach (ListViewItem item in select)
        {
            msg += "나라: " + item.SubItems[0].Text + "\r\n";
            msg += "수도: " + item.SubItems[1].Text + "\r\n";
            msg += "대륙: " + item.SubItems[2].Text + "\r\n";
        }

        MessageBox.Show(msg);
    }

    void btn_Click(object sender, EventArgs ea)
    {
        if ((Button)sender == btn[0])
            this.lst.View = View.Details;
        else if ((Button)sender == btn[1])
            this.lst.View = View.LargeIcon;
        else if ((Button)sender == btn[2])
            this.lst.View = View.List;
        else if ((Button)sender == btn[3])
            this.lst.View = View.SmallIcon;
        else if ((Button)sender == btn[4])
            this.lst.View = View.Tile;
    }

    void LoadImage()
    {
        small_img = new ImageList();
        small_img.TransparentColor = Color.Black;
        small_img.Images.Add(new Bitmap(GetType(), "ListViewExam2.대한민국1.bmp"));
        small_img.Images.Add(new Bitmap(GetType(), "ListViewExam2.캐나다1.bmp"));
        small_img.Images.Add(new Bitmap(GetType(), "ListViewExam2.프랑스1.bmp"));

        large_img = new ImageList();
        large_img.Images.Add(new Bitmap(GetType(), "ListViewExam2.대한민국.bmp"));
        large_img.Images.Add(new Bitmap(GetType(), "ListViewExam2.캐나다.bmp"));
        large_img.Images.Add(new Bitmap(GetType(), "ListViewExam2.프랑스.bmp"));
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new ListViewExam2());
    }
}