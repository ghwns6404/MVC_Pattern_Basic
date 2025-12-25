using System;
using System.Runtime.CompilerServices;
using System.Windows.Forms;
using Word_Quiz.Database;
using Word_Quiz.Forms;

namespace Word_Quiz
{
    public partial class StartForm : Form
    {
        MyDB db = new MyDB();
        public StartForm()
        {
            InitializeComponent();
            db.Connect();
        }

        #region 난이도 선택
        public void animalButton_Click(object sender, EventArgs e)
        {
            string selectedTable = "EASY";
            GameForm gameForm = new GameForm(selectedTable, this);
            gameForm.Show();
            this.Hide();
        }
        
        private void jobButton_Click(object sender, EventArgs e)
        {
            string selectedTable = "NORMAL";
            GameForm gameForm = new GameForm(selectedTable, this);
            gameForm.Show();
        }

        private void fruitButton_Click(object sender, EventArgs e)
        {
            string selectedTable = "HARD";
            GameForm gameForm = new GameForm(selectedTable, this);
            gameForm.Show();
        }
        #endregion
    }
}
