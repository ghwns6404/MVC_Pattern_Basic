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
using System.Reflection; // Brushes 클래스의 멤버를 얻기위해

namespace FlipThroughTheBrushes
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        int index = 0;
        PropertyInfo[] props;  //프로퍼티 정보를 얻기위한 PropertyInfo 배열
        
        public MainWindow()
        {
            props = typeof(Brushes).GetProperties(BindingFlags.Public | BindingFlags.Static); //Brushes 클래서의 프로퍼티를 얻온다.
            SetTitleAndBackGround();
            InitializeComponent();
        }

        private void SetTitleAndBackGround()
        {
            Title = "Flip Through the Brushes - " + props[index].Name;
            Background = (Brush)props[index].GetValue(null, null);          //실제 SolidColorBrush 객체를 반환한다.
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Down || e.Key == Key.Up)
            {
                index += e.Key == Key.Up ? 1 : props.Length - 1;
                index %= props.Length;
                SetTitleAndBackGround();
            }
        }
    }
}
