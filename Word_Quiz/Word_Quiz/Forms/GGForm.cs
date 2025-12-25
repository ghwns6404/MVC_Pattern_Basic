//GGForm.cs

using System.Drawing;
using System.Windows.Forms;
using Word_Quiz.Forms;

namespace Word_Quiz
{
    public partial class GGForm : Form
    {
        public StartForm startForm;
        #region 생성자
        public GGForm(GameForm gameform, int total_score, int O_score, int X_score, StartForm startForm, string tableName)
        {
            InitializeComponent();
            Ending(total_score, O_score, X_score);
            this.startForm = startForm;
            this.levelBox.Text = tableName;
        }
        #endregion

        #region 겜 끝나고 등급출력할때 로직
        public void Ending(int total_score, int O_score, int X_score)
        {
            oBox.Text = O_score.ToString();
            xBox.Text = X_score.ToString();

            int jumsu = O_score - X_score;
            jumsuBox.Text = jumsu.ToString();
            #region 등급출력
            if (jumsu * 10 >= 90)
            {
                gradeBox.BackColor = Color.Green;
                gradeBox.Text = "A+";
            }
            else if (jumsu * 10 >= 80)
            {
                gradeBox.BackColor = Color.LightGreen;
                gradeBox.Text = "B";
            }
            else if (jumsu * 10 >= 70)
            {
                gradeBox.BackColor = Color.Yellow;
                gradeBox.Text = "C";
            }
            else if (jumsu * 10 >= 60)
            {
                gradeBox.BackColor = Color.Orange;
                gradeBox.Text = "D";
            }
            else
            {
                gradeBox.BackColor = Color.Red;
                gradeBox.Text = "병신";
            }
            #endregion
        }
        #endregion

        #region 폼 닫힐때 start폼 재출력
        private void GGForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            startForm.Show();
        }
        #endregion
    }
}
