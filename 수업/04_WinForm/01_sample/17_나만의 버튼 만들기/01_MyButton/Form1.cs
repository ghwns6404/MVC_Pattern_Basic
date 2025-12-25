using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace MyButton
{
	/// <summary>
	/// Form1에 대한 요약 설명입니다.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
        private MyButton.RainbowButton button1;
		private MyButton.RainbowButton2 button2;
		private MyButton.RainbowButton3 button3;
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Windows Form 디자이너 지원에 필요합니다.
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent를 호출한 다음 생성자 코드를 추가합니다.
			//
		}

		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form 디자이너에서 생성한 코드
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		private void InitializeComponent()
		{
			this.button1 = new MyButton.RainbowButton();
			this.button2 = new MyButton.RainbowButton2();
			this.button3 = new MyButton.RainbowButton3();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.ForeColor = System.Drawing.SystemColors.ControlText;
			this.button1.Location = new System.Drawing.Point(40, 32);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(120, 104);
			this.button1.TabIndex = 0;
			this.button1.Text = "버튼1";
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(40, 144);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(120, 96);
			this.button2.TabIndex = 1;
			this.button2.Text = "버튼2";
			// 
			// button3
			// 
			this.button3.Location = new System.Drawing.Point(176, 48);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(208, 168);
			this.button3.TabIndex = 2;
			this.button3.Text = "버튼3";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(9, 19);
			this.ClientSize = new System.Drawing.Size(424, 286);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.Font = new System.Drawing.Font("굴림체", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(129)));
			this.ForeColor = System.Drawing.SystemColors.ControlLightLight;
			this.Name = "Form1";
			this.Text = "무지개 버튼 만들기";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// 해당 응용 프로그램의 주 진입점입니다.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}
	}
}
