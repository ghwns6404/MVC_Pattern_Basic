//GameForm.cs

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Word_Quiz.Data;
using Word_Quiz.Database;

namespace Word_Quiz.Forms
{
    public partial class GameForm : Form
    {
        #region 선언부 
        public MyDB db = new MyDB();
        public Random random = new Random();
        public Timer timer = new Timer();
        public StartForm newstarform;

        public int lefttime = 10;   //남은시간 조절
        public int O_score = 0;     //맞은갯수
        public int X_score = 0;     //틀린갯수
        public int total_score = 0; //총 점수 (등급매길때)

        public int correctIndex;    //정답의 인덱스
        public string tableName;    //난이도
        public string correctAnswer;//정답(오답노트용)
        #endregion

        #region 생성자
        public GameForm(string selectedTable, StartForm startform)
        {
            InitializeComponent();
            newstarform = startform;
            tableName = selectedTable;
            if (!db.Connect())
            {
                MessageBox.Show("DB 연결 실패");
                return;
            }
            QuestionShow(tableName);
            timer.Start();

            timer.Interval = 1000;  //타이머 간격
            timer.Tick += (_sender, _e) =>  //이벤트를 구독
            {
                lefttime--;
                lefttimeBox.Text = lefttime.ToString();
                if (lefttime == 0)
                {
                    timer.Stop();
                    lefttime = 15;
                    GGForm gg = new GGForm(this, total_score, O_score, X_score, startform, tableName);
                    this.Close();
                    gg.ShowDialog();
                }
            };
        }
        #endregion

        #region 문제 생성
        public void QuestionShow(string tableName)
        {
            List<string> total_question = new List<string>();

            QuizData correct = db.db_GetCorrect(tableName);
            if (correct == null)
            {
                MessageBox.Show("문제 생성중 에러 ");
                return;
            }

            
            List<string> wrongs = db.GetWrongChoices(tableName, correct.Word);
            if (wrongs.Count < 3)
            {
                MessageBox.Show("오답 생성중 에러");
                return;
            }

            //total_question이라는 새로운 배열에 
            total_question.Add(correct.Word);
            total_question.AddRange(wrongs);

            Shuffle(total_question);    //섞고

            correctIndex = total_question.IndexOf(correct.Word);    //답의 인덱스를 저장
            //correctAnswer = correct.Word;                         //오답노트용으로 답 자체를 저장

            meanBox.Text = correct.Meaning;
            qBox1.Text = total_question[0];
            qBox2.Text = total_question[1];
            qBox3.Text = total_question[2];
            qBox4.Text = total_question[3];

            //MessageBox.Show($"정답 단어: {correct.Word}, 정답 인덱스: {correctIndex}");    디버깅용임 콘솔어케띄우는지몰라서
            //MessageBox.Show($"보기 리스트: {string.Join(", ", total_question)}");
        }
        #endregion

        #region 문제 셔플 알고리즘
        /// <summary>
        /// 피셔예이츠 셔플 알고리즘
        /// C#에서 리스트를 무작위로 섞는 방법
        /// 피셔예이츠는 C#에서 리스트를 무작위로 섞는 국룰방법
        /// </summary>
        public void Shuffle<T>(List<T> list)    
        {
            for (int i = list.Count - 1; i > 0; i--)
            {
                int j = random.Next(i + 1);
                T temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
        #endregion

        #region 답 제출 (엔터 누를때)
        private void answer_Box_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (e.KeyCode == Keys.Enter)
                {
                    int userInput = int.Parse(answer_Box.Text.Trim());
                    if (userInput < 1 || userInput > 4)
                    {
                        MessageBox.Show("1~4 사이의 숫자를 입력하세요.", "입력 오류", MessageBoxButtons.OK);
                        answer_Box.Text = "";
                        return; 
                    }

                    int selectedIndex = userInput - 1;

                    if (selectedIndex == correctIndex)
                    {
                        answer_Box.Text = "";
                        O_score++;
                        total_score++;
                        scoreBox.Text = total_score.ToString();
                    }
                    else
                    {
                        answer_Box.Text = "";
                        X_score++;
                        total_score++;
                        scoreBox.Text = total_score.ToString();
                    }
                    QuestionShow(tableName);
                }
            }
            catch (Exception)
            {
                MessageBox.Show("숫자만 입력!", "숫자만 입력!", MessageBoxButtons.RetryCancel);
                answer_Box.Text = "";
            }
        }
        #endregion

        #region 폼 닫힐때 close~
        private void GameForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            db.Close();
        }
        #endregion
    }
}
