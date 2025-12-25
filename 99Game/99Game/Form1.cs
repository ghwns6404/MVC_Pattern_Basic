using System;
using System.Windows.Forms;


namespace _99Game
{
    public partial class Form1 : Form
    {
        public Random random = new Random();
        public Timer timer = new Timer();

        public int lefttime = 30;   //게임진행시간은 여기서 변경 !!

        public int score = 0; //초기 점수
        public int level = 1; //초기 단계

        public Form1()
        {
            InitializeComponent();
            timer.Interval = 1000;  //타이머 간격

            timer.Tick += (_sender, _e) =>  //이벤트를 구독
            {
                lefttime--;
                lefttimeBox.Text = lefttime.ToString();
                if (lefttime == 0)
                {
                    timer.Stop();
                    lefttime = 15;
                    GG gg = new GG(score, this);
                    gg.ShowDialog();
                }

            };
        }
        public void start_Click(object sender, EventArgs e)
        {
            Start();
        }
        private void answer_KeyDown(object sender, KeyEventArgs e) //눌렀을때만 발생
        {
            try
            {
                if (e.KeyCode == Keys.Enter)
                {
                    int a = int.Parse(currentA.Text);
                    int b = int.Parse(currentB.Text);
                    int answer = a * b;

                    int userInput = int.Parse(answerBox.Text.ToString());

                    if (answer == userInput)
                    {
                        score++;
                        scoreBox.Text = score.ToString();
                        answerBox.Text = "";
                    }
                    else
                    {
                        score--;
                        scoreBox.Text = score.ToString();
                        answerBox.Text = "";
                    }
                    MakeQuiz();
                    e.SuppressKeyPress = true;
                }
            }
            catch(Exception)
            {
                MessageBox.Show("숫자만 입력!", "숫자만 입력!", MessageBoxButtons.RetryCancel);
                answerBox.Text = "";
            }
           
        }
        public void MakeQuiz()
        {
            if(score < 10)
            {
                level = 1;
                levelBox.Text = level.ToString();

                int a = random.Next(1, 10);
                currentA.Text = a.ToString();

                int b = random.Next(1, 10);
                currentB.Text = b.ToString();
            }
            else if(score < 20)
            {
                level = 2;
                levelBox.Text = level.ToString();

                int a = random.Next(3, 8);
                currentA.Text = a.ToString();

                int b = random.Next(12, 18);
                currentB.Text = b.ToString();
            }
            else
            {
                level = 3;
                levelBox.Text = level.ToString();

                int a = random.Next(10, 19);
                currentA.Text = a.ToString();

                int b = random.Next(10, 19);
                currentB.Text = b.ToString();
            }
            
        }
        public void Start()
        {
            score = 0;
            scoreBox.Text = score.ToString();   //점수

            levelBox.Text = level.ToString();   //단계

            timer.Start();
            MakeQuiz();
        }
    }
}
