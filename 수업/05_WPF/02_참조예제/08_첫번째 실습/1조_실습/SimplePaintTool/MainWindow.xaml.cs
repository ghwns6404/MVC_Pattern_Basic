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
using Microsoft.Win32;

namespace SimplePaintTool
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        bool drawing = false;
        Point newpoint;
        Point oldpoint;
        int sel = 0;
        Color color = new Color();

        enum Shape { Rect = 0, Ellipse };

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            RectText.Text = "0";
            EllipseText.Text = "0";
            color = Color.FromRgb(0, 0, 0);

        }
       
        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (drawing == false)
            {
                newpoint = e.GetPosition(canvas1);
                drawing = true;
            }
        }

        private void Window_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (drawing == true && sel == (int)Shape.Rect)
            {
                SolidColorBrush brush = new SolidColorBrush(color);
                Rectangle rect = new Rectangle();
                rect.Height = Math.Abs(oldpoint.X - newpoint.X);
                rect.Width = Math.Abs(oldpoint.Y - newpoint.Y);     
                
                rect.Stroke = brush;
                canvas1.Children.Add(rect);
                Canvas.SetLeft(rect, newpoint.X);
                Canvas.SetTop(rect, newpoint.Y);
                
                drawing = false;
                int text = int.Parse(RectText.Text);
                text++;
                RectText.Text = text.ToString(); 
                
            }
            else if (drawing == true && sel == (int)Shape.Ellipse)
            {
                SolidColorBrush brush = new SolidColorBrush(color);
                Ellipse rect = new Ellipse();
                rect.Height = Math.Abs(oldpoint.X - newpoint.X);
                rect.Width = Math.Abs(oldpoint.Y - newpoint.Y);     

                rect.Stroke = brush;
                canvas1.Children.Add(rect);
                Canvas.SetLeft(rect, newpoint.X);
                Canvas.SetTop(rect, newpoint.Y);

                drawing = false;
             
                int text = int.Parse(EllipseText.Text);
                text++;
                EllipseText.Text = text.ToString(); 
            }
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
             if (drawing == true && sel == (int)Shape.Rect)
            {
                oldpoint = e.GetPosition(canvas1);
                
                
            }
            else if (drawing == true && sel == (int)Shape.Ellipse)
            {
                oldpoint = e.GetPosition(canvas1);
  
            }
            Point point = e.GetPosition(canvas1);
            string temp = string.Format("{0} : {1}", point.X, point.Y);
            XYText.Text = temp;
            
        }

        private void RectBtn_Click(object sender, RoutedEventArgs e)
        {
            sel = (int)Shape.Rect;
        }

        private void ElliseBtn_Click(object sender, RoutedEventArgs e)
        {
            sel = (int)Shape.Ellipse;
        }

        private void scrollBarB_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int temp = Convert.ToInt32(e.NewValue);
            color.B = byte.Parse(temp.ToString());
        }

        private void scrollBarG_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int temp = Convert.ToInt32(e.NewValue);
            color.G = byte.Parse(temp.ToString());
        }

        private void scrollBarR_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int temp = Convert.ToInt32(e.NewValue);
            color.R = byte.Parse(temp.ToString());
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //try
            //{
            //    Directory.CreateDirectory(System.IO.Path.GetDirectoryName(strFileName));
            //    UIElement ele = canvas1.Children[1];
            //    canvas1.
            //    File.WriteAllBytes(strFileName, txtbox.Text);

            //}
            //catch (Exception exc)
            //{
            //    MessageBoxResult result =
            //        MessageBox.Show("File could not be saved: " + exc.Message +
            //                        "\nClose program anyway?", Title,
            //                        MessageBoxButton.YesNo,
            //                        MessageBoxImage.Exclamation);

            //    args.Cancel = (result == MessageBoxResult.No);
            //}
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            RenderTargetBitmap bitmap = ConverterBitmapImage(canvas1);
            ImageSave(bitmap);
        }
        // 해당 객체를 이미지로 변환
        private static RenderTargetBitmap ConverterBitmapImage(FrameworkElement element)
        {
            DrawingVisual drawingVisual = new DrawingVisual();
            DrawingContext drawingContext = drawingVisual.RenderOpen();

            // 해당 객체의 그래픽요소로 사각형의 그림을 그립니다.
            drawingContext.DrawRectangle(null, null,
            new Rect(new Point(0, 0), new Point(element.ActualWidth, element.ActualHeight)));

            drawingContext.Close();

            // 비트맵으로 변환합니다.
            RenderTargetBitmap target =
                new RenderTargetBitmap((int)element.ActualWidth, (int)element.ActualHeight,
                96, 96, System.Windows.Media.PixelFormats.Pbgra32);

            target.Render(drawingVisual);
            return target;
        }
        // 해당 이미지 저장
        private static void ImageSave(BitmapSource source)
        {
            SaveFileDialog saveDialog = new SaveFileDialog();

            // 이미지 포맷들
            saveDialog.Filter = "PNG|*.png|JPG|*.jpg|GIF|*.gif|BMP|*.bmp";
            saveDialog.AddExtension = true;

            if (saveDialog.ShowDialog() == true)
            {
                BitmapEncoder encoder = null;
                // 파일 생성
                FileStream stream = new FileStream(saveDialog.FileName, FileMode.Create, FileAccess.Write);

                // 파일 포맷
                string upper = saveDialog.SafeFileName.ToUpper();
                char[] format = upper.ToCharArray(saveDialog.SafeFileName.Length - 3, 3);
                upper = new string(format);

                // 해당 포맷에 맞게 인코더 생성
                switch (upper.ToString())
                {
                    case "PNG":
                        encoder = new PngBitmapEncoder();
                        break;

                    case "JPG":
                        encoder = new JpegBitmapEncoder();
                        break;

                    case "GIF":
                        encoder = new GifBitmapEncoder();
                        break;

                    case "BMP":
                        encoder = new BmpBitmapEncoder();
                        break;
                }

                // 인코더 프레임에 이미지 추가
                encoder.Frames.Add(BitmapFrame.Create(source));
                // 파일에 저장
                encoder.Save(stream);

                stream.Close();
            }
        }
    }
}
