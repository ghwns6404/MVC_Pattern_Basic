using System;
using System.Drawing;
using System.Windows.Forms;

public class ListViewExam : Form
{
    public ListViewExam()
    {
        this.Text = "ListView 예제";
        ListView lst = new ListView();
        lst.Parent = this;
        lst.Dock = DockStyle.Fill;
        lst.View = View.Details;        // 컬럼 정보가 출력

        // 컬럼 해더 추가 
        lst.Columns.Add("국가", 70, HorizontalAlignment.Left);
        lst.Columns.Add("수도", 70, HorizontalAlignment.Center);
        lst.Columns.Add("대륙", 70, HorizontalAlignment.Right);

        // 데이터 추가 
        ListViewItem lvi_korea = new ListViewItem("대한민국");
        lvi_korea.SubItems.Add("서울");
        lvi_korea.SubItems.Add("아시아");

        ListViewItem lvi_canada = new ListViewItem("캐나다");
        lvi_canada.SubItems.Add("오타와");
        lvi_canada.SubItems.Add("아메리카");

        ListViewItem lvi_france = new ListViewItem("프랑스");
        lvi_france.SubItems.Add("파리");
        lvi_france.SubItems.Add("유럽");

        lst.Items.Add(lvi_korea);
        lst.Items.Add(lvi_canada);
        lst.Items.Add(lvi_france);
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new ListViewExam());
    }
}