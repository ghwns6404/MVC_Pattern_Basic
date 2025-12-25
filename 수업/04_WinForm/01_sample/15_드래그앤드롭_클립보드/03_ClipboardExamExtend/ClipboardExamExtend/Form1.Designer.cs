namespace ClipboardExamExtend
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
            this.btn_getRTF = new System.Windows.Forms.Button();
            this.btn_addRTF = new System.Windows.Forms.Button();
            this.btn_getTime = new System.Windows.Forms.Button();
            this.btn_addTime = new System.Windows.Forms.Button();
            this.btn_clipboardReset = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // btn_getRTF
            // 
            this.btn_getRTF.Location = new System.Drawing.Point(320, 269);
            this.btn_getRTF.Name = "btn_getRTF";
            this.btn_getRTF.Size = new System.Drawing.Size(85, 37);
            this.btn_getRTF.TabIndex = 13;
            this.btn_getRTF.Text = "RTF 추출";
            this.btn_getRTF.UseVisualStyleBackColor = true;
            this.btn_getRTF.Click += new System.EventHandler(this.btn_getRTF_Click);
            // 
            // btn_addRTF
            // 
            this.btn_addRTF.Location = new System.Drawing.Point(320, 226);
            this.btn_addRTF.Name = "btn_addRTF";
            this.btn_addRTF.Size = new System.Drawing.Size(85, 37);
            this.btn_addRTF.TabIndex = 12;
            this.btn_addRTF.Text = "RTF 추가";
            this.btn_addRTF.UseVisualStyleBackColor = true;
            this.btn_addRTF.Click += new System.EventHandler(this.btn_addRTF_Click);
            // 
            // btn_getTime
            // 
            this.btn_getTime.Location = new System.Drawing.Point(177, 269);
            this.btn_getTime.Name = "btn_getTime";
            this.btn_getTime.Size = new System.Drawing.Size(87, 37);
            this.btn_getTime.TabIndex = 11;
            this.btn_getTime.Text = " 클립보드   시간 얻기";
            this.btn_getTime.UseVisualStyleBackColor = true;
            this.btn_getTime.Click += new System.EventHandler(this.btn_getTime_Click);
            // 
            // btn_addTime
            // 
            this.btn_addTime.Location = new System.Drawing.Point(177, 226);
            this.btn_addTime.Name = "btn_addTime";
            this.btn_addTime.Size = new System.Drawing.Size(87, 37);
            this.btn_addTime.TabIndex = 10;
            this.btn_addTime.Text = "시간 입력";
            this.btn_addTime.UseVisualStyleBackColor = true;
            this.btn_addTime.Click += new System.EventHandler(this.btn_addTime_Click);
            // 
            // btn_clipboardReset
            // 
            this.btn_clipboardReset.Location = new System.Drawing.Point(8, 226);
            this.btn_clipboardReset.Name = "btn_clipboardReset";
            this.btn_clipboardReset.Size = new System.Drawing.Size(122, 88);
            this.btn_clipboardReset.TabIndex = 9;
            this.btn_clipboardReset.Text = "클립보드 초기화";
            this.btn_clipboardReset.UseVisualStyleBackColor = true;
            this.btn_clipboardReset.Click += new System.EventHandler(this.btn_clipboardReset_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Blue;
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(393, 201);
            this.panel1.TabIndex = 16;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(421, 337);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btn_getRTF);
            this.Controls.Add(this.btn_addRTF);
            this.Controls.Add(this.btn_getTime);
            this.Controls.Add(this.btn_addTime);
            this.Controls.Add(this.btn_clipboardReset);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btn_getRTF;
        private System.Windows.Forms.Button btn_addRTF;
        private System.Windows.Forms.Button btn_getTime;
        private System.Windows.Forms.Button btn_addTime;
        private System.Windows.Forms.Button btn_clipboardReset;
        private System.Windows.Forms.Panel panel1;
    }
}

