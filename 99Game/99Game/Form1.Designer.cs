namespace _99Game
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
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.currentA = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.currentB = new System.Windows.Forms.TextBox();
            this.levelBox = new System.Windows.Forms.TextBox();
            this.scoreBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.answerBox = new System.Windows.Forms.TextBox();
            this.start = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.lefttimeBox = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // currentA
            // 
            this.currentA.Font = new System.Drawing.Font("굴림", 15F);
            this.currentA.Location = new System.Drawing.Point(41, 119);
            this.currentA.Name = "currentA";
            this.currentA.ReadOnly = true;
            this.currentA.Size = new System.Drawing.Size(106, 30);
            this.currentA.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("굴림", 15F);
            this.label1.Location = new System.Drawing.Point(153, 123);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(22, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "X";
            // 
            // currentB
            // 
            this.currentB.Font = new System.Drawing.Font("굴림", 15F);
            this.currentB.Location = new System.Drawing.Point(181, 120);
            this.currentB.Name = "currentB";
            this.currentB.ReadOnly = true;
            this.currentB.Size = new System.Drawing.Size(106, 30);
            this.currentB.TabIndex = 2;
            // 
            // levelBox
            // 
            this.levelBox.Location = new System.Drawing.Point(65, 20);
            this.levelBox.Name = "levelBox";
            this.levelBox.ReadOnly = true;
            this.levelBox.Size = new System.Drawing.Size(52, 21);
            this.levelBox.TabIndex = 3;
            // 
            // scoreBox
            // 
            this.scoreBox.Location = new System.Drawing.Point(65, 57);
            this.scoreBox.Name = "scoreBox";
            this.scoreBox.ReadOnly = true;
            this.scoreBox.Size = new System.Drawing.Size(52, 21);
            this.scoreBox.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "단계";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 61);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "점수";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("굴림", 15F);
            this.label4.Location = new System.Drawing.Point(293, 123);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(20, 20);
            this.label4.TabIndex = 7;
            this.label4.Text = "=";
            // 
            // answerBox
            // 
            this.answerBox.Font = new System.Drawing.Font("굴림", 15F);
            this.answerBox.Location = new System.Drawing.Point(319, 119);
            this.answerBox.Name = "answerBox";
            this.answerBox.Size = new System.Drawing.Size(106, 30);
            this.answerBox.TabIndex = 8;
            this.answerBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.answer_KeyDown);
            // 
            // start
            // 
            this.start.Location = new System.Drawing.Point(181, 12);
            this.start.Name = "start";
            this.start.Size = new System.Drawing.Size(117, 35);
            this.start.TabIndex = 9;
            this.start.Text = "게임 시작";
            this.start.UseVisualStyleBackColor = true;
            this.start.Click += new System.EventHandler(this.start_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(329, 17);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 10;
            this.label5.Text = "남은시간";
            // 
            // lefttimeBox
            // 
            this.lefttimeBox.Location = new System.Drawing.Point(388, 14);
            this.lefttimeBox.Name = "lefttimeBox";
            this.lefttimeBox.ReadOnly = true;
            this.lefttimeBox.Size = new System.Drawing.Size(57, 21);
            this.lefttimeBox.TabIndex = 11;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lefttimeBox);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.answerBox);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.scoreBox);
            this.groupBox1.Controls.Add(this.levelBox);
            this.groupBox1.Controls.Add(this.currentB);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.currentA);
            this.groupBox1.Location = new System.Drawing.Point(14, 53);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(451, 183);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "게임창";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 254);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.start);
            this.Name = "Form1";
            this.Text = "99Game";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox currentA;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox currentB;
        private System.Windows.Forms.TextBox levelBox;
        private System.Windows.Forms.TextBox scoreBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox answerBox;
        private System.Windows.Forms.Button start;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox lefttimeBox;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}

