using System;
using System.IO;
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
using Microsoft.Win32;

namespace Paint
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        Point prePosition; 

        Rectangle currentRect;
        Ellipse currentElli;

        Color col = new Color();
        Color backCol = new Color();

        int type = 0;

        bool check = false;
        // ==============================================================================
        #region 생성자...
        public MainWindow()
        {
            InitializeComponent();

            col = Colors.Black;
            backCol = Colors.White;

            this.canvas1.MouseLeftButtonDown += new MouseButtonEventHandler(canvas1_MouseLeftButtonDown);
            this.canvas1.MouseMove += new MouseEventHandler(canvas1_MouseMove);
            this.canvas1.MouseLeftButtonUp += new MouseButtonEventHandler(canvas1_MouseLeftButtonUp);
            //this.stackPanel6.MouseEnter += new MouseEventHandler(stackPanel4_MouseEnter);

        }
        #endregion
        // ==============================================================================
        #region 마우스 이벤트...
        private void canvas1_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            prePosition = e.GetPosition(this.canvas1);
            this.canvas1.CaptureMouse();

            if (type == 1)
            {        
                if (currentRect == null)
                {            
                    CreteRectangle();
                    check = true;
                }
            }
            else if (type == 2)
            {      
                if (currentElli == null)
                {           
                    CreteEllipse();
                    check = true;
                }
            }
        }

        private void canvas1_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (check == true && canvas1.EditingMode == InkCanvasEditingMode.None)
            {        
                this.canvas1.ReleaseMouseCapture();
                SetRectangleProperty();
                currentRect = null;
                currentElli = null;
                check = false;
            }
        }

        private void canvas1_MouseMove(object sender, MouseEventArgs e)
        {
            Point currnetPosition = e.GetPosition(this.canvas1);

            this.ptX.Text = string.Format("{0}", currnetPosition.X);
            this.ptY.Text = string.Format("{0}", currnetPosition.Y);

            if (e.MouseDevice.LeftButton == MouseButtonState.Pressed)
            {
                if (currentRect != null || currentElli != null)
                {
                    double left = prePosition.X;
                    double top = prePosition.Y;

                    if (prePosition.X > currnetPosition.X)
                    {
                        left = currnetPosition.X;
                    }
                    if (prePosition.Y > currnetPosition.Y)
                    {
                        top = currnetPosition.Y;
                    }

                    if (type == 1)
                    {
                        currentRect.Margin = new Thickness(left, top, 0, 0);
                        currentRect.Width = Math.Abs(prePosition.X - currnetPosition.X);
                        currentRect.Height = Math.Abs(prePosition.Y - currnetPosition.Y);
                    }
                    else if (type == 2)
                    {
                        currentElli.Margin = new Thickness(left, top, 0, 0);
                        currentElli.Width = Math.Abs(prePosition.X - currnetPosition.X);
                        currentElli.Height = Math.Abs(prePosition.Y - currnetPosition.Y);
                    }
                }
            }
        }
        #endregion

        #region 도형 그리기...
        private void CreteRectangle()
        {                     
            currentRect = new Rectangle();          
            currentRect.Stroke = new SolidColorBrush(col);
            currentRect.Fill = new SolidColorBrush(backCol);
            currentRect.StrokeThickness = 2;          
            currentRect.Opacity = 0.7;          
            DoubleCollection dashSize = new DoubleCollection();          
            dashSize.Add(1);                    
            currentRect.StrokeDashArray = dashSize;          
            currentRect.StrokeDashOffset = 0;          
            currentRect.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;          
            currentRect.VerticalAlignment = System.Windows.VerticalAlignment.Top;                   
            this.canvas1.Children.Add(currentRect);      
        }

        private void CreteEllipse()
        {
            currentElli = new Ellipse();
            currentElli.Stroke = new SolidColorBrush(col);
            currentElli.Fill = new SolidColorBrush(backCol);
            currentElli.StrokeThickness = 2;
            currentElli.Opacity = 0.7;         
            DoubleCollection dashSize = new DoubleCollection();
            dashSize.Add(1);
            currentElli.StrokeDashArray = dashSize;
            currentElli.StrokeDashOffset = 0;       
            currentElli.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
            currentElli.VerticalAlignment = System.Windows.VerticalAlignment.Top;       
            this.canvas1.Children.Add(currentElli);
        }
        
        private void SetRectangleProperty()      
        {
            if (type == 1)
            {       
                currentRect.Opacity = 1;       
                currentRect.Fill = new SolidColorBrush(backCol);         
                currentRect.StrokeDashArray = new DoubleCollection();
            }
            else if (type == 2)
            {        
                currentElli.Opacity = 1;      
                currentElli.Fill = new SolidColorBrush(backCol);        
                currentElli.StrokeDashArray = new DoubleCollection();
            }
        }
        #endregion
        // ==============================================================================
        #region 그리기 종류...
        private void button7_Click(object sender, RoutedEventArgs e)
        {
            if (type != 0)
            {
                canvas1.EditingMode = InkCanvasEditingMode.Ink;

                type = 0;

                this.textType.Text = string.Format("Pen");
            }
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            if (type != 1)
            {
                canvas1.EditingMode = InkCanvasEditingMode.None;

                type = 1;

                this.textType.Text = string.Format("Rectangle");
            }
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            if( type != 2)
            {
                 canvas1.EditingMode = InkCanvasEditingMode.None;

                type = 2;

                this.textType.Text = string.Format("Ellipse");
            }
        }
        #endregion
        // ==============================================================================
        #region 지우기...
        private void button8_Click(object sender, RoutedEventArgs e)
        {
            canvas1.Strokes.Clear();
            canvas1.Children.Clear();
        }
        #endregion
        // ==============================================================================
        #region 색 버튼...
        private void button3_Click(object sender, RoutedEventArgs e)
        {
            col = Colors.Red;
            canvas1.DefaultDrawingAttributes.Color = col;

            this.textPenColor.Text = string.Format("Red");
        }

        private void button4_Click(object sender, RoutedEventArgs e)
        {
            col = Colors.Blue;
            canvas1.DefaultDrawingAttributes.Color = col;

            this.textPenColor.Text = string.Format("Blue");
        }

        private void button5_Click(object sender, RoutedEventArgs e)
        {
            col = Colors.Green;
            canvas1.DefaultDrawingAttributes.Color = col;

            this.textPenColor.Text = string.Format("Green");
        }

        private void button6_Click(object sender, RoutedEventArgs e)
        {
            col = Colors.Black;
            canvas1.DefaultDrawingAttributes.Color = col;

            this.textPenColor.Text = string.Format("Black");
        }
        #endregion

        #region 배경 색 버튼
        private void button11_Click(object sender, RoutedEventArgs e)
        {
            backCol = Colors.Red;

            this.textBackColor.Text = string.Format("Red");
        }

        private void button12_Click(object sender, RoutedEventArgs e)
        {
            backCol = Colors.Blue;

            this.textBackColor.Text = string.Format("Blue");
        }

        private void button13_Click(object sender, RoutedEventArgs e)
        {
            backCol = Colors.Green;

            this.textBackColor.Text = string.Format("Green");
        }

        private void button14_Click(object sender, RoutedEventArgs e)
        {
            backCol = Colors.White;

            this.textBackColor.Text = string.Format("White");
        }
        #endregion
        // ===================================================================================
        #region 저장 & 불러오기...
        // 해당 객체를 이미지로 변환
        private static RenderTargetBitmap ConverterBitmapImage(FrameworkElement element)
        {
            DrawingVisual drawingVisual = new DrawingVisual();
            DrawingContext drawingContext = drawingVisual.RenderOpen();

            drawingContext.DrawRectangle(new VisualBrush(element), null,
                new Rect(new Point(0, 0), new Point(element.ActualWidth, element.ActualHeight)));

            drawingContext.Close();

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

            saveDialog.Filter = "PNG|*.png|JPG|*.jpg|GIF|*.gif|BMP|*.bmp";
            saveDialog.AddExtension = true;

            if (saveDialog.ShowDialog() == true)
            {
                BitmapEncoder encoder = null;
                FileStream stream = new FileStream(saveDialog.FileName, FileMode.Create, FileAccess.Write);

                string upper = saveDialog.SafeFileName.ToUpper();
                char[] format = upper.ToCharArray(saveDialog.SafeFileName.Length - 3, 3);
                upper = new string(format);

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

                encoder.Frames.Add(BitmapFrame.Create(source));
                encoder.Save(stream);
                stream.Close();
            }
        }

        // 저장
        private void button9_Click(object sender, RoutedEventArgs e)
        {
            RenderTargetBitmap bitmap = ConverterBitmapImage(canvas1);
            ImageSave(bitmap);
        }

        // 불러오기
        private void button10_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openDialog = new OpenFileDialog();

            if (openDialog.ShowDialog() == true)
            {
                if (File.Exists(openDialog.FileName))
                {
                    BitmapImage bitmapImage = new BitmapImage(new Uri(openDialog.FileName,
                        UriKind.RelativeOrAbsolute));

                    canvas1.Background = new ImageBrush(bitmapImage);
                }
            }
        }
        #endregion
        private void stackPanel4_MouseEnter(object sender, MouseEventArgs e)
        {
         //   MessageBox.Show("TEST");
        }
        private void stackPanel6_MouseEnter(object sender, MouseEventArgs e)
        {
            MessageBox.Show("TEST");
        }

        private void canvas2_MouseEnter(object sender, MouseEventArgs e)
        {
            MessageBox.Show("TEST");
        }

        private void canvas3_MouseEnter(object sender, MouseEventArgs e)
        {
            MessageBox.Show("TEST");
        }
        // ===================================================================================
    }
}
