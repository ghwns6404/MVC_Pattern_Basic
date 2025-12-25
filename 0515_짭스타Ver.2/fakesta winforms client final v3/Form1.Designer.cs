namespace FakeStar
{
    partial class Form1
    {
        public System.ComponentModel.IContainer components = null;
        public System.Windows.Forms.Button buttonLogin;
        public System.Windows.Forms.Button buttonSignUp;
        public System.Windows.Forms.Button buttonUpload;
        public System.Windows.Forms.Button buttonLogout;
        public System.Windows.Forms.DataGridView dataGridView1;
        public System.Windows.Forms.DataGridViewImageColumn colImage;
        public System.Windows.Forms.DataGridViewTextBoxColumn colTitle;
        public System.Windows.Forms.Button buttonMemberManage;
        public System.Windows.Forms.Button buttonPostManage;


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
            this.buttonLogin = new System.Windows.Forms.Button();
            this.buttonSignUp = new System.Windows.Forms.Button();
            this.buttonUpload = new System.Windows.Forms.Button();
            this.buttonLogout = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.colImage = new System.Windows.Forms.DataGridViewImageColumn();
            this.colTitle = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.buttonMemberManage = new System.Windows.Forms.Button();
            this.buttonPostManage = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonLogin
            // 
            this.buttonLogin.Location = new System.Drawing.Point(26, 14);
            this.buttonLogin.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonLogin.Name = "buttonLogin";
            this.buttonLogin.Size = new System.Drawing.Size(79, 21);
            this.buttonLogin.TabIndex = 0;
            this.buttonLogin.Text = "로그인";
            this.buttonLogin.UseVisualStyleBackColor = true;
            this.buttonLogin.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonSignUp
            // 
            this.buttonSignUp.Location = new System.Drawing.Point(122, 14);
            this.buttonSignUp.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonSignUp.Name = "buttonSignUp";
            this.buttonSignUp.Size = new System.Drawing.Size(79, 21);
            this.buttonSignUp.TabIndex = 1;
            this.buttonSignUp.Text = "회원가입";
            this.buttonSignUp.UseVisualStyleBackColor = true;
            this.buttonSignUp.Click += new System.EventHandler(this.button2_Click);
            // 
            // buttonUpload
            // 
            this.buttonUpload.Location = new System.Drawing.Point(219, 14);
            this.buttonUpload.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonUpload.Name = "buttonUpload";
            this.buttonUpload.Size = new System.Drawing.Size(79, 21);
            this.buttonUpload.TabIndex = 2;
            this.buttonUpload.Text = "업로드";
            this.buttonUpload.UseVisualStyleBackColor = true;
            this.buttonUpload.Visible = false;
            this.buttonUpload.Click += new System.EventHandler(this.button3_Click);
            // 
            // buttonLogout
            // 
            this.buttonLogout.Location = new System.Drawing.Point(411, 14);
            this.buttonLogout.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonLogout.Name = "buttonLogout";
            this.buttonLogout.Size = new System.Drawing.Size(79, 21);
            this.buttonLogout.TabIndex = 4;
            this.buttonLogout.Text = "로그아웃";
            this.buttonLogout.UseVisualStyleBackColor = true;
            this.buttonLogout.Visible = false;
            this.buttonLogout.Click += new System.EventHandler(this.button5_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.colImage,
            this.colTitle});
            this.dataGridView1.Location = new System.Drawing.Point(26, 50);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersWidth = 51;
            this.dataGridView1.RowTemplate.Height = 80;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView1.Size = new System.Drawing.Size(464, 212);
            this.dataGridView1.TabIndex = 5;
            this.dataGridView1.CellDoubleClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellDoubleClick);
            // 
            // colImage
            // 
            this.colImage.HeaderText = "사진";
            this.colImage.ImageLayout = System.Windows.Forms.DataGridViewImageCellLayout.Zoom;
            this.colImage.MinimumWidth = 6;
            this.colImage.Name = "colImage";
            this.colImage.ReadOnly = true;
            this.colImage.Width = 120;
            // 
            // colTitle
            // 
            this.colTitle.HeaderText = "제목";
            this.colTitle.MinimumWidth = 6;
            this.colTitle.Name = "colTitle";
            this.colTitle.ReadOnly = true;
            this.colTitle.Width = 350;
            // 
            // buttonMemberManage
            // 
            this.buttonMemberManage.Location = new System.Drawing.Point(505, 50);
            this.buttonMemberManage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonMemberManage.Name = "buttonMemberManage";
            this.buttonMemberManage.Size = new System.Drawing.Size(88, 24);
            this.buttonMemberManage.TabIndex = 6;
            this.buttonMemberManage.Text = "회원관리";
            this.buttonMemberManage.UseVisualStyleBackColor = true;
            this.buttonMemberManage.Click += new System.EventHandler(this.buttonMemberManage_Click);
            // 
            // buttonPostManage
            // 
            this.buttonPostManage.Location = new System.Drawing.Point(505, 78);
            this.buttonPostManage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonPostManage.Name = "buttonPostManage";
            this.buttonPostManage.Size = new System.Drawing.Size(88, 24);
            this.buttonPostManage.TabIndex = 7;
            this.buttonPostManage.Text = "게시물관리";
            this.buttonPostManage.UseVisualStyleBackColor = true;
            this.buttonPostManage.Click += new System.EventHandler(this.buttonPostManage_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(605, 282);
            this.Controls.Add(this.buttonPostManage);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.buttonLogout);
            this.Controls.Add(this.buttonUpload);
            this.Controls.Add(this.buttonSignUp);
            this.Controls.Add(this.buttonLogin);
            this.Controls.Add(this.buttonMemberManage);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "FakeStar";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }
    }
}
