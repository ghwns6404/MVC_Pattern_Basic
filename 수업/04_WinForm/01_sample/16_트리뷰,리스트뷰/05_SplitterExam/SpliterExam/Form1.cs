using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SpliterExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            this.Text = "Splitter 컨트롤 사용하기";

            TreeView treeView = new TreeView();
            ListView listView = new ListView();
            Splitter splitter = new Splitter(); // splitter 개체 생성(과거버전)

            treeView.Dock = DockStyle.Left; // 트리뷰를 폼의 외쪽에 배치
            splitter.Dock = DockStyle.Left; // 스플리터를 폼의 왼쪽에 배치

            splitter.MinExtra = 100;    // 스플리터와 컨테이너 반대쪽 가장자리 사이의 간격
            splitter.MinSize = 75;      // 스플리터와 컨트롤 간의 최소 간격

            listView.Dock = DockStyle.Fill; // 리스트뷰는 화면 전체에 채움 

            treeView.Nodes.Add("트리 노드1");
            treeView.Nodes.Add("트리 노드2");
            listView.Items.Add("리스트 아이템1");
            listView.Items.Add("리스트 아이템2");

            // 폼에 리스트뷰, 스플리터, 트리뷰 컨트롤을 추가 
            this.Controls.AddRange(new Control[] { listView, splitter, treeView });


        }
    }
}