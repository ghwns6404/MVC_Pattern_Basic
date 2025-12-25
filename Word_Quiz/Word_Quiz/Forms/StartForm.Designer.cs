namespace Word_Quiz
{
    partial class StartForm
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
            this.animalButton = new System.Windows.Forms.Button();
            this.jobButton = new System.Windows.Forms.Button();
            this.fruitButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // animalButton
            // 
            this.animalButton.Font = new System.Drawing.Font("굴림", 13F);
            this.animalButton.Location = new System.Drawing.Point(72, 75);
            this.animalButton.Name = "animalButton";
            this.animalButton.Size = new System.Drawing.Size(113, 49);
            this.animalButton.TabIndex = 0;
            this.animalButton.Text = "EASY";
            this.animalButton.UseVisualStyleBackColor = true;
            this.animalButton.Click += new System.EventHandler(this.animalButton_Click);
            // 
            // jobButton
            // 
            this.jobButton.Font = new System.Drawing.Font("굴림", 13F);
            this.jobButton.Location = new System.Drawing.Point(72, 149);
            this.jobButton.Name = "jobButton";
            this.jobButton.Size = new System.Drawing.Size(113, 49);
            this.jobButton.TabIndex = 1;
            this.jobButton.Text = "NORMAL";
            this.jobButton.UseVisualStyleBackColor = true;
            this.jobButton.Click += new System.EventHandler(this.jobButton_Click);
            // 
            // fruitButton
            // 
            this.fruitButton.Font = new System.Drawing.Font("굴림", 13F);
            this.fruitButton.Location = new System.Drawing.Point(72, 224);
            this.fruitButton.Name = "fruitButton";
            this.fruitButton.Size = new System.Drawing.Size(113, 49);
            this.fruitButton.TabIndex = 2;
            this.fruitButton.Text = "HARD";
            this.fruitButton.UseVisualStyleBackColor = true;
            this.fruitButton.Click += new System.EventHandler(this.fruitButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("굴림", 15F);
            this.label1.Location = new System.Drawing.Point(79, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(116, 20);
            this.label1.TabIndex = 4;
            this.label1.Text = "난이도 선택";
            // 
            // StartForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(278, 297);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.fruitButton);
            this.Controls.Add(this.jobButton);
            this.Controls.Add(this.animalButton);
            this.Name = "StartForm";
            this.Text = "StartForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button animalButton;
        private System.Windows.Forms.Button jobButton;
        private System.Windows.Forms.Button fruitButton;
        private System.Windows.Forms.Label label1;
    }
}