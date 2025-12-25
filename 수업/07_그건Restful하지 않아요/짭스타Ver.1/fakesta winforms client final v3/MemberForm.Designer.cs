namespace FakeStar
{
    partial class MemberForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.ListView listViewMembers;
        private System.Windows.Forms.ColumnHeader columnId;
        private System.Windows.Forms.ColumnHeader columnName;
        private System.Windows.Forms.ColumnHeader columnCreatedAt;
        private System.Windows.Forms.Button buttonAllMembers;
        private System.Windows.Forms.TextBox textBoxId;
        private System.Windows.Forms.Button buttonSearch;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonMyInfo; // 추가

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
            this.listViewMembers = new System.Windows.Forms.ListView();
            this.columnId = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnCreatedAt = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonAllMembers = new System.Windows.Forms.Button();
            this.textBoxId = new System.Windows.Forms.TextBox();
            this.buttonSearch = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonMyInfo = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // listViewMembers
            // 
            this.listViewMembers.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnId,
            this.columnName,
            this.columnCreatedAt});
            this.listViewMembers.FullRowSelect = true;
            this.listViewMembers.GridLines = true;
            this.listViewMembers.HideSelection = false;
            this.listViewMembers.Location = new System.Drawing.Point(20, 60);
            this.listViewMembers.Name = "listViewMembers";
            this.listViewMembers.Size = new System.Drawing.Size(400, 250);
            this.listViewMembers.TabIndex = 0;
            this.listViewMembers.UseCompatibleStateImageBehavior = false;
            this.listViewMembers.View = System.Windows.Forms.View.Details;
            // 
            // columnId
            // 
            this.columnId.Text = "ID";
            this.columnId.Width = 100;
            // 
            // columnName
            // 
            this.columnName.Text = "이름";
            this.columnName.Width = 120;
            // 
            // columnCreatedAt
            // 
            this.columnCreatedAt.Text = "가입일";
            this.columnCreatedAt.Width = 150;
            // 
            // buttonAllMembers
            // 
            this.buttonAllMembers.Location = new System.Drawing.Point(20, 20);
            this.buttonAllMembers.Name = "buttonAllMembers";
            this.buttonAllMembers.Size = new System.Drawing.Size(90, 28);
            this.buttonAllMembers.TabIndex = 1;
            this.buttonAllMembers.Text = "전체회원";
            this.buttonAllMembers.UseVisualStyleBackColor = true;
            this.buttonAllMembers.Click += new System.EventHandler(this.buttonAllMembers_Click);
            // 
            // textBoxId
            // 
            this.textBoxId.Location = new System.Drawing.Point(180, 23);
            this.textBoxId.Name = "textBoxId";
            this.textBoxId.Size = new System.Drawing.Size(100, 25);
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
            // buttonMyInfo
            // 
            this.buttonMyInfo.Location = new System.Drawing.Point(360, 20);
            this.buttonMyInfo.Name = "buttonMyInfo";
            this.buttonMyInfo.Size = new System.Drawing.Size(75, 28);
            this.buttonMyInfo.TabIndex = 4;
            this.buttonMyInfo.Text = "내 정보";
            this.buttonMyInfo.UseVisualStyleBackColor = true;
            this.buttonMyInfo.Click += new System.EventHandler(this.buttonMyInfo_Click);
            // 
            // MemberForm
            // 
            this.ClientSize = new System.Drawing.Size(449, 330);
            this.Controls.Add(this.listViewMembers);
            this.Controls.Add(this.buttonAllMembers);
            this.Controls.Add(this.textBoxId);
            this.Controls.Add(this.buttonSearch);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonMyInfo);
            this.Name = "MemberForm";
            this.Text = "회원 관리";
            this.ResumeLayout(false);
            this.PerformLayout();

        }
    }
}
