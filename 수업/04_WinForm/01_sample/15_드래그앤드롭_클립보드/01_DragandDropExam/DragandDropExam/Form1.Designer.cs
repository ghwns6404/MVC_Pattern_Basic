namespace DragandDropExam
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
            this.txt_box = new System.Windows.Forms.TextBox();
            this.rich_txt_box = new System.Windows.Forms.RichTextBox();
            this.pic_box = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pic_box)).BeginInit();
            this.SuspendLayout();
            // 
            // txt_box
            // 
            this.txt_box.Location = new System.Drawing.Point(15, 7);
            this.txt_box.Multiline = true;
            this.txt_box.Name = "txt_box";
            this.txt_box.Size = new System.Drawing.Size(163, 324);
            this.txt_box.TabIndex = 0;
            // 
            // rich_txt_box
            // 
            this.rich_txt_box.Location = new System.Drawing.Point(190, 8);
            this.rich_txt_box.Name = "rich_txt_box";
            this.rich_txt_box.Size = new System.Drawing.Size(185, 325);
            this.rich_txt_box.TabIndex = 1;
            this.rich_txt_box.Text = "";
            // 
            // pic_box
            // 
            this.pic_box.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pic_box.Location = new System.Drawing.Point(389, 9);
            this.pic_box.Name = "pic_box";
            this.pic_box.Size = new System.Drawing.Size(159, 323);
            this.pic_box.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pic_box.TabIndex = 2;
            this.pic_box.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(563, 345);
            this.Controls.Add(this.pic_box);
            this.Controls.Add(this.rich_txt_box);
            this.Controls.Add(this.txt_box);
            this.Name = "Form1";
            this.Text = "텍스트, RTF, 이미지  드래그앤드롭";
            ((System.ComponentModel.ISupportInitialize)(this.pic_box)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txt_box;
        private System.Windows.Forms.RichTextBox rich_txt_box;
        private System.Windows.Forms.PictureBox pic_box;
    }
}

