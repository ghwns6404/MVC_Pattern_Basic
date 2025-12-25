using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DialogMode
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btn_modal_Click(object sender, EventArgs e)
        {
            /*
            Form frm = new Form();
            frm.Text = "모달 예제";
            frm.Size = new Size(200, 200);
            frm.ShowDialog();
            */

            ExamForm frm = new ExamForm();
            frm.Text = "반환값을 갖는 모달 예제";
            DialogResult result = frm.ShowDialog();

            if(result == DialogResult.OK)
            {
                this.Text = "모달 대화상자 반환값은 OK";
            }
            else if(result == DialogResult.Cancel)
            {
                this.Text = "모달 대화상자 반환값은 Cancel";
            }
            else if(result == DialogResult.Retry)
            {
                this.Text = "모달 대화상자 반환값은 Retry";
            }
            else if(result == DialogResult.Abort)
            {
                this.Text = "모달 대화상자 반환값은 Abort";
            }
            else
            {
                this.Text = "모달 대화상자 반환값은 모름";
            }
        }

        private void btn_modㄷless_Click(object sender, EventArgs e)
        {
            Form frm = new Form();
            frm.Text = "모덜리스 예제";
            frm.Size = new Size(200, 200);
            frm.Show();
        }
    }
}