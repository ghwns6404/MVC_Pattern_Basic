namespace FakeStar
{
    partial class ImageViewerForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.PictureBox pictureBoxLarge;
        private System.Windows.Forms.Button buttonPrev;
        private System.Windows.Forms.Button buttonNext;
        private System.Windows.Forms.Label labelIndex;
        private System.Windows.Forms.TextBox textBoxDescription;
        private System.Windows.Forms.Button buttonSaveDesc;
        private System.Windows.Forms.Button buttonLike;
        private System.Windows.Forms.Label labelLikes;
        private System.Windows.Forms.ListBox listBoxComments;
        private System.Windows.Forms.TextBox textBoxComment;
        private System.Windows.Forms.Button buttonAddComment;
        private System.Windows.Forms.Label labelDesc;
        private System.Windows.Forms.Label labelComment;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.pictureBoxLarge = new System.Windows.Forms.PictureBox();
            this.buttonPrev = new System.Windows.Forms.Button();
            this.buttonNext = new System.Windows.Forms.Button();
            this.labelIndex = new System.Windows.Forms.Label();
            this.textBoxDescription = new System.Windows.Forms.TextBox();
            this.buttonSaveDesc = new System.Windows.Forms.Button();
            this.buttonLike = new System.Windows.Forms.Button();
            this.labelLikes = new System.Windows.Forms.Label();
            this.listBoxComments = new System.Windows.Forms.ListBox();
            this.textBoxComment = new System.Windows.Forms.TextBox();
            this.buttonAddComment = new System.Windows.Forms.Button();
            this.labelDesc = new System.Windows.Forms.Label();
            this.labelComment = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLarge)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxLarge
            // 
            this.pictureBoxLarge.Location = new System.Drawing.Point(30, 20);
            this.pictureBoxLarge.Name = "pictureBoxLarge";
            this.pictureBoxLarge.Size = new System.Drawing.Size(400, 250);
            this.pictureBoxLarge.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBoxLarge.TabIndex = 0;
            this.pictureBoxLarge.TabStop = false;
            this.pictureBoxLarge.Click += new System.EventHandler(this.pictureBoxLarge_Click);
            // 
            // buttonPrev
            // 
            this.buttonPrev.Location = new System.Drawing.Point(30, 280);
            this.buttonPrev.Name = "buttonPrev";
            this.buttonPrev.Size = new System.Drawing.Size(80, 28);
            this.buttonPrev.TabIndex = 1;
            this.buttonPrev.Text = "<";
            this.buttonPrev.UseVisualStyleBackColor = true;
            this.buttonPrev.Click += new System.EventHandler(this.buttonPrev_Click);
            // 
            // buttonNext
            // 
            this.buttonNext.Location = new System.Drawing.Point(350, 280);
            this.buttonNext.Name = "buttonNext";
            this.buttonNext.Size = new System.Drawing.Size(80, 28);
            this.buttonNext.TabIndex = 2;
            this.buttonNext.Text = ">";
            this.buttonNext.UseVisualStyleBackColor = true;
            this.buttonNext.Click += new System.EventHandler(this.buttonNext_Click);
            // 
            // labelIndex
            // 
            this.labelIndex.Location = new System.Drawing.Point(180, 285);
            this.labelIndex.Name = "labelIndex";
            this.labelIndex.Size = new System.Drawing.Size(100, 18);
            this.labelIndex.TabIndex = 3;
            this.labelIndex.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelDesc
            // 
            this.labelDesc.Location = new System.Drawing.Point(30, 320);
            this.labelDesc.Name = "labelDesc";
            this.labelDesc.Size = new System.Drawing.Size(60, 18);
            this.labelDesc.Text = "설명";
            // 
            // textBoxDescription
            // 
            this.textBoxDescription.Location = new System.Drawing.Point(90, 317);
            this.textBoxDescription.Name = "textBoxDescription";
            this.textBoxDescription.Size = new System.Drawing.Size(220, 25);
            this.textBoxDescription.TabIndex = 4;
            // 
            // buttonSaveDesc
            // 
            this.buttonSaveDesc.Location = new System.Drawing.Point(320, 315);
            this.buttonSaveDesc.Name = "buttonSaveDesc";
            this.buttonSaveDesc.Size = new System.Drawing.Size(50, 28);
            this.buttonSaveDesc.TabIndex = 5;
            this.buttonSaveDesc.Text = "저장";
            this.buttonSaveDesc.UseVisualStyleBackColor = true;
            this.buttonSaveDesc.Click += new System.EventHandler(this.buttonSaveDesc_Click);
            // 
            // buttonLike
            // 
            this.buttonLike.Location = new System.Drawing.Point(380, 315);
            this.buttonLike.Name = "buttonLike";
            this.buttonLike.Size = new System.Drawing.Size(50, 28);
            this.buttonLike.TabIndex = 6;
            this.buttonLike.Text = "♥";
            this.buttonLike.UseVisualStyleBackColor = true;
            this.buttonLike.Click += new System.EventHandler(this.buttonLike_Click);
            // 
            // labelLikes
            // 
            this.labelLikes.Location = new System.Drawing.Point(30, 350);
            this.labelLikes.Name = "labelLikes";
            this.labelLikes.Size = new System.Drawing.Size(120, 18);
            this.labelLikes.TabIndex = 7;
            this.labelLikes.Text = "좋아요: 0";
            // 
            // labelComment
            // 
            this.labelComment.Location = new System.Drawing.Point(30, 380);
            this.labelComment.Name = "labelComment";
            this.labelComment.Size = new System.Drawing.Size(60, 18);
            this.labelComment.Text = "댓글";
            // 
            // listBoxComments
            // 
            this.listBoxComments.FormattingEnabled = true;
            this.listBoxComments.ItemHeight = 15;
            this.listBoxComments.Location = new System.Drawing.Point(30, 400);
            this.listBoxComments.Name = "listBoxComments";
            this.listBoxComments.Size = new System.Drawing.Size(400, 64);
            this.listBoxComments.TabIndex = 8;
            // 
            // textBoxComment
            // 
            this.textBoxComment.Location = new System.Drawing.Point(30, 470);
            this.textBoxComment.Name = "textBoxComment";
            this.textBoxComment.Size = new System.Drawing.Size(320, 25);
            this.textBoxComment.TabIndex = 9;
            // 
            // buttonAddComment
            // 
            this.buttonAddComment.Location = new System.Drawing.Point(360, 468);
            this.buttonAddComment.Name = "buttonAddComment";
            this.buttonAddComment.Size = new System.Drawing.Size(70, 28);
            this.buttonAddComment.TabIndex = 10;
            this.buttonAddComment.Text = "댓글달기";
            this.buttonAddComment.UseVisualStyleBackColor = true;
            this.buttonAddComment.Click += new System.EventHandler(this.buttonAddComment_Click);
            // 
            // ImageViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(460, 520);
            this.Controls.Add(this.pictureBoxLarge);
            this.Controls.Add(this.buttonPrev);
            this.Controls.Add(this.buttonNext);
            this.Controls.Add(this.labelIndex);
            this.Controls.Add(this.labelDesc);
            this.Controls.Add(this.textBoxDescription);
            this.Controls.Add(this.buttonSaveDesc);
            this.Controls.Add(this.buttonLike);
            this.Controls.Add(this.labelLikes);
            this.Controls.Add(this.labelComment);
            this.Controls.Add(this.listBoxComments);
            this.Controls.Add(this.textBoxComment);
            this.Controls.Add(this.buttonAddComment);
            this.Name = "ImageViewerForm";
            this.Text = "이미지 보기";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLarge)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
