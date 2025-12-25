using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BingoGameSample
{          
    public partial class BingoSingleBoard : UserControl
    {
        const int SIZE = 5;
        Button[,] playerButtons     = new Button[SIZE, SIZE];   //버튼
        int[,] playerBoard          = new int[SIZE, SIZE];      //버튼의 숫자
        bool[,] playerMarked        = new bool[SIZE, SIZE];     //선택여부
        HashSet<int> pickedNumbers  = new HashSet<int>();       //중복버튼 선택 에러 확인용

        public event BoardButtonClick NumberClick = null; // delegate event 선언

        [Category("IsShow"), Description("번호를 보여줄지 여부")]
        public bool IsShow { get; set; } = true;

        public BingoSingleBoard()
        {
            InitializeComponent();

            InitBoards();
        }

        #region 초기화 과정(InitBoards) : 렌덤하게 보드의 값 설정, 버튼 생성(clickHandler) 등록
        private void InitBoards()
        {
            GenerateNumbers(playerBoard);
            CreateButtons(playerButtons, playerBoard, new Point(20, 20), PlayerClick);
        }        
        

        private void CreateButtons(Button[,] buttons, int[,] board, Point origin, EventHandler clickHandler)
        {
            int btnSize = 50;
            int number = 1;
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    Button btn = new Button();
                    //btn.Text        = board[i, j].ToString();
                    if (IsShow == true)
                        btn.Text = (number++).ToString();
                    else
                        btn.Text = "";
                    btn.Size = new Size(btnSize, btnSize);
                    btn.Location    = new Point(origin.X + j * btnSize, origin.Y + i * btnSize);
                    if (clickHandler != null) btn.Click += clickHandler;
                    this.Controls.Add(btn);
                    buttons[i, j] = btn;
                }
            }
        }
        #endregion

        #region 렌덤하게 번호 설정하기(SetNumberRandom)
        public void SetNumberRandom()
        {
            GenerateNumbers(playerBoard);

            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    Button btn = playerButtons[i, j];
                    //btn.Text   = playerBoard[i, j].ToString();
                    if (IsShow == true)
                        btn.Text = playerBoard[i, j].ToString();
                    else
                        btn.Text = "";
                }
            }
        }
        private void GenerateNumbers(int[,] board)
        {
            Random rand = new Random();
            var nums = Enumerable.Range(1, 25).OrderBy(x => rand.Next()).ToArray();
            int index = 0;
            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    board[i, j] = nums[index++];            
        }
        #endregion

        #region 버튼 클릭 이벤트(PlyerClick) -> 마킹 -> 빙고여부 판단
        private void PlayerClick(object sender, EventArgs e)
        {
            Button btn = sender as Button;
            int number = int.Parse(btn.Text);

            if (pickedNumbers.Contains(number)) return;
            pickedNumbers.Add(number);

            //************************ 수정 **********************************
            int x = -1, y = -1;
            MarkNumber(number, ref x, ref y);

            //이벤트 발생
            if (NumberClick != null)
                NumberClick(this, new NumberEventArgs(number, x, y));            
            //*******************************************************************

            if (CheckBingo(playerMarked) >= 5)
            {
                MessageBox.Show("🎉 플레이어 승리!");
               // this.Close();
                return;
            }
        }        
        private void MarkNumber(int number, ref int x, ref int y)
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    if (playerBoard[i, j] == number)
                    {
                        playerMarked[i, j] = true;
                        playerButtons[i, j].BackColor = Color.LightGreen;
                        x = i; y = j;   //************************************
                    }
                }
            }
        }
       
        private int CheckBingo(bool[,] marked)
        {
            int count = 0;

            for (int i = 0; i < SIZE; i++)
            {
                if (Enumerable.Range(0, SIZE).All(j => marked[i, j])) count++;
                if (Enumerable.Range(0, SIZE).All(j => marked[j, i])) count++;
            }

            if (Enumerable.Range(0, SIZE).All(i => marked[i, i])) count++;
            if (Enumerable.Range(0, SIZE).All(i => marked[i, SIZE - i - 1])) count++;

            return count;
        }
        #endregion
    }

    public class NumberEventArgs : EventArgs
    {
        public int Number { get; private set; }
        public int X { get; private set; }
        public int Y { get; private set; }
        public NumberEventArgs(int number, int x, int y)
        {
            Number = number;
            X = x;
            Y = y;
        }
    }

    // Control의 부모 쪽으로 전달할 Event Delegate         
    public delegate void BoardButtonClick(object Sender, NumberEventArgs e);
}
