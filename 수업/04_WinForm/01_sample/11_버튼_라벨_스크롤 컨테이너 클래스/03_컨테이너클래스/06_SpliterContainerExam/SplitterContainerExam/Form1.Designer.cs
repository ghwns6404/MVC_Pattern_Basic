namespace SplitterContainerExam
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.btn_panel2 = new System.Windows.Forms.Button();
            this.btn_panel1 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_splitop = new System.Windows.Forms.Button();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.splitContainer1.Panel1.Controls.Add(this.btn_panel2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.splitContainer1.Panel2.Controls.Add(this.btn_panel1);
            this.splitContainer1.Panel2.Controls.Add(this.textBox1);
            this.splitContainer1.Panel2.Controls.Add(this.label1);
            this.splitContainer1.Panel2.Controls.Add(this.btn_splitop);
            this.splitContainer1.Size = new System.Drawing.Size(362, 269);
            this.splitContainer1.SplitterDistance = 135;
            this.splitContainer1.TabIndex = 0;
            // 
            // btn_panel2
            // 
            this.btn_panel2.Location = new System.Drawing.Point(3, 77);
            this.btn_panel2.Name = "btn_panel2";
            this.btn_panel2.Size = new System.Drawing.Size(122, 28);
            this.btn_panel2.TabIndex = 2;
            this.btn_panel2.Text = "Panel2 숨기기";
            this.btn_panel2.UseVisualStyleBackColor = true;
            this.btn_panel2.Click += new System.EventHandler(this.btn_panel2_Click);
            // 
            // btn_panel1
            // 
            this.btn_panel1.Location = new System.Drawing.Point(35, 105);
            this.btn_panel1.Name = "btn_panel1";
            this.btn_panel1.Size = new System.Drawing.Size(122, 28);
            this.btn_panel1.TabIndex = 1;
            this.btn_panel1.Text = "Panel1 숨기기";
            this.btn_panel1.UseVisualStyleBackColor = true;
            this.btn_panel1.Click += new System.EventHandler(this.btn_panel1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(120, 68);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(54, 21);
            this.textBox1.TabIndex = 2;
            this.textBox1.Text = "10";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(33, 77);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Splitter Width:";
            // 
            // btn_splitop
            // 
            this.btn_splitop.Location = new System.Drawing.Point(30, 23);
            this.btn_splitop.Name = "btn_splitop";
            this.btn_splitop.Size = new System.Drawing.Size(120, 26);
            this.btn_splitop.TabIndex = 0;
            this.btn_splitop.Text = "수평 스플리트";
            this.btn_splitop.UseVisualStyleBackColor = true;
            this.btn_splitop.Click += new System.EventHandler(this.btn_splitop_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(362, 269);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_splitop;
        private System.Windows.Forms.Button btn_panel2;
        private System.Windows.Forms.Button btn_panel1;
        private System.Windows.Forms.TextBox textBox1;
    }
}

