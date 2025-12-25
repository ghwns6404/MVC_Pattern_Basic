namespace FakeStar
{
    partial class MyInfoForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.Label labelId;
        private System.Windows.Forms.Label labelEmail;
        private System.Windows.Forms.Label labelName;
        private System.Windows.Forms.Label labelPw;
        private System.Windows.Forms.Label labelNewPw;
        private System.Windows.Forms.TextBox textBoxId;
        private System.Windows.Forms.TextBox textBoxEmail;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.TextBox textBoxPw;
        private System.Windows.Forms.TextBox textBoxNewPw;
        private System.Windows.Forms.Button buttonChangePw;
        private System.Windows.Forms.Button buttonDelete;

        // 세션 정보용 컨트롤
        private System.Windows.Forms.Label labelSessionId;
        private System.Windows.Forms.Label labelMemId;
        private System.Windows.Forms.Label labelSessionCreated;
        private System.Windows.Forms.Label labelSessionUpdated;
        private System.Windows.Forms.TextBox textBoxSessionId;
        private System.Windows.Forms.TextBox textBoxMemId;
        private System.Windows.Forms.TextBox textBoxSessionCreated;
        private System.Windows.Forms.TextBox textBoxSessionUpdated;

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
            this.labelId = new System.Windows.Forms.Label();
            this.labelEmail = new System.Windows.Forms.Label();
            this.labelName = new System.Windows.Forms.Label();
            this.labelPw = new System.Windows.Forms.Label();
            this.labelNewPw = new System.Windows.Forms.Label();
            this.textBoxId = new System.Windows.Forms.TextBox();
            this.textBoxEmail = new System.Windows.Forms.TextBox();
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.textBoxPw = new System.Windows.Forms.TextBox();
            this.textBoxNewPw = new System.Windows.Forms.TextBox();
            this.buttonChangePw = new System.Windows.Forms.Button();
            this.buttonDelete = new System.Windows.Forms.Button();
            this.labelSessionId = new System.Windows.Forms.Label();
            this.labelMemId = new System.Windows.Forms.Label();
            this.labelSessionCreated = new System.Windows.Forms.Label();
            this.labelSessionUpdated = new System.Windows.Forms.Label();
            this.textBoxSessionId = new System.Windows.Forms.TextBox();
            this.textBoxMemId = new System.Windows.Forms.TextBox();
            this.textBoxSessionCreated = new System.Windows.Forms.TextBox();
            this.textBoxSessionUpdated = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // labelId
            // 
            this.labelId.Location = new System.Drawing.Point(20, 20);
            this.labelId.Size = new System.Drawing.Size(80, 20);
            this.labelId.Text = "ID";
            // 
            // labelEmail
            // 
            this.labelEmail.Location = new System.Drawing.Point(20, 55);
            this.labelEmail.Size = new System.Drawing.Size(80, 20);
            this.labelEmail.Text = "이메일";
            // 
            // labelName
            // 
            this.labelName.Location = new System.Drawing.Point(20, 90);
            this.labelName.Size = new System.Drawing.Size(80, 20);
            this.labelName.Text = "이름";
            // 
            // labelPw
            // 
            this.labelPw.Location = new System.Drawing.Point(20, 125);
            this.labelPw.Size = new System.Drawing.Size(80, 20);
            this.labelPw.Text = "비밀번호";
            // 
            // labelNewPw
            // 
            this.labelNewPw.Location = new System.Drawing.Point(20, 160);
            this.labelNewPw.Size = new System.Drawing.Size(80, 20);
            this.labelNewPw.Text = "새 비밀번호";
            // 
            // textBoxId
            // 
            this.textBoxId.Location = new System.Drawing.Point(110, 20);
            this.textBoxId.Size = new System.Drawing.Size(180, 21);
            this.textBoxId.ReadOnly = true;
            // 
            // textBoxEmail
            // 
            this.textBoxEmail.Location = new System.Drawing.Point(110, 55);
            this.textBoxEmail.Size = new System.Drawing.Size(180, 21);
            this.textBoxEmail.ReadOnly = true;
            // 
            // textBoxName
            // 
            this.textBoxName.Location = new System.Drawing.Point(110, 90);
            this.textBoxName.Size = new System.Drawing.Size(180, 21);
            this.textBoxName.ReadOnly = true;
            // 
            // textBoxPw
            // 
            this.textBoxPw.Location = new System.Drawing.Point(110, 125);
            this.textBoxPw.Size = new System.Drawing.Size(180, 21);
            this.textBoxPw.PasswordChar = '*';
            // 
            // textBoxNewPw
            // 
            this.textBoxNewPw.Location = new System.Drawing.Point(110, 160);
            this.textBoxNewPw.Size = new System.Drawing.Size(180, 21);
            this.textBoxNewPw.PasswordChar = '*';
            // 
            // buttonChangePw
            // 
            this.buttonChangePw.Location = new System.Drawing.Point(110, 200);
            this.buttonChangePw.Size = new System.Drawing.Size(120, 28);
            this.buttonChangePw.Text = "비밀번호 변경";
            this.buttonChangePw.UseVisualStyleBackColor = true;
            this.buttonChangePw.Click += new System.EventHandler(this.buttonChangePw_Click);
            // 
            // buttonDelete
            // 
            this.buttonDelete.Location = new System.Drawing.Point(110, 240);
            this.buttonDelete.Size = new System.Drawing.Size(120, 28);
            this.buttonDelete.Text = "회원탈퇴";
            this.buttonDelete.UseVisualStyleBackColor = true;
            this.buttonDelete.Click += new System.EventHandler(this.buttonDelete_Click);
            // 
            // labelSessionId
            // 
            this.labelSessionId.Location = new System.Drawing.Point(20, 280);
            this.labelSessionId.Size = new System.Drawing.Size(80, 20);
            this.labelSessionId.Text = "세션ID";
            // 
            // labelMemId
            // 
            this.labelMemId.Location = new System.Drawing.Point(20, 315);
            this.labelMemId.Size = new System.Drawing.Size(80, 20);
            this.labelMemId.Text = "멤버ID";
            // 
            // labelSessionCreated
            // 
            this.labelSessionCreated.Location = new System.Drawing.Point(20, 350);
            this.labelSessionCreated.Size = new System.Drawing.Size(80, 20);
            this.labelSessionCreated.Text = "세션생성일";
            // 
            // labelSessionUpdated
            // 
            this.labelSessionUpdated.Location = new System.Drawing.Point(20, 385);
            this.labelSessionUpdated.Size = new System.Drawing.Size(80, 20);
            this.labelSessionUpdated.Text = "세션수정일";
            // 
            // textBoxSessionId
            // 
            this.textBoxSessionId.Location = new System.Drawing.Point(110, 280);
            this.textBoxSessionId.Size = new System.Drawing.Size(180, 21);
            this.textBoxSessionId.ReadOnly = true;
            // 
            // textBoxMemId
            // 
            this.textBoxMemId.Location = new System.Drawing.Point(110, 315);
            this.textBoxMemId.Size = new System.Drawing.Size(180, 21);
            this.textBoxMemId.ReadOnly = true;
            // 
            // textBoxSessionCreated
            // 
            this.textBoxSessionCreated.Location = new System.Drawing.Point(110, 350);
            this.textBoxSessionCreated.Size = new System.Drawing.Size(180, 21);
            this.textBoxSessionCreated.ReadOnly = true;
            // 
            // textBoxSessionUpdated
            // 
            this.textBoxSessionUpdated.Location = new System.Drawing.Point(110, 385);
            this.textBoxSessionUpdated.Size = new System.Drawing.Size(180, 21);
            this.textBoxSessionUpdated.ReadOnly = true;
            // 
            // MyInfoForm
            // 
            this.ClientSize = new System.Drawing.Size(320, 430);
            this.Controls.Add(this.labelId);
            this.Controls.Add(this.labelEmail);
            this.Controls.Add(this.labelName);
            this.Controls.Add(this.labelPw);
            this.Controls.Add(this.labelNewPw);
            this.Controls.Add(this.textBoxId);
            this.Controls.Add(this.textBoxEmail);
            this.Controls.Add(this.textBoxName);
            this.Controls.Add(this.textBoxPw);
            this.Controls.Add(this.textBoxNewPw);
            this.Controls.Add(this.buttonChangePw);
            this.Controls.Add(this.buttonDelete);
            this.Controls.Add(this.labelSessionId);
            this.Controls.Add(this.labelMemId);
            this.Controls.Add(this.labelSessionCreated);
            this.Controls.Add(this.labelSessionUpdated);
            this.Controls.Add(this.textBoxSessionId);
            this.Controls.Add(this.textBoxMemId);
            this.Controls.Add(this.textBoxSessionCreated);
            this.Controls.Add(this.textBoxSessionUpdated);
            this.Name = "MyInfoForm";
            this.Text = "내 정보";
            this.Load += new System.EventHandler(this.MyInfoForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}

