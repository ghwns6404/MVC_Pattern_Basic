using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SplitContainerExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            this.Text = "SplitContainer 사용하기";

            TreeView treeView = new TreeView();
            ListView listView = new ListView();

            treeView.Dock = DockStyle.Fill;
            listView.Dock = DockStyle.Fill;
            treeView.Nodes.Add("트리 노드1");
            treeView.Nodes.Add("트리 노드2");
            listView.Items.Add("리스트 아이템1");
            listView.Items.Add("리스트 아이템2");


            // 스풀리터 컨트롤 현재 버전(권장)
            SplitContainer splitContainer = new SplitContainer();
            splitContainer.Dock = DockStyle.Fill;
            splitContainer.Panel1MinSize = 30;  // Pane1의 최소 크기 30
            splitContainer.Panel2MinSize = 50;  // Pane2의 최소 크기 50
            splitContainer.SplitterWidth = 1;  // 스플리터 폭을 1으로 설정
            splitContainer.SplitterIncrement = 5;   // 스프리터 이동간격을 5로 설정
            
            splitContainer.Panel1.Controls.Add(treeView);   //스플리터 왼쪽에 트리뷰
            splitContainer.Panel2.Controls.Add(listView);   // 스플리터 오른쪽에 리스트뷰

            this.Controls.Add(splitContainer);
        }
    }
}