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

namespace test
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        StringBuilder build = new StringBuilder("text : ");
        
        public MainWindow()
        {
            InitializeComponent();

            #region keystroker

            //Title = "Key Stroker";
            //Content = "";

            #endregion

            #region 이미지 예제

            //Title = "이미지";

            ////Uri uri = new Uri("http://main.nateimg.co.kr/img/v3/bi_basic_100818.gif");
            ////Uri uri = new Uri("C:\\Users\\Public\\Pictures\\Sample Pictures\\Tulips.jpg");
            //Uri uri = new Uri(System.IO.Path.Combine(Environment.GetEnvironmentVariable("windir"), "2011하반기.bmp"));

            //BitmapImage Bitmap = new BitmapImage(uri);

            ////BitmapImage bitmap = new BitmapImage();
            ////bitmap.BeginInit();
            ////bitmap.UriSource = uri;
            ////bitmap.EndInit();

            //Image img = new Image();
            //img.Source = Bitmap;
            //Content = img;

            //// 그림을 클라이언트에 채우는  형식...
            //img.Stretch = Stretch.None;


            ////이미지 위치변경
            //img.HorizontalAlignment = HorizontalAlignment.Right;    //오른쪽에 붙이기
            //img.VerticalAlignment = VerticalAlignment.Top;           // 위쪽에 붙이기


            ////클라이언트 경계를 설정해서 Content의 내용과 클라이언트 프레임사이에 여백두기.
            ////img.Margin = new Thickness(200);


            ////이미지에 투명도를 주면 백그라운드 설정에 따라 영향을 받게된다.   1 은 불투명..
            //img.Opacity = 0.5;
            //Background = new LinearGradientBrush(Colors.Red, Colors.Blue, new Point(0, 0), new Point(1, 1));


            ////이미지 기울이기..
            //img.LayoutTransform = new RotateTransform(20);






            #endregion

            #region 타원그리기..

            //Title = "타원임..";

            //Ellipse ellipse = new Ellipse();
            //ellipse.Fill = Brushes.AliceBlue;
            //ellipse.StrokeThickness = 24;
            //ellipse.Stroke = new LinearGradientBrush(Colors.CadetBlue, Colors.Chocolate, new Point(0, 1), new Point(1, 0));

            //ellipse.Width = 300;
            //ellipse.Height = 300;

            //ellipse.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
            //ellipse.VerticalAlignment = System.Windows.VerticalAlignment.Bottom;

            //Content = ellipse;
            #endregion

            #region TextBlock 사용
            //TextBlock txt = new TextBlock();
            //txt.FontSize = 32;
            //txt.Inlines.Add("This is some");
            //txt.Inlines.Add(new Italic(new Run("italic")));
            //txt.Inlines.Add(" text, and this is some");
            //txt.Inlines.Add(new Bold(new Run("bold")));
            //txt.Inlines.Add(" text, and let`s cap it off with some");
            //txt.Inlines.Add(new Bold(new Italic(new Run("bold italic"))));
            //txt.Inlines.Add(" text.");
            //txt.TextWrapping = TextWrapping.Wrap;
            ////txt.FontSize = 45;
            //Foreground = Brushes.CornflowerBlue;
            //Content = txt;
            #endregion

            #region Textblock 사용자 정의 이벤트 사용
            TextBlock txt = new TextBlock();
            txt.FontSize = 32;
            txt.HorizontalAlignment = System.Windows.HorizontalAlignment.Center;
            txt.VerticalAlignment = System.Windows.VerticalAlignment.Center;

            Content = txt;

            string strQuote = "To be, or not to be, that is the question";
            string[] strWords = strQuote.Split();

            foreach (string str in strWords)
            {
                Run run = new Run(str);
                run.MouseDown += RunOnMouseDown;
                txt.Inlines.Add(run);
                txt.Inlines.Add(" ");
            }
            #endregion

            //#region FrameworkElement 상속받은 사용자 정의 Class 사용
            //SimpleEllipse sample = new SimpleEllipse();
            //Content = sample;
            //#endregion

        }

        #region String...
        //protected override void OnTextInput(TextCompositionEventArgs e)
        //{
        //    base.OnTextInput(e);

        //    string str = Content as string;

        //    if (e.Text == "\b")
        //    {
        //        if (str.Length > 0)
        //            str = str.Substring(0, str.Length - 1);

        //    }

        //    else
        //    {
        //        str += e.Text;
        //    }

        //    Content = str;

        //}
        #endregion

        #region StringBuilder
        //protected override void OnTextInput(TextCompositionEventArgs e)
        //{
        //    base.OnTextInput(e);
        //    string str = Content as string; //string 으로 강제 캐스팅..

        //    if (e.Text == "\b") // 백스페이스.... 
        //    {
        //        if (build.Length > 0) // 문자가 있을 경우
        //            str = str.Substring(0, str.Length - 1); // 한자리 삭제..
        //        build.Remove(build.Length - 1, 1);
        //    }
        //    else
        //    {
        //        str += e.Text;
        //        build.Append(e.Text);
        //    }
        //    Content = null;
        //    Content = build;
        //}
        #endregion

        #region TextBlock RunOnMouseDown 이벤트
        public void RunOnMouseDown(object sender, MouseButtonEventArgs args)
        {
            Run run = sender as Run;

            if (args.ChangedButton == MouseButton.Left)
            {
                run.FontStyle = run.FontStyle == FontStyles.Italic ? FontStyles.Normal : FontStyles.Italic;
            }

            if (args.ChangedButton == MouseButton.Right)
            {
                run.FontWeight = run.FontWeight == FontWeights.Bold ? FontWeights.Normal : FontWeights.Bold;
            }
        }
        #endregion
    }
}
