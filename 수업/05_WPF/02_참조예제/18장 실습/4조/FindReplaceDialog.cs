using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Windows;
using System.Windows.Controls;
namespace Notepad
{
    class FindReplaceDialog : Window
    {
        // public 이벤트
        public event EventHandler FindNext;             //찾는내용
        public event EventHandler Replace;              //수정    
        public event EventHandler ReplaceAll;           //글 전체 수정

        //protected 필드
        protected Label lblReplace;                     //걍 라벨
        protected TextBox txtboxFind, txtboxReplace;    //찾는 내용 텍스트박스
        protected CheckBox checkMatch;                  //대소문자 구별 쳌박스

        protected GroupBox groupDirection;
        protected RadioButton radioDown, radioUp;       //내용 위아래 라디오버튼
        protected Button btnFind, btnReplace, btnAll;   //기타 버튼들


        //public  프로퍼티

        //텍스트박스 찾는 프로퍼티
        public string FindWhat
        {
            set { txtboxFind.Text = value; }               
            get { return txtboxReplace.Text; }
        }

        //텍스트박스 수정하는 프로퍼티
        public string ReplaceWith
        {
            set { txtboxReplace.Text = value; }
            get { return txtboxReplace.Text; }
        }

        // 대소문자 검사 프로퍼티
        public bool MatchCase 
        {
            set { checkMatch.IsChecked = value; }
            get { return (bool)checkMatch.IsChecked; }
        }
        //find 다이얼로그 프로퍼티
        public Direction Direction
        {
            set
            {
                if (value == Direction.Down)             
                    radioDown.IsChecked = true;
                else
                    radioUp.IsChecked = true;
            }
            get
            {
                return (bool)radioDown.IsChecked ? Direction.Down : Direction.Up;   //라디오버튼이 체크값 리턴 
            }
        }

        // Protected 생성자 (추상클래스이기 때문)
        protected FindReplaceDialog(Window owner)
        {
            //공용 대화상자 프로퍼티를 설정
            ShowInTaskbar = false;                          //작업 표시줄 표시 안함
            WindowStyle = WindowStyle.ToolWindow;           //윈도우 도구창 
            SizeToContent = SizeToContent.WidthAndHeight;
            WindowStartupLocation = WindowStartupLocation.CenterOwner;
            Owner = owner;                                  //오너 권한 설정

            //자동으로 크기가 변하는 3개의 행과 열을 가진 grid를 생성
            Grid grid = new Grid();
            Content = grid;

            //find 창 만드는 부분
            for (int i = 0; i < 3; i++)
            {
                
                RowDefinition rowdef = new RowDefinition();         
                rowdef.Height = GridLength.Auto;
                grid.RowDefinitions.Add(rowdef);

                ColumnDefinition coldef = new ColumnDefinition();
                coldef.Width = GridLength.Auto;
                grid.ColumnDefinitions.Add(coldef);
            }

            // 찾을 내용 : 라벨과 텍스트 박스
            Label lbl = new Label();
            lbl.Content = "Fi_nd what:";                                //find what 라벨
            lbl.VerticalAlignment = VerticalAlignment.Center;           //위치 가운데    
            lbl.Margin = new Thickness(12);                             
            grid.Children.Add(lbl);
            Grid.SetRow(lbl, 0);                                        //첫번째 행
            Grid.SetColumn(lbl, 0);                                     //첫번째 칼럼

            txtboxFind = new TextBox();                                 //내용 찾는 txtbox 생성
            txtboxFind.Margin = new Thickness(12);
            txtboxFind.TextChanged += FindTextBoxOnTextChanged;         //찾은 내용을 txtbox내용으로 바꿈
            grid.Children.Add(txtboxFind);
            Grid.SetRow(txtboxFind, 0);                                  //첫번째 행
            Grid.SetColumn(txtboxFind, 1);                               //두번째 칼럼

            // 바꿀 내용 : 라벨과 텍스트 박스
            lblReplace = new Label();                                      //라벨생성
            lblReplace.Content = "Re_place with:";                         //라벨 내용  
            lblReplace.VerticalAlignment = VerticalAlignment.Center;       //라벨 위치    
            lblReplace.Margin = new Thickness(12);
            grid.Children.Add(lblReplace);                                  //라벨 저장
            Grid.SetRow(lblReplace, 1);                                     //두넙째 행 
            Grid.SetColumn(lblReplace, 0);                                  //첫번째 칼럼

            txtboxReplace = new TextBox();                                  //텍스트박스 생성   
            txtboxReplace.Margin = new Thickness(12);                                
            grid.Children.Add(txtboxReplace);                               //텍스트박스 저장  
            Grid.SetRow(txtboxReplace, 1);                                  //두번째 행
            Grid.SetColumn(txtboxReplace, 1);                               //두번째 칼럼

            // 대/소문자 구분 체크 박스
            // 체크하면 대문자만 구별한다.
            checkMatch = new CheckBox();                                    //쳌박스 생성
            checkMatch.Content = "Match _case";                             //content 저장  
            checkMatch.VerticalAlignment = VerticalAlignment.Center;        //위치   
            checkMatch.Margin = new Thickness(12);
            grid.Children.Add(checkMatch);                                   //쳌박스 저장
            Grid.SetRow(checkMatch, 2);
            Grid.SetColumn(checkMatch, 0);

            // 글 찾는 방향(위/아래) 그룹 박스와 2개읭 라디오버튼
            groupDirection = new GroupBox();                                   //그룸박스 생성
            groupDirection.Header = "Direction";                               //content저장
            groupDirection.Margin = new Thickness(12);
            groupDirection.HorizontalAlignment = HorizontalAlignment.Left;
            grid.Children.Add(groupDirection);                                  //그룹박스 저장
            Grid.SetRow(groupDirection, 2);                                     //3번째행
            Grid.SetColumn(groupDirection, 1);                                  //두번째 칼럼 

            //스택에 잇는 글을 검사하는 부분
            StackPanel stack = new StackPanel();                                 //스택패널 생성
            stack.Orientation = Orientation.Horizontal;
            groupDirection.Content = stack;                                      //스택에 잇는 내용을 저장 

            radioUp = new RadioButton();                                        //위 라디오 버튼 생성
            radioUp.Content = "_Up";                                            //contetn 저장 
            radioUp.Margin = new Thickness(6);
            stack.Children.Add(radioUp);                                           

            radioDown = new RadioButton();                                       //아래 라디오 버튼 생성
            radioDown.Content = "_Down";                                         //contetn 저장 
            radioDown.Margin = new Thickness(6);
            stack.Children.Add(radioDown);

            // 버튼을 위한 스택 패널 생성
            stack = new StackPanel();                                            //스택패널 생성
            stack.Margin = new Thickness(6);
            grid.Children.Add(stack);


            //위와 동일....
            //메뉴 생성
            Grid.SetRow(stack, 0);
            Grid.SetColumn(stack, 2);
            Grid.SetRowSpan(stack, 3);

            //메뉴 버튼 생성.
            btnFind = new Button();
            btnFind.Content = "_Find Next";
            btnFind.Margin = new Thickness(6);
            btnFind.IsDefault = true;
            btnFind.Click += FindNextOnClick;
            stack.Children.Add(btnFind);

            btnReplace = new Button();
            btnReplace.Content = "_Replace";
            btnReplace.Margin = new Thickness(6);
            btnReplace.Click += ReplaceOnClick;
            stack.Children.Add(btnReplace);

            btnAll = new Button();
            btnAll.Content = "Replace _All";
            btnAll.Margin = new Thickness(6);
            btnAll.Click += ReplaceAllOnClick;
            stack.Children.Add(btnAll);

            Button btn = new Button();
            btn.Content = "Cancel";
            btn.Margin = new Thickness(6);
            btn.IsCancel = true;
            btn.Click += CancelOnClick;
            stack.Children.Add(btn);

            txtboxFind.Focus();
        }
        // 텍스트를 찾게 되면 3개버튼만 활성화
        void FindTextBoxOnTextChanged(object sender, TextChangedEventArgs args)
        {
            TextBox txtbox = args.Source as TextBox;          //텍스트 박스의 내용이 써지면 버튼이 활성화가 된다.
            btnFind.IsEnabled =
            btnReplace.IsEnabled =
            btnAll.IsEnabled = (txtbox.Text.Length > 0);
        }
        // FindnextonClick 메소드는 OnFindNext 메소드를 호출하고,
        // 여기에서 FindNext 이벤트 발생
        void FindNextOnClick(object sender, RoutedEventArgs args)
        {
            OnFindNext(new EventArgs());                         //find 에서 찾은 내용의 다음을 찾는다.
        }
        protected virtual void OnFindNext(EventArgs args)
        {
            if (FindNext != null)                                //더이상 fine할게 없으면 끝
                FindNext(this, args);
        }
        // Replace 이벤트가 발생하면 ReplaceOnClick 메소드는
        // OnReplace 메소드르 호출한다.
        void ReplaceOnClick(object sender, RoutedEventArgs args)
        {
            OnReplace(new EventArgs());                               //수정박스에 잇는내용으로 수정
        }
        protected virtual void OnReplace(EventArgs args)
        {
            if (Replace != null)                                        //수정할 내용이없으면 끝
                Replace(this, args);
        }
        // ReplaceAlll 이벤트가 발생하면 ReplaceAllOnClick 메소드는
        //   OnReplace 메소드르 호출한다.
        void ReplaceAllOnClick(object sender, RoutedEventArgs args)
        {
            OnReplaceAll(new EventArgs());                                //내용을 전부 찾아서 수정
        }
        protected virtual void OnReplaceAll(EventArgs args)
        {
            if (ReplaceAll != null)
                ReplaceAll(this, args);
        }
        // Cancel 버튼은 대화상자를 종료
        void CancelOnClick(object sender, RoutedEventArgs args)
        {
            Close();                                                       //종료
        }
    }
}
