namespace Word_Quiz.Forms
{
    partial class GameForm
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
            this.qBox1 = new System.Windows.Forms.TextBox();
            this.qBox2 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.qBox3 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.qBox4 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.answer_Box = new System.Windows.Forms.TextBox();
            this.answer_label = new System.Windows.Forms.Label();
            this.meanBox = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.levelBox = new System.Windows.Forms.TextBox();
            this.scoreBox = new System.Windows.Forms.TextBox();
            this.lefttimeBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("굴림", 20F);
            this.label1.Location = new System.Drawing.Point(42, 136);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 27);
            this.label1.TabIndex = 0;
            this.label1.Text = "1. ";
            // 
            // qBox1
            // 
            this.qBox1.Font = new System.Drawing.Font("굴림", 20F);
            this.qBox1.Location = new System.Drawing.Point(85, 131);
            this.qBox1.Name = "qBox1";
            this.qBox1.ReadOnly = true;
            this.qBox1.Size = new System.Drawing.Size(196, 38);
            this.qBox1.TabIndex = 1;
            // 
            // qBox2
            // 
            this.qBox2.Font = new System.Drawing.Font("굴림", 20F);
            this.qBox2.Location = new System.Drawing.Point(85, 187);
            this.qBox2.Name = "qBox2";
            this.qBox2.ReadOnly = true;
            this.qBox2.Size = new System.Drawing.Size(196, 38);
            this.qBox2.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("굴림", 20F);
            this.label2.Location = new System.Drawing.Point(42, 190);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 27);
            this.label2.TabIndex = 2;
            this.label2.Text = "2.";
            // 
            // qBox3
            // 
            this.qBox3.Font = new System.Drawing.Font("굴림", 20F);
            this.qBox3.Location = new System.Drawing.Point(85, 241);
            this.qBox3.Name = "qBox3";
            this.qBox3.ReadOnly = true;
            this.qBox3.Size = new System.Drawing.Size(196, 38);
            this.qBox3.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("굴림", 20F);
            this.label3.Location = new System.Drawing.Point(42, 244);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 27);
            this.label3.TabIndex = 4;
            this.label3.Text = "3.";
            // 
            // qBox4
            // 
            this.qBox4.Font = new System.Drawing.Font("굴림", 20F);
            this.qBox4.Location = new System.Drawing.Point(85, 298);
            this.qBox4.Name = "qBox4";
            this.qBox4.ReadOnly = true;
            this.qBox4.Size = new System.Drawing.Size(196, 38);
            this.qBox4.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("굴림", 20F);
            this.label4.Location = new System.Drawing.Point(42, 301);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 27);
            this.label4.TabIndex = 6;
            this.label4.Text = "4.";
            // 
            // answer_Box
            // 
            this.answer_Box.Font = new System.Drawing.Font("굴림", 15F);
            this.answer_Box.Location = new System.Drawing.Point(136, 375);
            this.answer_Box.Name = "answer_Box";
            this.answer_Box.Size = new System.Drawing.Size(78, 30);
            this.answer_Box.TabIndex = 9;
            this.answer_Box.KeyDown += new System.Windows.Forms.KeyEventHandler(this.answer_Box_KeyDown);
            // 
            // answer_label
            // 
            this.answer_label.AutoSize = true;
            this.answer_label.Font = new System.Drawing.Font("굴림", 15F);
            this.answer_label.Location = new System.Drawing.Point(81, 378);
            this.answer_label.Name = "answer_label";
            this.answer_label.Size = new System.Drawing.Size(49, 20);
            this.answer_label.TabIndex = 8;
            this.answer_label.Text = "정답";
            // 
            // meanBox
            // 
            this.meanBox.Font = new System.Drawing.Font("굴림", 20F);
            this.meanBox.Location = new System.Drawing.Point(50, 50);
            this.meanBox.Name = "meanBox";
            this.meanBox.ReadOnly = true;
            this.meanBox.Size = new System.Drawing.Size(231, 38);
            this.meanBox.TabIndex = 10;
            this.meanBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("굴림", 13F);
            this.label5.Location = new System.Drawing.Point(327, 175);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 18);
            this.label5.TabIndex = 11;
            this.label5.Text = "단계";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("굴림", 13F);
            this.label6.Location = new System.Drawing.Point(327, 226);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(44, 18);
            this.label6.TabIndex = 12;
            this.label6.Text = "점수";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("굴림", 13F);
            this.label7.Location = new System.Drawing.Point(327, 281);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(80, 18);
            this.label7.TabIndex = 13;
            this.label7.Text = "남은시간";
            // 
            // levelBox
            // 
            this.levelBox.Font = new System.Drawing.Font("굴림", 13F);
            this.levelBox.Location = new System.Drawing.Point(417, 172);
            this.levelBox.Name = "levelBox";
            this.levelBox.ReadOnly = true;
            this.levelBox.Size = new System.Drawing.Size(100, 27);
            this.levelBox.TabIndex = 14;
            // 
            // scoreBox
            // 
            this.scoreBox.Font = new System.Drawing.Font("굴림", 13F);
            this.scoreBox.Location = new System.Drawing.Point(417, 223);
            this.scoreBox.Name = "scoreBox";
            this.scoreBox.ReadOnly = true;
            this.scoreBox.Size = new System.Drawing.Size(100, 27);
            this.scoreBox.TabIndex = 15;
            // 
            // lefttimeBox
            // 
            this.lefttimeBox.Font = new System.Drawing.Font("굴림", 13F);
            this.lefttimeBox.Location = new System.Drawing.Point(417, 279);
            this.lefttimeBox.Name = "lefttimeBox";
            this.lefttimeBox.ReadOnly = true;
            this.lefttimeBox.Size = new System.Drawing.Size(100, 27);
            this.lefttimeBox.TabIndex = 16;
            // 
            // GameForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(550, 450);
            this.Controls.Add(this.lefttimeBox);
            this.Controls.Add(this.scoreBox);
            this.Controls.Add(this.levelBox);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.meanBox);
            this.Controls.Add(this.answer_Box);
            this.Controls.Add(this.answer_label);
            this.Controls.Add(this.qBox4);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.qBox3);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.qBox2);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.qBox1);
            this.Controls.Add(this.label1);
            this.Name = "GameForm";
            this.Text = "GameForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.GameForm_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox qBox1;
        private System.Windows.Forms.TextBox qBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox qBox3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox qBox4;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox answer_Box;
        private System.Windows.Forms.Label answer_label;
        private System.Windows.Forms.TextBox meanBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox levelBox;
        private System.Windows.Forms.TextBox scoreBox;
        private System.Windows.Forms.TextBox lefttimeBox;
    }
}