namespace MyDiary
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
            this.dtp_Date = new System.Windows.Forms.DateTimePicker();
            this.btn_PageSetup = new System.Windows.Forms.Button();
            this.btn_Print = new System.Windows.Forms.Button();
            this.cb_Weather = new System.Windows.Forms.ComboBox();
            this.txt_Content = new System.Windows.Forms.TextBox();
            this.lbl_Content = new System.Windows.Forms.Label();
            this.txt_Title = new System.Windows.Forms.TextBox();
            this.lbl_Title = new System.Windows.Forms.Label();
            this.lbl_Weather = new System.Windows.Forms.Label();
            this.btn_Preview = new System.Windows.Forms.Button();
            this.lbl_Date = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // dtp_Date
            // 
            this.dtp_Date.Location = new System.Drawing.Point(80, 11);
            this.dtp_Date.Name = "dtp_Date";
            this.dtp_Date.Size = new System.Drawing.Size(176, 21);
            this.dtp_Date.TabIndex = 24;
            // 
            // btn_PageSetup
            // 
            this.btn_PageSetup.Location = new System.Drawing.Point(80, 283);
            this.btn_PageSetup.Name = "btn_PageSetup";
            this.btn_PageSetup.Size = new System.Drawing.Size(120, 48);
            this.btn_PageSetup.TabIndex = 23;
            this.btn_PageSetup.Text = "페이지 설정";
            this.btn_PageSetup.Click += new System.EventHandler(this.btn_PageSetup_Click);
            // 
            // btn_Print
            // 
            this.btn_Print.Location = new System.Drawing.Point(336, 283);
            this.btn_Print.Name = "btn_Print";
            this.btn_Print.Size = new System.Drawing.Size(104, 48);
            this.btn_Print.TabIndex = 22;
            this.btn_Print.Text = "프린트";
            this.btn_Print.Click += new System.EventHandler(this.btn_Print_Click);
            // 
            // cb_Weather
            // 
            this.cb_Weather.Items.AddRange(new object[] {
            "맑음",
            "흐림",
            "비옴",
            "눈옴"});
            this.cb_Weather.Location = new System.Drawing.Point(80, 51);
            this.cb_Weather.Name = "cb_Weather";
            this.cb_Weather.Size = new System.Drawing.Size(160, 20);
            this.cb_Weather.TabIndex = 21;
            this.cb_Weather.Text = "맑음";
            // 
            // txt_Content
            // 
            this.txt_Content.Location = new System.Drawing.Point(80, 123);
            this.txt_Content.Multiline = true;
            this.txt_Content.Name = "txt_Content";
            this.txt_Content.Size = new System.Drawing.Size(368, 152);
            this.txt_Content.TabIndex = 20;
            // 
            // lbl_Content
            // 
            this.lbl_Content.Location = new System.Drawing.Point(16, 131);
            this.lbl_Content.Name = "lbl_Content";
            this.lbl_Content.Size = new System.Drawing.Size(64, 23);
            this.lbl_Content.TabIndex = 19;
            this.lbl_Content.Text = "내용 입력";
            // 
            // txt_Title
            // 
            this.txt_Title.Location = new System.Drawing.Point(80, 91);
            this.txt_Title.Name = "txt_Title";
            this.txt_Title.Size = new System.Drawing.Size(368, 21);
            this.txt_Title.TabIndex = 18;
            // 
            // lbl_Title
            // 
            this.lbl_Title.Location = new System.Drawing.Point(16, 91);
            this.lbl_Title.Name = "lbl_Title";
            this.lbl_Title.Size = new System.Drawing.Size(64, 23);
            this.lbl_Title.TabIndex = 17;
            this.lbl_Title.Text = "제목 입력";
            // 
            // lbl_Weather
            // 
            this.lbl_Weather.Location = new System.Drawing.Point(16, 51);
            this.lbl_Weather.Name = "lbl_Weather";
            this.lbl_Weather.Size = new System.Drawing.Size(64, 23);
            this.lbl_Weather.TabIndex = 16;
            this.lbl_Weather.Text = "날씨 입력";
            // 
            // btn_Preview
            // 
            this.btn_Preview.Location = new System.Drawing.Point(208, 283);
            this.btn_Preview.Name = "btn_Preview";
            this.btn_Preview.Size = new System.Drawing.Size(120, 48);
            this.btn_Preview.TabIndex = 15;
            this.btn_Preview.Text = "미리보기";
            this.btn_Preview.Click += new System.EventHandler(this.btn_Preview_Click);
            // 
            // lbl_Date
            // 
            this.lbl_Date.Location = new System.Drawing.Point(16, 11);
            this.lbl_Date.Name = "lbl_Date";
            this.lbl_Date.Size = new System.Drawing.Size(64, 23);
            this.lbl_Date.TabIndex = 14;
            this.lbl_Date.Text = "날짜 입력";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(464, 342);
            this.Controls.Add(this.dtp_Date);
            this.Controls.Add(this.btn_PageSetup);
            this.Controls.Add(this.btn_Print);
            this.Controls.Add(this.cb_Weather);
            this.Controls.Add(this.txt_Content);
            this.Controls.Add(this.lbl_Content);
            this.Controls.Add(this.txt_Title);
            this.Controls.Add(this.lbl_Title);
            this.Controls.Add(this.lbl_Weather);
            this.Controls.Add(this.btn_Preview);
            this.Controls.Add(this.lbl_Date);
            this.Name = "Form1";
            this.Text = "일기장 1.0";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DateTimePicker dtp_Date;
        private System.Windows.Forms.Button btn_PageSetup;
        private System.Windows.Forms.Button btn_Print;
        private System.Windows.Forms.ComboBox cb_Weather;
        private System.Windows.Forms.TextBox txt_Content;
        private System.Windows.Forms.Label lbl_Content;
        private System.Windows.Forms.TextBox txt_Title;
        private System.Windows.Forms.Label lbl_Title;
        private System.Windows.Forms.Label lbl_Weather;
        private System.Windows.Forms.Button btn_Preview;
        private System.Windows.Forms.Label lbl_Date;
    }
}

