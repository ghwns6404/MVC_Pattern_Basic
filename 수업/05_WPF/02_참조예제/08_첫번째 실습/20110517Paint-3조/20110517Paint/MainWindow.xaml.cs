using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;

namespace _20110517Paint
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {

        #region 데이터 선언부' ㅁ '/
        public LinearGradientBrush m_NowBrush;
        public string nowshape;

        //Grid gridMain = new Grid();
        Grid gridOption = new Grid();
        public StackPanel DrawStack = new StackPanel();

        #endregion


        public MainWindow()
        {
            InitializeComponent();

            m_NowBrush = MyBrush.RedBrush;
            nowshape = MyShape.Ell;

            Content = gridMain;
            MakeGrid();
        }

        private void Window_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (Mouse.GetPosition(DrawStack).X > 0) 
            {
                if (nowshape == MyShape.Ell)
                {
                    MyEllipse newdata = new MyEllipse(m_NowBrush, Mouse.GetPosition(DrawStack));
                    DrawStack.Children.Add(newdata);
                }
                else
                {
                    MyRectangle newdata = new MyRectangle(m_NowBrush, Mouse.GetPosition(DrawStack));
                    DrawStack.Children.Add(newdata);
                }
            }
        }



        #region  그리드 및 컨트롤 생성 ..
        public void MakeGrid()
        {

            #region Main 그리드
            gridMain.ColumnDefinitions.Add(new ColumnDefinition());
            gridMain.ColumnDefinitions.Add(new ColumnDefinition());
            gridMain.ColumnDefinitions.Add(new ColumnDefinition());
            
            gridMain.ColumnDefinitions[1].Width = new GridLength(10);
            gridMain.ColumnDefinitions[0].Width = new GridLength(100);

            GridSplitter gspi = new GridSplitter();
            gspi.Width = 5;
            gridMain.Children.Add(gspi);
            Grid.SetRow(gspi, 0);
            Grid.SetColumn(gspi, 1);
            //Label lb = new Label();
            //lb.Width = 10;
            //lb.Background = Brushes.Black;

            //gridMain.Children.Add(lb);
            //Grid.SetRow(lb, 1);
            //Grid.SetColumn(lb, 1);


            gridMain.Children.Add(gridOption);
            Grid.SetRow(gridOption, 0);
            Grid.SetColumn(gridOption, 0);


            gridMain.Children.Add(DrawStack);
            Grid.SetRow(DrawStack, 2);
            Grid.SetColumn(DrawStack, 2);
            #endregion


            #region 버튼그리드.
           
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition()); 
            gridOption.RowDefinitions.Add(new RowDefinition());
            gridOption.RowDefinitions.Add(new RowDefinition());

            foreach ( RowDefinition rd in gridOption.RowDefinitions )
            {
                rd.Height = new GridLength(50) ;
            }
            gridOption.RowDefinitions[2].Height  =new GridLength(10) ;
            gridOption.RowDefinitions[7].Height = new GridLength(10);
            
            

            Button bt;

            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Rect);
            bt.Content = "사 각 형";
            bt.FontSize = 20;
            bt.FontWeight = FontWeights.UltraBold;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 0);
            Grid.SetColumn(bt, 0);

            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Ellipse);
            bt.Content = "동그라미";
            bt.FontSize = 20;
            bt.FontWeight = FontWeights.UltraBold;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 1);
            Grid.SetColumn(bt, 1);

            Label temp = new Label();
            temp.Background = Brushes.Black;
            gridOption.Children.Add(temp);
            Grid.SetRow(temp, 2);
            Grid.SetColumn(temp, 2);

            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Blak);
            bt.Background = MyBrush.DefBrush;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 3);
            Grid.SetColumn(bt, 3);


            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Red);
            bt.Background = MyBrush.RedBrush;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 4);
            Grid.SetColumn(bt, 4);

            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Blue);
            bt.Background = MyBrush.BlueBrush;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 5);
            Grid.SetColumn(bt, 5);


            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Green);
            bt.Background = MyBrush.GreenBrush;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 6);
            Grid.SetColumn(bt, 6);

            temp = new Label();
            temp.Background = Brushes.Black;
            gridOption.Children.Add(temp);
            Grid.SetRow(temp, 7);
            Grid.SetColumn(temp, 7);


            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Load);
            bt.Content = "열    기";
            bt.FontSize = 20;
            bt.FontWeight = FontWeights.UltraBold;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 8);
            Grid.SetColumn(bt, 8);

            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Save);
            bt.Content = "저    장";
            bt.FontSize = 20;
            bt.FontWeight = FontWeights.UltraBold;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 9);
            Grid.SetColumn(bt, 9);

            bt = new Button();
            bt.Click += new RoutedEventHandler(bt_Click_Clear);
            bt.Content = "지 우 기";
            bt.FontSize = 20;
            bt.FontWeight = FontWeights.UltraBold;
            gridOption.Children.Add(bt);
            Grid.SetRow(bt, 10);
            Grid.SetColumn(bt, 10);


            #endregion
        }
        #endregion


        #region 세이브, 클리어 버튼 이밴트.

        void bt_Click_Load(object sender, RoutedEventArgs e)
        {

        }

        void bt_Click_Save(object sender, RoutedEventArgs e)
        {

        }
       
        
        void bt_Click_Clear(object sender, RoutedEventArgs e)
        {
            DrawStack.Children.Clear();
        }

        #endregion

        #region 프로퍼티바꾸는 버튼이벤트.
        void bt_Click_Red(object sender, RoutedEventArgs e)
        {
            m_NowBrush = MyBrush.RedBrush;
        }

        void bt_Click_Blue(object sender, RoutedEventArgs e)
        {
            m_NowBrush = MyBrush.BlueBrush;
        }

        void bt_Click_Green(object sender, RoutedEventArgs e)
        {
            m_NowBrush = MyBrush.GreenBrush;
        }

        void bt_Click_Blak(object sender, RoutedEventArgs e)
        {
            m_NowBrush = MyBrush.DefBrush;
        }

        void bt_Click_Ellipse(object sender, RoutedEventArgs e)
        {
            nowshape = MyShape.Ell;
        }

        void bt_Click_Rect(object sender, RoutedEventArgs e)
        {
            nowshape = MyShape.Rec;
        }

        #endregion
               
    }


    #region 그리기에 관한 코드..
    public class MyRectangle : FrameworkElement
    {
        Rect rt = new Rect();
        LinearGradientBrush m_Brush;

        public MyRectangle(LinearGradientBrush _Brush, Point _pt)
        {
            rt.X = _pt.X;
            rt.Y = _pt.Y;
            rt.Width = 40;
            rt.Height = 40;
            m_Brush = _Brush;
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {  
            object  ob = this.Parent;
            StackPanel ob2 = (StackPanel)ob;
            ob2.Children.Remove(this);
            base.OnMouseRightButtonDown(e);
          
        }

        protected override void OnRender(DrawingContext dc)
        {
            dc.DrawRectangle(m_Brush, new Pen(), rt);
            base.OnRender(dc);
        }
    }


    public class MyEllipse : FrameworkElement
    {
        Point pt;
        LinearGradientBrush m_Brush;

        public MyEllipse(LinearGradientBrush _Brush, Point _pt)
        {
            pt = _pt;
            pt.X = pt.X + 20;
            pt.Y = pt.Y + 20;
            m_Brush = _Brush;
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            object ob = this.Parent;
            StackPanel ob2 = (StackPanel)ob;
            ob2.Children.Remove(this);
            base.OnMouseRightButtonDown(e);

        }

        protected override void OnRender(DrawingContext dc)
        {
            dc.DrawEllipse(m_Brush, new Pen(), pt, 20, 20);
            base.OnRender(dc);
        }
    }
    # endregion


    #region 개인 스태틱 지정값들..

    public class MyBrush
    {
        public static readonly LinearGradientBrush DefBrush = new LinearGradientBrush(Colors.Black, Colors.LightGray, new Point(0, 0), new Point(1, 1));
        public static readonly LinearGradientBrush RedBrush = new LinearGradientBrush(Colors.Red, Colors.Yellow, new Point(0, 0), new Point(1, 1));
        public static readonly LinearGradientBrush BlueBrush = new LinearGradientBrush(Colors.Blue, Colors.Aqua, new Point(0, 0), new Point(1, 1));
        public static readonly LinearGradientBrush GreenBrush = new LinearGradientBrush(Colors.Green, Colors.Lime, new Point(0, 0), new Point(1, 1));
    }

    public class MyShape
    {
        public static readonly string Ell = "Ellipse";
        public static readonly string Rec = "Rectangle";
    }

    #endregion
}
