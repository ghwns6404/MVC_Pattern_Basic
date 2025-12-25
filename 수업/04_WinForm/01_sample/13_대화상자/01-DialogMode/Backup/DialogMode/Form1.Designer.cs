namespace DialogMode
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
            this.btn_modal = new System.Windows.Forms.Button();
            this.btn_modㄷless = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btn_modal
            // 
            this.btn_modal.Location = new System.Drawing.Point(58, 39);
            this.btn_modal.Name = "btn_modal";
            this.btn_modal.Size = new System.Drawing.Size(172, 40);
            this.btn_modal.TabIndex = 0;
            this.btn_modal.Text = "모달";
            this.btn_modal.UseVisualStyleBackColor = true;
            this.btn_modal.Click += new System.EventHandler(this.btn_modal_Click);
            // 
            // btn_modㄷless
            // 
            this.btn_modㄷless.Location = new System.Drawing.Point(60, 113);
            this.btn_modㄷless.Name = "btn_modㄷless";
            this.btn_modㄷless.Size = new System.Drawing.Size(172, 40);
            this.btn_modㄷless.TabIndex = 1;
            this.btn_modㄷless.Text = "모덜리스";
            this.btn_modㄷless.UseVisualStyleBackColor = true;
            this.btn_modㄷless.Click += new System.EventHandler(this.btn_modㄷless_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 200);
            this.Controls.Add(this.btn_modㄷless);
            this.Controls.Add(this.btn_modal);
            this.Name = "Form1";
            this.Text = "모달/모달리스 예제";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btn_modal;
        private System.Windows.Forms.Button btn_modㄷless;
    }
}

