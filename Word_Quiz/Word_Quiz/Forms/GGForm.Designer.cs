namespace Word_Quiz
{
    partial class GGForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.levelBox = new System.Windows.Forms.TextBox();
            this.oBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.xBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.gradeBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.jumsuBox = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("굴림", 25F);
            this.label1.Location = new System.Drawing.Point(94, 47);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(181, 34);
            this.label1.TabIndex = 0;
            this.label1.Text = "-성 적 표-";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("굴림", 15F);
            this.label2.Location = new System.Drawing.Point(13, 134);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(116, 20);
            this.label2.TabIndex = 1;
            this.label2.Text = "선택 난이도";
            // 
            // levelBox
            // 
            this.levelBox.Font = new System.Drawing.Font("굴림", 15F);
            this.levelBox.Location = new System.Drawing.Point(142, 131);
            this.levelBox.Name = "levelBox";
            this.levelBox.ReadOnly = true;
            this.levelBox.Size = new System.Drawing.Size(161, 30);
            this.levelBox.TabIndex = 2;
            // 
            // oBox
            // 
            this.oBox.Font = new System.Drawing.Font("굴림", 15F);
            this.oBox.Location = new System.Drawing.Point(142, 197);
            this.oBox.Name = "oBox";
            this.oBox.ReadOnly = true;
            this.oBox.Size = new System.Drawing.Size(161, 30);
            this.oBox.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("굴림", 15F);
            this.label3.Location = new System.Drawing.Point(30, 200);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(96, 20);
            this.label3.TabIndex = 3;
            this.label3.Text = "맞은 갯수";
            // 
            // xBox
            // 
            this.xBox.Font = new System.Drawing.Font("굴림", 15F);
            this.xBox.Location = new System.Drawing.Point(142, 255);
            this.xBox.Name = "xBox";
            this.xBox.ReadOnly = true;
            this.xBox.Size = new System.Drawing.Size(161, 30);
            this.xBox.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("굴림", 15F);
            this.label4.Location = new System.Drawing.Point(30, 258);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(96, 20);
            this.label4.TabIndex = 5;
            this.label4.Text = "틀린 갯수";
            // 
            // gradeBox
            // 
            this.gradeBox.Font = new System.Drawing.Font("굴림", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.gradeBox.Location = new System.Drawing.Point(156, 385);
            this.gradeBox.Name = "gradeBox";
            this.gradeBox.ReadOnly = true;
            this.gradeBox.Size = new System.Drawing.Size(100, 30);
            this.gradeBox.TabIndex = 10;
            this.gradeBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("굴림", 15F);
            this.label6.Location = new System.Drawing.Point(94, 388);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(49, 20);
            this.label6.TabIndex = 9;
            this.label6.Text = "등급";
            // 
            // jumsuBox
            // 
            this.jumsuBox.Font = new System.Drawing.Font("굴림", 15F);
            this.jumsuBox.Location = new System.Drawing.Point(142, 316);
            this.jumsuBox.Name = "jumsuBox";
            this.jumsuBox.ReadOnly = true;
            this.jumsuBox.Size = new System.Drawing.Size(161, 30);
            this.jumsuBox.TabIndex = 12;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("굴림", 15F);
            this.label5.Location = new System.Drawing.Point(50, 319);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(76, 20);
            this.label5.TabIndex = 11;
            this.label5.Text = "총 점수";
            // 
            // GGForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(349, 437);
            this.Controls.Add(this.jumsuBox);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.gradeBox);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.xBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.oBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.levelBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "GGForm";
            this.Text = "ReportForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.GGForm_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox levelBox;
        private System.Windows.Forms.TextBox oBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox xBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox gradeBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox jumsuBox;
        private System.Windows.Forms.Label label5;
    }
}