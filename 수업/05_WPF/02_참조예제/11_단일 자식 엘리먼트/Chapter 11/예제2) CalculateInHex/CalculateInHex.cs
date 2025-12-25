//-----------------------------------------------
// CalculateInHex.cs (c) 2006 by Charles Petzold
//-----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;

namespace Petzold.CalculateInHex
{
    public class CalculateInHex : Window
    {
        // Private 필드
        RoundedButton btnDisplay;
        ulong numDisplay;
        ulong numFirst;
        bool bNewNumber = true;
        char chOperation = '=';

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new CalculateInHex());
        }
        // 생성자
        public CalculateInHex()
        {
            Title = "Calculate in Hex";
            SizeToContent = SizeToContent.WidthAndHeight;       // 콘텐츠 크기를 창의 크기에 맞게 자동으로 조절
            ResizeMode = ResizeMode.CanMinimize;                // 창 최소화와 복원만 사용 가능

            // Grid를 생성하고 Content로 지정
            Grid grid = new Grid();
            grid.Margin = new Thickness(4);                     // grid여백 설정
            Content = grid;                                     // grid를 Content로 지정

            // 5개의 열을 생성
            for (int i = 0; i < 5; i++)
            {
                ColumnDefinition col = new ColumnDefinition();
                col.Width = GridLength.Auto;                    // col의 너비 자동 설정
                grid.ColumnDefinitions.Add(col);                // 5개의 열을 생성
            }

            // 7개의 행을 생성
            for (int i = 0; i < 7; i++)
            {
                RowDefinition row = new RowDefinition();
                row.Height = GridLength.Auto;                   // row의 높이 자동 설정
                grid.RowDefinitions.Add(row);                   // 7개의 행을 생성
            }

            // 버튼에 보여질 텍스트
            string[] strButtons = { "0",
                                    "D", "E", "F", "+", "&",
                                    "A", "B", "C", "-", "|",
                                    "7", "8", "9", "*", "^",
                                    "4", "5", "6", "/", "<<",
                                    "1", "2", "3", "%", ">>", 
                                    "0", "Back", "Equals" };
            int iRow = 0, iCol = 0;

            // 버튼 생성
            foreach (string str in strButtons)
            {
                // RoundedButton 생성
                RoundedButton btn = new RoundedButton();
                btn.Focusable = false;
                btn.Height = 32;                            // 버튼 높이
                btn.Margin = new Thickness(4);              // 버튼 여백 설정
                btn.Click += ButtonOnClick;

                // TextBlock을 생성해서 RoundedButton의 자식으로 지정
                TextBlock txt = new TextBlock();
                txt.Text = str;
                btn.Child = txt;

                // RoundedButton을 Grid에 추가
                grid.Children.Add(btn);
                Grid.SetRow(btn, iRow);
                Grid.SetColumn(btn, iCol);

                // 버튼 출력 때 예외 상황 처리
                if (iRow == 0 && iCol == 0)
                {
                    btnDisplay = btn;
                    btn.Margin = new Thickness(4, 4, 4, 6);
                    Grid.SetColumnSpan(btn, 5);                 // btn의 5개의 열을 합친다.
                    iRow = 1;
                }

                // Back과 Equals 버튼 처리
                else if (iRow == 6 && iCol > 0)
                {
                    Grid.SetColumnSpan(btn, 2);
                    iCol += 2;
                }

                // 나머지 버튼의 경우
                else
                {
                    btn.Width = 32;
                    if (0 == (iCol = (iCol + 1) % 5))
                        iRow++;
                }
            }
        }

        // Click 이벤트 핸들러
        void ButtonOnClick(object sender, RoutedEventArgs args)
        {
            // 클릭된 버튼 구하기
            RoundedButton btn = args.Source as RoundedButton;

            if (btn == null)
                return;

            // 버튼 텍스트와 첫 글자 구하기
            string strButton = (btn.Child as TextBlock).Text;
            char chButton = strButton[0];

            // 몇 가지 특이한 경우
            if (strButton == "Equals")
                chButton = '=';

            if (btn == btnDisplay)
                numDisplay = 0;

            else if (strButton == "Back")
                numDisplay /= 16;

            // 16진수 글자
            else if (Char.IsLetterOrDigit(chButton))
            {
                if (bNewNumber)
                {
                    numFirst = numDisplay;
                    numDisplay = 0;
                    bNewNumber = false;
                }
                if (numDisplay <= ulong.MaxValue >> 4)
                    numDisplay = 16 * numDisplay + (ulong)(chButton - 
                                    (Char.IsDigit(chButton) ? '0' : 'A' - 10));
            }

            // 연산
            else
            {
                if (!bNewNumber)
                {
                    switch (chOperation)
                    {
                        case '=': break;
                        case '+': numDisplay = numFirst + numDisplay; break;
                        case '-': numDisplay = numFirst - numDisplay; break;
                        case '*': numDisplay = numFirst * numDisplay; break;
                        case '&': numDisplay = numFirst & numDisplay; break;
                        case '|': numDisplay = numFirst | numDisplay; break;
                        case '^': numDisplay = numFirst ^ numDisplay; break;
                        case '<': numDisplay = numFirst << (int)numDisplay; break;
                        case '>': numDisplay = numFirst >> (int)numDisplay; break;
                        case '/':
                            numDisplay = 
                                numDisplay != 0 ? numFirst / numDisplay : 
                                                  ulong.MaxValue;
                            break;
                        case '%':
                            numDisplay = 
                                numDisplay != 0 ? numFirst % numDisplay : 
                                                  ulong.MaxValue;
                            break;
                        default: numDisplay = 0; break;
                    }
                }
                bNewNumber = true;
                chOperation = chButton;
            }

            // 출력 형식 지정
            TextBlock text = new TextBlock();
            text.Text = String.Format("{0:X}", numDisplay);
            btnDisplay.Child = text;
        }

        protected override void OnTextInput(TextCompositionEventArgs args)
        {
            base.OnTextInput(args);

            if (args.Text.Length == 0)
                return;

            // 문자 입력 구하기
            char chKey = Char.ToUpper(args.Text[0]);

            // 버튼을 통한 루프
            foreach (UIElement child in (Content as Grid).Children)
            {
                RoundedButton btn = child as RoundedButton;
                string strButton = (btn.Child as TextBlock).Text;

                // 일치하는 버튼을 확인하기 위한 로직
                if ((chKey == strButton[0] && btn != btnDisplay &&
                        strButton != "Equals" && strButton != "Back") ||
                    (chKey == '=' && strButton == "Equals") ||
                    (chKey == '\r' && strButton == "Equals") ||
                    (chKey == '\b' && strButton == "Back") ||
                    (chKey == '\x1B' && btn == btnDisplay))
                {
                    // 키 입력을 처리하기 위해 Click 이벤트를 발생
                    RoutedEventArgs argsClick = 
                        new RoutedEventArgs(RoundedButton.ClickEvent, btn);
                    btn.RaiseEvent(argsClick);

                    // 버튼이 눌린 것처럼 표시
                    btn.IsPressed = true;

                    // 버튼을 다시 떼어 놓는 상태로 만들기 위한 타이머 설정
                    DispatcherTimer tmr = new DispatcherTimer();
                    tmr.Interval = TimeSpan.FromMilliseconds(100);
                    tmr.Tag = btn;
                    tmr.Tick += TimerOnTick;
                    tmr.Start();

                    args.Handled = true;
                } 
            }
        }

        void TimerOnTick(object sender, EventArgs args)
        {
            // 눌린 버튼 복원
            DispatcherTimer tmr = sender as DispatcherTimer;
            RoundedButton btn = tmr.Tag as RoundedButton;
            btn.IsPressed = false;

            // 타이머 종료하고 이벤트 핸들러 제거
            tmr.Stop();
            tmr.Tick -= TimerOnTick;
        }
    }
}
