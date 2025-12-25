namespace FileExplore
{
    partial class MainWnd
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.menuClose = new System.Windows.Forms.ToolStripMenuItem();
            this.menuView = new System.Windows.Forms.ToolStripMenuItem();
            this.menuLargeIcon = new System.Windows.Forms.ToolStripMenuItem();
            this.menuSmallIcon = new System.Windows.Forms.ToolStripMenuItem();
            this.menuDetail = new System.Windows.Forms.ToolStripMenuItem();
            this.menuList = new System.Windows.Forms.ToolStripMenuItem();
            this.treeView = new System.Windows.Forms.TreeView();
            this.imglstTree = new System.Windows.Forms.ImageList(this.components);
            this.listView = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.imglstLarge = new System.Windows.Forms.ImageList(this.components);
            this.imglstSmall = new System.Windows.Forms.ImageList(this.components);
            this.splitter2 = new System.Windows.Forms.Splitter();
            this.SearchTimer = new System.Windows.Forms.Timer(this.components);
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.status = new System.Windows.Forms.StatusStrip();
            this.status_txt = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip1.SuspendLayout();
            this.status.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFile,
            this.menuView});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(569, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // menuFile
            // 
            this.menuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuClose});
            this.menuFile.Name = "menuFile";
            this.menuFile.Size = new System.Drawing.Size(41, 20);
            this.menuFile.Text = "파일";
            // 
            // menuClose
            // 
            this.menuClose.Name = "menuClose";
            this.menuClose.Size = new System.Drawing.Size(100, 22);
            this.menuClose.Text = "종료";
            this.menuClose.Click += new System.EventHandler(this.menuClose_Click);
            // 
            // menuView
            // 
            this.menuView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuLargeIcon,
            this.menuSmallIcon,
            this.menuDetail,
            this.menuList});
            this.menuView.Name = "menuView";
            this.menuView.Size = new System.Drawing.Size(41, 20);
            this.menuView.Text = "보기";
            // 
            // menuLargeIcon
            // 
            this.menuLargeIcon.Name = "menuLargeIcon";
            this.menuLargeIcon.Size = new System.Drawing.Size(136, 22);
            this.menuLargeIcon.Text = "큰아이콘";
            this.menuLargeIcon.Click += new System.EventHandler(this.menuLargeIcon_Click);
            // 
            // menuSmallIcon
            // 
            this.menuSmallIcon.Name = "menuSmallIcon";
            this.menuSmallIcon.Size = new System.Drawing.Size(136, 22);
            this.menuSmallIcon.Text = "작은아이콘";
            this.menuSmallIcon.Click += new System.EventHandler(this.menuSmallIcon_Click);
            // 
            // menuDetail
            // 
            this.menuDetail.Name = "menuDetail";
            this.menuDetail.Size = new System.Drawing.Size(136, 22);
            this.menuDetail.Text = "자세히";
            this.menuDetail.Click += new System.EventHandler(this.menuDetail_Click);
            // 
            // menuList
            // 
            this.menuList.Name = "menuList";
            this.menuList.Size = new System.Drawing.Size(136, 22);
            this.menuList.Text = "간단히";
            this.menuList.Click += new System.EventHandler(this.menuList_Click);
            // 
            // treeView
            // 
            this.treeView.Dock = System.Windows.Forms.DockStyle.Left;
            this.treeView.ImageIndex = 0;
            this.treeView.ImageList = this.imglstTree;
            this.treeView.Location = new System.Drawing.Point(0, 24);
            this.treeView.Name = "treeView";
            this.treeView.SelectedImageIndex = 0;
            this.treeView.Size = new System.Drawing.Size(137, 342);
            this.treeView.TabIndex = 1;
            this.treeView.AfterCollapse += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterCollapse);
            this.treeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterSelect);
            this.treeView.AfterExpand += new System.Windows.Forms.TreeViewEventHandler(this.treeView_AfterExpand);
            // 
            // imglstTree
            // 
            this.imglstTree.ColorDepth = System.Windows.Forms.ColorDepth.Depth16Bit;
            this.imglstTree.ImageSize = new System.Drawing.Size(16, 16);
            this.imglstTree.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // listView
            // 
            this.listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.listView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView.LargeImageList = this.imglstLarge;
            this.listView.Location = new System.Drawing.Point(137, 24);
            this.listView.Name = "listView";
            this.listView.Size = new System.Drawing.Size(432, 342);
            this.listView.SmallImageList = this.imglstSmall;
            this.listView.TabIndex = 2;
            this.listView.UseCompatibleStateImageBehavior = false;
            this.listView.View = System.Windows.Forms.View.Details;
            this.listView.DoubleClick += new System.EventHandler(this.listView_DoubleClick);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "이름";
            this.columnHeader1.Width = 200;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "크기";
            this.columnHeader2.Width = 80;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "종류";
            this.columnHeader3.Width = 70;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "수정한 날짜";
            this.columnHeader4.Width = 150;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "특성";
            // 
            // imglstLarge
            // 
            this.imglstLarge.ColorDepth = System.Windows.Forms.ColorDepth.Depth16Bit;
            this.imglstLarge.ImageSize = new System.Drawing.Size(32, 32);
            this.imglstLarge.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // imglstSmall
            // 
            this.imglstSmall.ColorDepth = System.Windows.Forms.ColorDepth.Depth16Bit;
            this.imglstSmall.ImageSize = new System.Drawing.Size(16, 16);
            this.imglstSmall.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // splitter2
            // 
            this.splitter2.Dock = System.Windows.Forms.DockStyle.Right;
            this.splitter2.Location = new System.Drawing.Point(566, 24);
            this.splitter2.Name = "splitter2";
            this.splitter2.Size = new System.Drawing.Size(3, 342);
            this.splitter2.TabIndex = 4;
            this.splitter2.TabStop = false;
            // 
            // SearchTimer
            // 
            this.SearchTimer.Tick += new System.EventHandler(this.SearchTimer_Tick);
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(137, 24);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(3, 342);
            this.splitter1.TabIndex = 5;
            this.splitter1.TabStop = false;
            // 
            // status
            // 
            this.status.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.status_txt});
            this.status.Location = new System.Drawing.Point(140, 344);
            this.status.Name = "status";
            this.status.Size = new System.Drawing.Size(426, 22);
            this.status.TabIndex = 6;
            this.status.Text = "...";
            // 
            // status_txt
            // 
            this.status_txt.BorderStyle = System.Windows.Forms.Border3DStyle.RaisedInner;
            this.status_txt.Name = "status_txt";
            this.status_txt.Size = new System.Drawing.Size(17, 17);
            this.status_txt.Text = "...";
            // 
            // MainWnd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(569, 366);
            this.Controls.Add(this.status);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this.splitter2);
            this.Controls.Add(this.listView);
            this.Controls.Add(this.treeView);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWnd";
            this.Text = "파일 탐색기";
            this.Load += new System.EventHandler(this.MainWnd_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.status.ResumeLayout(false);
            this.status.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuFile;
        private System.Windows.Forms.ToolStripMenuItem menuClose;
        private System.Windows.Forms.ToolStripMenuItem menuView;
        private System.Windows.Forms.ToolStripMenuItem menuLargeIcon;
        private System.Windows.Forms.ToolStripMenuItem menuSmallIcon;
        private System.Windows.Forms.ToolStripMenuItem menuDetail;
        private System.Windows.Forms.ToolStripMenuItem menuList;
        private System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.ListView listView;
        internal System.Windows.Forms.ImageList imglstLarge;
        internal System.Windows.Forms.ImageList imglstTree;
        internal System.Windows.Forms.Splitter splitter2;
        internal System.Windows.Forms.ImageList imglstSmall;
        internal System.Windows.Forms.Timer SearchTimer;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.StatusStrip status;
        private System.Windows.Forms.ToolStripStatusLabel status_txt;
        private System.Windows.Forms.ColumnHeader columnHeader5;

    }
}

