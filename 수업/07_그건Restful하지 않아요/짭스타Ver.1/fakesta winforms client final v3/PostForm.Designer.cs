namespace FakeStar
{
    partial class PostForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.ListView listViewPosts;
        private System.Windows.Forms.ColumnHeader columnId;
        private System.Windows.Forms.ColumnHeader columnTitle;
        private System.Windows.Forms.ColumnHeader columnContent;
        private System.Windows.Forms.ColumnHeader columnFilename;
        private System.Windows.Forms.Button buttonAllPosts;
        private System.Windows.Forms.TextBox textBoxId;
        private System.Windows.Forms.Button buttonSearch;
        private System.Windows.Forms.Label label1;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.listViewPosts = new System.Windows.Forms.ListView();
            this.columnId = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnTitle = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnContent = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnFilename = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonAllPosts = new System.Windows.Forms.Button();
            this.textBoxId = new System.Windows.Forms.TextBox();
            this.buttonSearch = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // listViewPosts
            // 
            this.listViewPosts.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnId,
            this.columnTitle,
            this.columnContent,
            this.columnFilename});
            this.listViewPosts.FullRowSelect = true;
            this.listViewPosts.GridLines = true;
            this.listViewPosts.HideSelection = false;
            this.listViewPosts.Location = new System.Drawing.Point(20, 60);
            this.listViewPosts.Name = "listViewPosts";
            this.listViewPosts.Size = new System.Drawing.Size(500, 250);
            this.listViewPosts.TabIndex = 0;
            this.listViewPosts.UseCompatibleStateImageBehavior = false;
            this.listViewPosts.View = System.Windows.Forms.View.Details;
            // 
            // columnId
            // 
            this.columnId.Text = "ID";
            this.columnId.Width = 80;
            // 
            // columnTitle
            // 
            this.columnTitle.Text = "제목";
            this.columnTitle.Width = 120;
            // 
            // columnContent
            // 
            this.columnContent.Text = "내용";
            this.columnContent.Width = 180;
            // 
            // columnFilename
            // 
            this.columnFilename.Text = "파일명";
            this.columnFilename.Width = 100;
            // 
            // buttonAllPosts
            // 
            this.buttonAllPosts.Location = new System.Drawing.Point(20, 20);
            this.buttonAllPosts.Name = "buttonAllPosts";
            this.buttonAllPosts.Size = new System.Drawing.Size(90, 28);
            this.buttonAllPosts.TabIndex = 1;
            this.buttonAllPosts.Text = "전체게시물";
            this.buttonAllPosts.UseVisualStyleBackColor = true;
            this.buttonAllPosts.Click += new System.EventHandler(this.buttonAllPosts_Click);
            // 
            // textBoxId
            // 
            this.textBoxId.Location = new System.Drawing.Point(180, 23);
            this.textBoxId.Name = "textBoxId";
            this.textBoxId.Size = new System.Drawing.Size(100, 21);
            this.textBoxId.TabIndex = 2;
            // 
            // buttonSearch
            // 
            this.buttonSearch.Location = new System.Drawing.Point(290, 20);
            this.buttonSearch.Name = "buttonSearch";
            this.buttonSearch.Size = new System.Drawing.Size(60, 28);
            this.buttonSearch.TabIndex = 3;
            this.buttonSearch.Text = "검색";
            this.buttonSearch.UseVisualStyleBackColor = true;
            this.buttonSearch.Click += new System.EventHandler(this.buttonSearch_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(130, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 15);
            this.label1.TabIndex = 4;
            this.label1.Text = "ID:";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(399, 23);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 5;
            this.button1.Text = "삭제";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // PostForm
            // 
            this.ClientSize = new System.Drawing.Size(540, 330);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.listViewPosts);
            this.Controls.Add(this.buttonAllPosts);
            this.Controls.Add(this.textBoxId);
            this.Controls.Add(this.buttonSearch);
            this.Controls.Add(this.label1);
            this.Name = "PostForm";
            this.Text = "게시물 관리";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private System.Windows.Forms.Button button1;
    }
}
