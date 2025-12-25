using System;
using System.Drawing;
using System.Windows.Forms;

public class ImageTreeview : Form
{
    ImageList imglst;

    public ImageTreeview()
    {
        this.Text = "Treeview 예제";
        this.LoadIcon();

        TreeView tree = new TreeView();
        tree.ImageList = imglst;    // 트리뷰에 이미지 리스트 연결
        tree.Parent = this;
        tree.Dock = DockStyle.Fill;
        tree.AfterSelect += new TreeViewEventHandler(tree_AfterSelect); // 이벤트 핸들러 등록

        tree.Nodes.Add("가전제품");
        tree.Nodes[0].Nodes.Add("CD");
        tree.Nodes[0].Nodes[0].ImageIndex = 1;
        tree.Nodes[0].Nodes.Add("카메라");
        tree.Nodes[0].Nodes[1].ImageIndex = 2;
        tree.Nodes[0].Nodes.Add("휴대폰");
        tree.Nodes[0].Nodes[2].ImageIndex = 3;
        tree.Nodes[0].Nodes.Add("팩스");
        tree.Nodes[0].Nodes[3].ImageIndex = 4;

        tree.Nodes.Add("검색하기");
        tree.Nodes[1].Nodes.Add("단순 찾기");
        tree.Nodes[1].Nodes[0].ImageIndex = 5;
        tree.Nodes[1].Nodes.Add("폴더 찾기");
        tree.Nodes[1].Nodes[1].ImageIndex = 6;
        tree.Nodes[1].Nodes.Add("문서 찾기");
        tree.Nodes[1].Nodes[2].ImageIndex = 7;
        tree.Nodes[1].Nodes.Add("사람 찾기");
        tree.Nodes[1].Nodes[3].ImageIndex = 8;
        tree.Nodes[1].Nodes.Add("프린터 찾기");
        tree.Nodes[1].Nodes[4].ImageIndex = 9;
        tree.Nodes[1].Nodes.Add("웹 검색");
        tree.Nodes[1].Nodes[5].ImageIndex = 10;

        tree.Nodes.Add("서비스");
        tree.Nodes[2].Nodes.Add("서비스 중지");
        tree.Nodes[2].Nodes[0].ImageIndex = 11;
        tree.Nodes[2].Nodes.Add("서비스 시작");
        tree.Nodes[2].Nodes[1].ImageIndex = 12;
        tree.Nodes[2].Nodes.Add("서비스 대기");
        tree.Nodes[2].Nodes[2].ImageIndex = 13;
        tree.Nodes[2].Nodes.Add("서비스 중지");
        tree.Nodes[2].Nodes[3].ImageIndex = 14;
        tree.Nodes[2].Nodes.Add("서비스 에러");
        tree.Nodes[2].Nodes[4].ImageIndex = 15;
    }

    private void LoadIcon()
    {
        imglst = new ImageList();
        imglst.Images.Add("Messages", new Icon(GetType(), "ImageTreeview.Messages.ico"));
        imglst.Images.Add("blackcd", new Icon(GetType(), "ImageTreeview.blankcd.ico"));
        imglst.Images.Add("camera", new Icon(GetType(), "ImageTreeview.camera.ico"));
        imglst.Images.Add("cellphone", new Icon(GetType(), "ImageTreeview.cellphone.ico"));
        imglst.Images.Add("fax", new Icon(GetType(), "ImageTreeview.fax.ico"));
        imglst.Images.Add("search", new Icon(GetType(), "ImageTreeview.search.ico"));
        imglst.Images.Add("search4doc", new Icon(GetType(), "ImageTreeview.search4doc.ico"));
        imglst.Images.Add("search4files", new Icon(GetType(), "ImageTreeview.search4files.ico"));
        imglst.Images.Add("search4people", new Icon(GetType(), "ImageTreeview.search4people.ico"));
        imglst.Images.Add("search4printer", new Icon(GetType(), "ImageTreeview.search4printer.ico"));
        imglst.Images.Add("searchweb", new Icon(GetType(), "ImageTreeview.searchweb.ico"));
        imglst.Images.Add("servicepaused", new Icon(GetType(), "ImageTreeview.servicepaused.ico"));
        imglst.Images.Add("servicerunning", new Icon(GetType(), "ImageTreeview.servicerunning.ico"));
        imglst.Images.Add("services", new Icon(GetType(), "ImageTreeview.services.ico"));
        imglst.Images.Add("servicestopped", new Icon(GetType(), "ImageTreeview.servicestopped.ico"));
        imglst.Images.Add("serviceunknown", new Icon(GetType(), "ImageTreeview.serviceunknown.ico"));
    }

    private void tree_AfterSelect(object sender, TreeViewEventArgs tea)
    {
        if (tea.Action != TreeViewAction.Unknown)
        {
            tea.Node.Checked = (tea.Node.Checked) ? false : true;
            this.Text = "현재 선택한 목록 : " + tea.Node.Text;
        }
    }

    [STAThread]
    static void Main()
    {
        Application.Run(new ImageTreeview());
    }

}