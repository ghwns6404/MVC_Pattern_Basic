using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Petzold.SelectColorFromGrid
{
    class ColorGridBox:ListBox
    {
        string[] strColors =
        {
            "Black","Brown","DarkGreen","MidnightBlue","Navy","DarkBlue"
        };
        public ColorGridBox()           // 생성자에서 그리드 박스 생성.
        {
            FrameworkElementFactory factoryUnigrid = new FrameworkElementFactory(typeof(UniformGrid));
            factoryUnigrid.SetValue(UniformGrid.ColumnsProperty, 8);
            ItemsPanel = new ItemsPanelTemplate(factoryUnigrid);

            foreach (string strColor in strColors)
            {
                // Fill
                Rectangle rect = new Rectangle();
                rect.Width = 12;
                rect.Height = 12;
                rect.Margin = new Thickness(4);     // 여백설정
                rect.Fill = (Brush)typeof(Brushes).GetProperty(strColor).GetValue(null, null);
                Items.Add(rect);

                ToolTip tip = new ToolTip();
                tip.Content = strColor;
                rect.ToolTip = tip;
            }
            SelectedValuePath = "Fill";
        }
    }
}
