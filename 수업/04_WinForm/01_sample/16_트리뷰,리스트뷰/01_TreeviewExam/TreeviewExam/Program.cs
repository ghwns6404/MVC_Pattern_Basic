using System;
using System.Drawing;
using System.Windows.Forms;

public class TreeviewExam : Form
{
    public TreeviewExam()
    {
        this.Text = "Treeview 예제";

        TreeView tree = new TreeView();
        tree.Parent = this;
        tree.Dock = DockStyle.Fill;// 트리뷰를 폼 전체 영역에 그림
        
        tree.Nodes.Add("대한민국");
        tree.Nodes[0].Nodes.Add("서울시");
        tree.Nodes[0].Nodes[0].Nodes.Add("종로구");
        tree.Nodes[0].Nodes[0].Nodes.Add("광진구");
        tree.Nodes[0].Nodes[0].Nodes.Add("강남구");
        tree.Nodes[0].Nodes.Add("경기도");
        tree.Nodes[0].Nodes[1].Nodes.Add("수원시");
        tree.Nodes[0].Nodes[1].Nodes.Add("가평군");

        tree.Nodes.Add("미국");
        tree.Nodes[1].Nodes.Add("캘리포니아주");
        tree.Nodes[1].Nodes[0].Nodes.Add("LA");
        tree.Nodes[1].Nodes.Add("뉴욕주");

    }

    [STAThread]
    static void Main()
    {
        Application.Run(new TreeviewExam());
    }

}