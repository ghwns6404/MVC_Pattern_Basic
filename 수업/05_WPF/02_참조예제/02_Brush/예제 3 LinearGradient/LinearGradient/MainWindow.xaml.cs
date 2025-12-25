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

namespace LinearGradient
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            
            #region 1)point을 이용한 그라디언트 표현하기

            LinearGradientBrush brush = new LinearGradientBrush(Colors.White, Colors.Black, new Point(0, 0), new Point(1, 1));

            #endregion


            #region 2)angle 를 통한 그라디언트 표현하기
            //int angle = 0;
            //LinearGradientBrush brush = new LinearGradientBrush(Colors.Black, Colors.White, angle);
            #endregion
            Background = brush;
        }
    }
}
