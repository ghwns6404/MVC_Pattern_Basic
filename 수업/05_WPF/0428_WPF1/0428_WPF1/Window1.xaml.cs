using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.ConstrainedExecution;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml.Linq;

namespace _0428_WPF1
{
    /// <summary>
    /// Window1.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Window1 : Window
    {
        List<Polygon> stars = new List<Polygon>();
        public Window1()
        {
            InitializeComponent();
            
        }
        

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            double x = Canvas.GetLeft(rect);
            double y = Canvas.GetTop(rect);

            if (up_button == (Button)sender)
            {
                Canvas.SetTop(rect, y = y - 10);
            }
            else if(down_button == (Button)sender)
            {
                Canvas.SetTop(rect, y = y + 10);
            }
            else if (left_button == (Button)sender)
            {
                Canvas.SetLeft(rect, x = x - 10);
            }
            else if (right_button == (Button)sender)
            {
                Canvas.SetLeft(rect, x = x + 10);
            }

        }
        bool isDrawing = false;
        private void canvas_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            isDrawing = true;
            DrawStar(e.GetPosition(canvas));
        }
        private void canvas_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            isDrawing = false;
        }
        private void DrawStar(Point position)
        {
            Polygon star = new Polygon();
            star.Fill = new SolidColorBrush(Colors.Yellow);
            star.Stroke = new SolidColorBrush(Colors.Yellow);
            star.StrokeThickness = 1;

            PointCollection points = new PointCollection();
            points.Add(new Point(0, 8));
            points.Add(new Point(8, 8));
            points.Add(new Point(10, 0));
            points.Add(new Point(12, 8));
            points.Add(new Point(20, 8));
            points.Add(new Point(14, 13));
            points.Add(new Point(16, 20));
            points.Add(new Point(10, 16));
            points.Add(new Point(4, 20));
            points.Add(new Point(6, 13));
            star.Points = points;

            Canvas.SetLeft(star, position.X);
            Canvas.SetTop(star, position.Y);

            canvas.Children.Add(star);
            stars.Add(star);
        }
        private void canvas_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDrawing && e.RightButton == MouseButtonState.Pressed)
            {
                DrawStar(e.GetPosition(canvas));
            }
        }
        private void UndoLastStar()
        {
            if (stars.Count > 0)
            {
                Polygon lastStar = stars[stars.Count - 1];
                canvas.Children.Remove(lastStar);
                stars.RemoveAt(stars.Count - 1);
            }
        }
        private void UndoLastStar_All()
        {
            foreach (var star in stars)
            {
                canvas.Children.Remove(star);
            }
            stars.Clear();
        }
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            UndoLastStar();
        }
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            UndoLastStar_All();
        }
        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            canvas.Children.Clear();
        }

        
    }

    public class Person
    {
        public string Name { get; set; }
        public string Phone { get; set; }
        public Person()
        {
            Name = "천호준";
            Phone = "010-1234-5678";
        }
        private void UpdateNameToUI()
        {
            //Name.Text = Name;
        }
        private void UpdatePhoneToUI()
        {
            //Phone.Text = Phone;
        }
    }
} 
    



