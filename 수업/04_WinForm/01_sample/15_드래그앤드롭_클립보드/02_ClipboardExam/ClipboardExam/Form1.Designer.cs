namespace ClipboardExam
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
            this.txt_info = new System.Windows.Forms.TextBox();
            this.btn_clipinfo = new System.Windows.Forms.Button();
            this.btn_addImage = new System.Windows.Forms.Button();
            this.btn_getImage = new System.Windows.Forms.Button();
            this.btn_addAudio = new System.Windows.Forms.Button();
            this.btn_getAudio = new System.Windows.Forms.Button();
            this.btn_getText = new System.Windows.Forms.Button();
            this.btn_addText = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // txt_info
            // 
            this.txt_info.Location = new System.Drawing.Point(10, 12);
            this.txt_info.Multiline = true;
            this.txt_info.Name = "txt_info";
            this.txt_info.Size = new System.Drawing.Size(405, 240);
            this.txt_info.TabIndex = 0;
            // 
            // btn_clipinfo
            // 
            this.btn_clipinfo.Location = new System.Drawing.Point(13, 258);
            this.btn_clipinfo.Name = "btn_clipinfo";
            this.btn_clipinfo.Size = new System.Drawing.Size(122, 88);
            this.btn_clipinfo.TabIndex = 1;
            this.btn_clipinfo.Text = "클립보드 정보";
            this.btn_clipinfo.UseVisualStyleBackColor = true;
            this.btn_clipinfo.Click += new System.EventHandler(this.btn_clipinfo_Click);
            // 
            // btn_addImage
            // 
            this.btn_addImage.Enabled = false;
            this.btn_addImage.Location = new System.Drawing.Point(146, 261);
            this.btn_addImage.Name = "btn_addImage";
            this.btn_addImage.Size = new System.Drawing.Size(87, 37);
            this.btn_addImage.TabIndex = 2;
            this.btn_addImage.Text = "이미지 추가";
            this.btn_addImage.UseVisualStyleBackColor = true;
            this.btn_addImage.Click += new System.EventHandler(this.btn_addImage_Click);
            // 
            // btn_getImage
            // 
            this.btn_getImage.Enabled = false;
            this.btn_getImage.Location = new System.Drawing.Point(146, 304);
            this.btn_getImage.Name = "btn_getImage";
            this.btn_getImage.Size = new System.Drawing.Size(87, 37);
            this.btn_getImage.TabIndex = 3;
            this.btn_getImage.Text = "이미지 추출";
            this.btn_getImage.UseVisualStyleBackColor = true;
            this.btn_getImage.Click += new System.EventHandler(this.btn_getImage_Click);
            // 
            // btn_addAudio
            // 
            this.btn_addAudio.Enabled = false;
            this.btn_addAudio.Location = new System.Drawing.Point(239, 261);
            this.btn_addAudio.Name = "btn_addAudio";
            this.btn_addAudio.Size = new System.Drawing.Size(85, 37);
            this.btn_addAudio.TabIndex = 4;
            this.btn_addAudio.Text = "오디오 추가";
            this.btn_addAudio.UseVisualStyleBackColor = true;
            this.btn_addAudio.Click += new System.EventHandler(this.btn_addAudio_Click);
            // 
            // btn_getAudio
            // 
            this.btn_getAudio.Enabled = false;
            this.btn_getAudio.Location = new System.Drawing.Point(239, 304);
            this.btn_getAudio.Name = "btn_getAudio";
            this.btn_getAudio.Size = new System.Drawing.Size(85, 37);
            this.btn_getAudio.TabIndex = 5;
            this.btn_getAudio.Text = "오디오 추출";
            this.btn_getAudio.UseVisualStyleBackColor = true;
            this.btn_getAudio.Click += new System.EventHandler(this.btn_getAudio_Click);
            // 
            // btn_getText
            // 
            this.btn_getText.Enabled = false;
            this.btn_getText.Location = new System.Drawing.Point(330, 304);
            this.btn_getText.Name = "btn_getText";
            this.btn_getText.Size = new System.Drawing.Size(85, 37);
            this.btn_getText.TabIndex = 7;
            this.btn_getText.Text = "텍스트 추출";
            this.btn_getText.UseVisualStyleBackColor = true;
            this.btn_getText.Click += new System.EventHandler(this.btn_getText_Click);
            // 
            // btn_addText
            // 
            this.btn_addText.Enabled = false;
            this.btn_addText.Location = new System.Drawing.Point(330, 261);
            this.btn_addText.Name = "btn_addText";
            this.btn_addText.Size = new System.Drawing.Size(85, 37);
            this.btn_addText.TabIndex = 6;
            this.btn_addText.Text = "텍스트 추가";
            this.btn_addText.UseVisualStyleBackColor = true;
            this.btn_addText.Click += new System.EventHandler(this.btn_addText_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(430, 358);
            this.Controls.Add(this.btn_getText);
            this.Controls.Add(this.btn_addText);
            this.Controls.Add(this.btn_getAudio);
            this.Controls.Add(this.btn_addAudio);
            this.Controls.Add(this.btn_getImage);
            this.Controls.Add(this.btn_addImage);
            this.Controls.Add(this.btn_clipinfo);
            this.Controls.Add(this.txt_info);
            this.Name = "Form1";
            this.Text = "클립보드 예제";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txt_info;
        private System.Windows.Forms.Button btn_clipinfo;
        private System.Windows.Forms.Button btn_addImage;
        private System.Windows.Forms.Button btn_getImage;
        private System.Windows.Forms.Button btn_addAudio;
        private System.Windows.Forms.Button btn_getAudio;
        private System.Windows.Forms.Button btn_getText;
        private System.Windows.Forms.Button btn_addText;
    }
}

