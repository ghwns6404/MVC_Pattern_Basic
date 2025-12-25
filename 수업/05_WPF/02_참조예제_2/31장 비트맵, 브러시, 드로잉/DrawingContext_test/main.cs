using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

namespace DrawingContext_test
{
    public class main : Window
    {
        RenderTargetBitmap renderbit;
        DrawingVisual drawvis;
        Image img;
        Rect rec;

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new main());
        }

        public main()
        {
            Title = "Drawing Text 예제";

            renderbit = new RenderTargetBitmap(700, 700, 96, 96, PixelFormats.Default);
            drawvis = new DrawingVisual();

            Grid mainGrd = new Grid();
            Grid ButtonGrd = new Grid();
            ButtonGrd.VerticalAlignment = VerticalAlignment.Center;
            ButtonGrd.HorizontalAlignment = HorizontalAlignment.Center;

            mainGrd.RowDefinitions.Add(new RowDefinition());
            mainGrd.RowDefinitions.Add(new RowDefinition());

            mainGrd.ColumnDefinitions.Add(new ColumnDefinition());
            mainGrd.ColumnDefinitions.Add(new ColumnDefinition());
            
            ButtonGrd.RowDefinitions.Add(new RowDefinition());  // 사각형
            ButtonGrd.RowDefinitions.Add(new RowDefinition());  // 둥근 사각형
            ButtonGrd.RowDefinitions.Add(new RowDefinition());  // 원
            ButtonGrd.RowDefinitions.Add(new RowDefinition());  // 직선
            ButtonGrd.RowDefinitions.Add(new RowDefinition());  // 텍스트
            
            mainGrd.Children.Add(ButtonGrd);
            Grid.SetRow(ButtonGrd, 1);
            Grid.SetColumn(ButtonGrd, 1);

            img = new Image();
            renderbit.Render(drawvis);
            img.Source = renderbit;
            mainGrd.Children.Add(img);
            Grid.SetRow(img, 0);
            Grid.SetColumn(img, 0);

            Button BtnRect = new Button();
            BtnRect.Content = "사각형";
            BtnRect.TabIndex = 0;
            BtnRect.Width = 96;
            BtnRect.Height = 48;
            BtnRect.Click += new RoutedEventHandler(ButtonClick);
            ButtonGrd.Children.Add(BtnRect);
            Grid.SetRow(BtnRect, 0);

            Button BtnRoundRect = new Button();
            BtnRoundRect.Content = "둥근 사각형";
            BtnRoundRect.TabIndex = 1;
            BtnRoundRect.Width = 96;
            BtnRoundRect.Height = 48;
            BtnRoundRect.Click += new RoutedEventHandler(ButtonClick);
            ButtonGrd.Children.Add(BtnRoundRect);
            Grid.SetRow(BtnRoundRect, 1);

            Button BtnCircl = new Button();
            BtnCircl.Content = "원";
            BtnCircl.TabIndex = 2;
            BtnCircl.Width = 96;
            BtnCircl.Height = 48;
            BtnCircl.Click += new RoutedEventHandler(ButtonClick);
            ButtonGrd.Children.Add(BtnCircl);
            Grid.SetRow(BtnCircl, 2);

            Button BtnLine = new Button();
            BtnLine.Content = " 직선";
            BtnLine.TabIndex = 3;
            BtnLine.Width = 96;
            BtnLine.Height = 48;
            BtnLine.Click += new RoutedEventHandler(ButtonClick);
            ButtonGrd.Children.Add(BtnLine);
            Grid.SetRow(BtnLine, 3);

            Button BtnText = new Button();
            BtnText.Content = "텍스트";
            BtnText.TabIndex = 4;
            BtnText.Width = 96;
            BtnText.Height = 48;
            BtnText.Click += new RoutedEventHandler(ButtonClick);
            ButtonGrd.Children.Add(BtnText);
            Grid.SetRow(BtnText, 4);

            rec = new Rect(40, 100, 300, 300);

            Content = mainGrd;

        }

        public void ButtonClick(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;

            switch (btn.TabIndex)
            {
                case 0: Self_DrawRectangle();   break;
                case 1: Self_DrawRoundedRect(); break;
                case 2: Self_DrawCircle();      break;
                case 3: Self_DrawLine();        break;
                case 4: Self_DrawText();        break;
            }
        }

        public void Self_DrawRectangle()
        {
            DrawingContext dc = drawvis.RenderOpen();
            renderbit.Clear();

                dc.DrawRectangle(Brushes.AliceBlue, new Pen(Brushes.Green, 3), rec);


            dc.Close();
            renderbit.Render(drawvis);            
        }

        public void Self_DrawRoundedRect()
        {
            DrawingContext dc = drawvis.RenderOpen();
            renderbit.Clear();


                dc.DrawRoundedRectangle(Brushes.LightSkyBlue, new Pen(Brushes.Blue, 3), rec, 15, 15);


            dc.Close();
            renderbit.Render(drawvis);   

        }

        public void Self_DrawCircle()
        {
            DrawingContext dc = drawvis.RenderOpen();
            renderbit.Clear();
                
            dc.DrawEllipse(Brushes.Blue, new Pen(Brushes.Red, 3), new Point(200, 200), 150, 150);


            dc.Close();
            renderbit.Render(drawvis); 

        }

        public void Self_DrawLine()
        {
            DrawingContext dc = drawvis.RenderOpen();
            renderbit.Clear();

            // 선그리기
            Pen NewPen = new Pen(Brushes.Red, 5);
            Point point1 = new Point(50, 50);
            Point point2 = new Point(200, 400);
            dc.DrawLine(NewPen, point1, point2);

            dc.Close();
            renderbit.Render(drawvis); 
        }

        public void Self_DrawText()
        {
            DrawingContext dc = drawvis.RenderOpen();
            renderbit.Clear();

            // 글씨 쓰기
            string testString = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor";

            // Create the initial formatted text string.
            FormattedText formattedText = new FormattedText(
                testString,
                System.Globalization.CultureInfo.GetCultureInfo("en-us"),
                FlowDirection.LeftToRight,
                new Typeface("Verdana"),
                30,
                Brushes.Black);

            // Set a maximum width and height. If the text overflows these values, an ellipsis "..." appears.
            formattedText.MaxTextWidth = 300;
            formattedText.MaxTextHeight = 240;

            // Use a larger font size beginning at the first (zero-based) character and continuing for 5 characters.
            // The font size is calculated in terms of points -- not as device-independent pixels.
            formattedText.SetFontSize(36 * (96.0 / 72.0), 0, 5);

            // Use a Bold font weight beginning at the 6th character and continuing for 11 characters.
            formattedText.SetFontWeight(FontWeights.Bold, 2, 3);

            // Use a linear gradient brush beginning at the 6th character and continuing for 11 characters.
            formattedText.SetForegroundBrush(
                                    new LinearGradientBrush(
                                    Colors.Orange,
                                    Colors.Teal,
                                    90.0),
                                    6, 11);

            // Use an Italic font style beginning at the 28th character and continuing for 28 characters.
            formattedText.SetFontStyle(FontStyles.Italic, 28, 28);

            // Draw the formatted text string to the DrawingContext of the control.
            dc.DrawText(formattedText, new Point(100, 150));

            dc.Close();
            renderbit.Render(drawvis); 
        }
    }
}
