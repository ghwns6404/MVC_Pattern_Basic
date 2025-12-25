//----------------------------------------------------------
// ListColorsEvenElegantlier.cs (c) 2006 by Charles Petzold
//----------------------------------------------------------
using Petzold.ListNamedBrushes;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Petzold.ListColorsEvenElegantlier
{
    public class ListColorsEvenElegantlier : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new ListColorsEvenElegantlier());
        }
        public ListColorsEvenElegantlier()
        {
            Title = "List Colors Even Elegantlier";

            // 항목의 DataTemplate 생성
            DataTemplate template = new DataTemplate(typeof(NamedBrush));

            // 스택 패널 기반의 FrameworkElementFactory 생성
            FrameworkElementFactory factoryStack =
                                new FrameworkElementFactory(typeof(StackPanel));
            factoryStack.SetValue(StackPanel.OrientationProperty, 
                                                Orientation.Horizontal);

            // DataTemplate 비주얼 트리의 루트 생성
            template.VisualTree = factoryStack;

            // Rectangle 기반의 FrameworkElementFactory 생성
            FrameworkElementFactory factoryRectangle =
                                new FrameworkElementFactory(typeof(Rectangle));
            factoryRectangle.SetValue(Rectangle.WidthProperty, 16.0);
            factoryRectangle.SetValue(Rectangle.HeightProperty, 16.0);
            factoryRectangle.SetValue(Rectangle.MarginProperty, new Thickness(2));
            factoryRectangle.SetValue(Rectangle.StrokeProperty, 
                                            SystemColors.WindowTextBrush);
            factoryRectangle.SetBinding(Rectangle.FillProperty, 
                                            new Binding("Brush"));
            // StackPanel 에 factoryRectangle 객체 추가
            factoryStack.AppendChild(factoryRectangle);

            // TextBlock 기반의 FrameworkElementFactory 생성
            FrameworkElementFactory factoryTextBlock =
                                new FrameworkElementFactory(typeof(TextBlock));
            factoryTextBlock.SetValue(TextBlock.VerticalAlignmentProperty,
                                            VerticalAlignment.Center);
            factoryTextBlock.SetValue(TextBlock.TextProperty, 
                                            new Binding("Name"));
            // StackPanel 에 factoryTextBlock 객체 추가
            factoryStack.AppendChild(factoryTextBlock);

            // 윈도우 Content를 위한 리스트 박스 생성
            ListBox lstbox = new ListBox();
            lstbox.Width = 150;
            lstbox.Height = 150;
            Content = lstbox;

            // ItemTemplate 프로퍼티를 위해 생성한 template으로 설정
            lstbox.ItemTemplate = template;
   
            // ItemsSource를 NameBrush 객체 배열로 설정
            lstbox.ItemsSource = NamedBrush.All;

            // SelectedValue와 원도우 배경색을 바인딩
            lstbox.SelectedValuePath = "Brush";
            lstbox.SetBinding(ListBox.SelectedValueProperty, "Background");
            lstbox.DataContext = this;
        }
    }
}
