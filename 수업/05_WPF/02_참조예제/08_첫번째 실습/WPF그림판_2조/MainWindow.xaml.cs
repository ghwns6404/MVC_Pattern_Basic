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

namespace WPF그림판
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        string strMode;
        Color col;

        private byte[] Pixels = new byte[4];
        Rectangle curRect;
        Ellipse curElls;

        Point prePos;
        bool a = false;


        public MainWindow()
        {
            InitializeComponent();

            radioBtn_Free.IsChecked = true;
            strMode = (string)radioBtn_Free.Content;
            label_Shape.Content = strMode;

            col = new Color();
            col.A = 0;
            col.B = 255;
            col.G = 255;
            col.R = 255;

            this.inkCanvas.MouseLeftButtonDown += new MouseButtonEventHandler(inkCanvas_MouseLeftButtonDown);
            this.inkCanvas.MouseLeftButtonUp += new MouseButtonEventHandler(inkCanvas_MouseLeftButtonUp);
            this.inkCanvas.MouseMove += new MouseEventHandler(inkCanvas_MouseMove);
        }

        // 해당 객체를 이미지로 변환
        private static RenderTargetBitmap ConverterBitmapImage(FrameworkElement element)
        {
            DrawingVisual drawingVisual = new DrawingVisual();
            DrawingContext drawingContext = drawingVisual.RenderOpen();

            // 해당 객체의 그래픽요소로 사각형의 그림을 그립니다.
            drawingContext.DrawRectangle(new VisualBrush(element), null,
                new Rect(new Point(0, 0), new Point(element.ActualWidth, element.ActualHeight)));

            drawingContext.Close();

            // 비트맵으로 변환합니다.
            RenderTargetBitmap target =
                new RenderTargetBitmap((int)element.ActualWidth, (int)element.ActualHeight,
                96, 96, System.Windows.Media.PixelFormats.Pbgra32);

            target.Render(drawingVisual);
            return target;
        }
        
        private void ImageSaves(BitmapSource source)
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
        // 픽셀 값 얻어오기
        private Color GetPixelColor(Point CurrentPoint)
        {
            BitmapSource CurrentSource = colorsImage.Source as BitmapSource;

            // 비트맵 내의 좌표 값 계산
            CurrentPoint.X *= CurrentSource.PixelWidth / colorsImage.ActualWidth;
            CurrentPoint.Y *= CurrentSource.PixelHeight / colorsImage.ActualHeight;

            if (CurrentSource.Format == PixelFormats.Bgra32 || CurrentSource.Format == PixelFormats.Bgr32)
            {
                // 32bit stride = (width * bpp + 7) /8
                int Stride = (CurrentSource.PixelWidth * CurrentSource.Format.BitsPerPixel + 7) / 8;
                // 한 픽셀 복사
                CurrentSource.CopyPixels(new Int32Rect((int)CurrentPoint.X, (int)CurrentPoint.Y, 1, 1), Pixels, Stride, 0);

                // 컬러로 변환 후 리턴
                return Color.FromArgb(Pixels[3], Pixels[2], Pixels[1], Pixels[0]);
            }
            else
            {
                MessageBox.Show("지원되지 않는 포맷형식");
            }

            return Color.FromArgb(Pixels[3], Pixels[2], Pixels[1], Pixels[0]);
        }

        private void SaveBtn_Click(object sender, RoutedEventArgs e)
        {
            RenderTargetBitmap bitmap = ConverterBitmapImage(inkCanvas);
            ImageSaves(bitmap);
        }

        private void OpenBtn_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openDialog = new OpenFileDialog();

            openDialog.Filter = "PNG|*.png|JPG|*.jpg|GIF|*.gif|BMP|*.bmp";
            openDialog.AddExtension = true;

            if (openDialog.ShowDialog() == true)
            {
                if (File.Exists(openDialog.FileName))
                {
                    BitmapImage bitmapImage = new BitmapImage(new Uri(openDialog.FileName,
                        UriKind.RelativeOrAbsolute));

                    // InkCanvas의 배경으로 지정
                    inkCanvas.Background = new ImageBrush(bitmapImage);
                }
            }
        }
        private void btn_Clear_Click(object sender, RoutedEventArgs e)
        {
            inkCanvas.Strokes.Clear();
            inkCanvas.Children.Clear();
        }

        private void radioButton_Click(object sender, RoutedEventArgs e)
        {
            RadioButton rd = sender as RadioButton;
            strMode = (string)rd.Content;

            label_Shape.Content = strMode;

            switch (strMode)
            {
                case "원": inkCanvas.EditingMode = InkCanvasEditingMode.None; break;
                case "사각형": inkCanvas.EditingMode = InkCanvasEditingMode.None; break;
                case "자유곡선": inkCanvas.EditingMode = InkCanvasEditingMode.Ink; break;
                case "지우기": inkCanvas.EditingMode = InkCanvasEditingMode.EraseByPoint; break;
            }
        }

        private void inkCanvas_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {            
            this.inkCanvas.ReleaseMouseCapture();

            if (a)
            {
                switch (strMode)
                {
                    case "원": SetEllipseProperty();
                                curElls = null;
                                break;

                    case "사각형": SetRectangleProperty();
                                    curRect = null;
                                    break;
                }
            }
            a = false;
        }

        private void SetRectangleProperty()
        {
            //사각형의 투명도를 100% 로 설정
            curRect.Opacity = 1;
            //사각형의 색상을 지정
            curRect.Fill = new SolidColorBrush(col);
            //사각형의 테두리를 선으로 지정
            curRect.StrokeDashArray = new DoubleCollection(); ;
        }

        private void SetEllipseProperty()
        {
            //원의 투명도를 100% 로 설정
            curElls.Opacity = 1;
            //원의 색상을 지정
            curElls.Fill = new SolidColorBrush(col);
            //원의 테두리를 선으로 지정
            curElls.StrokeDashArray = new DoubleCollection(); ;
        }

        private void inkCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            Point curPos = e.GetPosition(this.inkCanvas);

            label_Point.Content = string.Format("{0} : {1}", e.GetPosition(inkCanvas).X, e.GetPosition(inkCanvas).Y);

            if (e.MouseDevice.LeftButton == MouseButtonState.Pressed)
            {
                switch (strMode)
                {
                    case "원":    DrawCircle(curPos);      break;
                    case "사각형": DrawRectangle(curPos); break;
                }
            }
        }

        private void inkCanvas_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            prePos = e.GetPosition(this.inkCanvas);
            
            this.inkCanvas.CaptureMouse();
            switch (strMode)
            {
                case "원": CreateCircle(); break;
                case "사각형": CreateRectangle(); break;
            }
        }

        void CreateCircle()
        {
            if (curElls == null)
            {
                curElls = new Ellipse();
                curElls.Stroke = new SolidColorBrush(col);
                curElls.StrokeThickness = 2;
                curElls.Opacity = 0.7;

                DoubleCollection dashSize = new DoubleCollection();
                //dashSize.Add(1);
                dashSize.Add(1);
                curElls.StrokeDashArray = dashSize;
                
                //원의 정렬 기준을 설정한다.
                curElls.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
                curElls.VerticalAlignment = System.Windows.VerticalAlignment.Top;

                //그리드에 추가한다.
                this.inkCanvas.Children.Add(curElls);
                a = true;
            }
        }

        void CreateRectangle()
        {
            if (curRect == null)
            {
                curRect = new Rectangle();
                curRect.Stroke = new SolidColorBrush(col);
                curRect.StrokeThickness = 2;
                curRect.Opacity = 0.7;
                
                DoubleCollection dashSize = new DoubleCollection();
                dashSize.Add(1);
                dashSize.Add(1);
                curRect.StrokeDashArray = dashSize;
                
                //사각형의 정렬 기준을 설정한다.
                curRect.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
                curRect.VerticalAlignment = System.Windows.VerticalAlignment.Top;

                //그리드에 추가한다.
                this.inkCanvas.Children.Add(curRect);
                a = true;
            }
        }

        void DrawCircle(Point curPos)
        {
            if (curElls != null)
            {
                //사각형이 나타날 기준점을 설정한다.
                double left = prePos.X;
                double top = prePos.Y;

                //마우스의 위치에 따라 적절히 기준점을 변경한다.
                if (prePos.X > curPos.X)
                {
                    left = curPos.X;
                }
                if (prePos.Y > curPos.Y)
                {
                    top = curPos.Y;
                }

                //타원의 위치 기준점(Margin)을 설정한다
                curElls.Margin = new Thickness(left, top, 0, 0);
                //타원의 크기를 설정한다. 음수가 나올 수 없으므로 절대값을 취해준다.
                curElls.Width = Math.Abs(prePos.X - curPos.X);
                curElls.Height = Math.Abs(prePos.Y - curPos.Y);
            }
        }

        void DrawRectangle(Point curPos)
        {
            if (curRect != null)
            {
                //사각형이 나타날 기준점을 설정한다.
                double left = prePos.X;
                double top = prePos.Y;

                //마우스의 위치에 따라 적절히 기준점을 변경한다.
                if (prePos.X > curPos.X)
                {
                    left = curPos.X;
                }
                if (prePos.Y > curPos.Y)
                {
                    top = curPos.Y;
                }

                //사각형의 위치 기준점(Margin)을 설정한다
                curRect.Margin = new Thickness(left, top, 0, 0);
                //사각형의 크기를 설정한다. 음수가 나올 수 없으므로 절대값을 취해준다.
                curRect.Width = Math.Abs(prePos.X - curPos.X);
                curRect.Height = Math.Abs(prePos.Y - curPos.Y);
            }
        }

         private void colorsImage_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Point point = e.GetPosition(sender as Image);

            inkCanvas.DefaultDrawingAttributes.Color = col = GetPixelColor(point);
            
        }

         private void inkCanvas_Gesture(object sender, InkCanvasGestureEventArgs e)
         {

         }

         private void radioBtn_Circle_Checked(object sender, RoutedEventArgs e)
         {

         }

         private void radioBtn_Free_Checked(object sender, RoutedEventArgs e)
         {

         }
    }
}
