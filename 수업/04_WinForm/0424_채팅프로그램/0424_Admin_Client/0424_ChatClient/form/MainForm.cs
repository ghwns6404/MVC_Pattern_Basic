using System;
using System.Windows.Forms;

namespace _0424_ChatClient
{
    public partial class MainForm : Form
    {
        private MemberControl memControl = MemberControl.Singleton;

        public MainForm()
        {
            InitializeComponent();
        }

        #region Form_Load, Form_Closed
        private void LogInForm_Load(object sender, EventArgs e)
        {
            button1_Click(sender, e);
        }
        private void LogInForm_FormClosed(object sender, FormClosedEventArgs e)
        {
        }
        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            dataGridView1.DataSource = MemberControl.Singleton.GetMemberTable();
        }
    }
}
