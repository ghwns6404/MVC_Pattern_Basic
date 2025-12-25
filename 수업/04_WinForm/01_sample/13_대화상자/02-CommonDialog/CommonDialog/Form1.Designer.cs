namespace CommonDialog
{
    partial class Form1
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
            this.btn_FileOpen = new System.Windows.Forms.Button();
            this.btn_FileSave = new System.Windows.Forms.Button();
            this.btn_Font = new System.Windows.Forms.Button();
            this.btn_Color = new System.Windows.Forms.Button();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.btn_FolderBrowser = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btn_FileOpen
            // 
            this.btn_FileOpen.Location = new System.Drawing.Point(486, 12);
            this.btn_FileOpen.Name = "btn_FileOpen";
            this.btn_FileOpen.Size = new System.Drawing.Size(92, 40);
            this.btn_FileOpen.TabIndex = 1;
            this.btn_FileOpen.Text = "파일 열기";
            this.btn_FileOpen.UseVisualStyleBackColor = true;
            this.btn_FileOpen.Click += new System.EventHandler(this.btn_FileOpen_Click);
            // 
            // btn_FileSave
            // 
            this.btn_FileSave.Location = new System.Drawing.Point(486, 58);
            this.btn_FileSave.Name = "btn_FileSave";
            this.btn_FileSave.Size = new System.Drawing.Size(93, 40);
            this.btn_FileSave.TabIndex = 2;
            this.btn_FileSave.Text = "파일 저장";
            this.btn_FileSave.UseVisualStyleBackColor = true;
            this.btn_FileSave.Click += new System.EventHandler(this.btn_FileSave_Click);
            // 
            // btn_Font
            // 
            this.btn_Font.Location = new System.Drawing.Point(486, 123);
            this.btn_Font.Name = "btn_Font";
            this.btn_Font.Size = new System.Drawing.Size(93, 40);
            this.btn_Font.TabIndex = 3;
            this.btn_Font.Text = "폰트 설정";
            this.btn_Font.UseVisualStyleBackColor = true;
            this.btn_Font.Click += new System.EventHandler(this.btn_Font_Click);
            // 
            // btn_Color
            // 
            this.btn_Color.Location = new System.Drawing.Point(486, 169);
            this.btn_Color.Name = "btn_Color";
            this.btn_Color.Size = new System.Drawing.Size(93, 40);
            this.btn_Color.TabIndex = 4;
            this.btn_Color.Text = "컬러 설정";
            this.btn_Color.UseVisualStyleBackColor = true;
            this.btn_Color.Click += new System.EventHandler(this.btn_Color_Click);
            // 
            // richTextBox1
            // 
            this.richTextBox1.Font = new System.Drawing.Font("굴림", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.richTextBox1.Location = new System.Drawing.Point(12, 12);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(456, 288);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // btn_FolderBrowser
            // 
            this.btn_FolderBrowser.Location = new System.Drawing.Point(487, 272);
            this.btn_FolderBrowser.Name = "btn_FolderBrowser";
            this.btn_FolderBrowser.Size = new System.Drawing.Size(92, 78);
            this.btn_FolderBrowser.TabIndex = 5;
            this.btn_FolderBrowser.Text = "폴더 브라우저";
            this.btn_FolderBrowser.UseVisualStyleBackColor = true;
            this.btn_FolderBrowser.Click += new System.EventHandler(this.btn_FolderBrowser_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(15, 321);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(453, 21);
            this.textBox1.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(590, 362);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.btn_FolderBrowser);
            this.Controls.Add(this.btn_Color);
            this.Controls.Add(this.btn_Font);
            this.Controls.Add(this.btn_FileSave);
            this.Controls.Add(this.btn_FileOpen);
            this.Controls.Add(this.richTextBox1);
            this.Name = "Form1";
            this.Text = "대화상자 예제";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_FileOpen;
        private System.Windows.Forms.Button btn_FileSave;
        private System.Windows.Forms.Button btn_Font;
        private System.Windows.Forms.Button btn_Color;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button btn_FolderBrowser;
        private System.Windows.Forms.TextBox textBox1;
    }
}

