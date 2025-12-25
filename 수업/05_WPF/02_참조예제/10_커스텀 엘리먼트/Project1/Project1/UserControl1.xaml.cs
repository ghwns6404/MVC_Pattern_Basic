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
using System.Globalization;

namespace Project1
{
    /// <summary>
    /// UserControl1.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class UserControl1 : UserControl
    {
        FormattedText formtxt;
        public static readonly DependencyProperty TextProperty;
        Point ptText = new Point(2, 2);

        static UserControl1()
        {
            //InitializeComponent();

            TextProperty =
               DependencyProperty.Register("Text", typeof(string),
                                           typeof(UserControl1),
                   new FrameworkPropertyMetadata(" ",
                           FrameworkPropertyMetadataOptions.AffectsMeasure));

        }

        public string Text
        {
            set { SetValue(TextProperty, value == null ? " " : value); }
            get { return (string)GetValue(TextProperty); }
        }

        // MeasureOverride called whenever the size of the button might change.
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            formtxt = new FormattedText(
                    Text, CultureInfo.CurrentCulture, FlowDirection,
                    new Typeface(FontFamily, FontStyle, FontWeight, FontStretch),
                    FontSize, Foreground);

            // Take account of Padding when calculating the size.
            Size sizeDesired = new Size(Math.Max(48, formtxt.Width) + 30,
                                                     formtxt.Height + 30);
            sizeDesired.Width += Padding.Left + Padding.Right;
            sizeDesired.Height += Padding.Top + Padding.Bottom;

            return sizeDesired;
        }

        // OnRender called to redraw the button.
        protected override void OnRender(DrawingContext dc)
        {
            Brush brushBackground = SystemColors.ControlBrush;
            // Determine pen width.

            Pen pen = new Pen(Foreground, IsMouseOver ? 2 : 1);
            dc.DrawRoundedRectangle(brushBackground, pen,
                                   new Rect(new Point(0, 0), RenderSize), 4, 4);
            dc.DrawText(formtxt, ptText);
        }

        protected override void OnMouseWheel(MouseWheelEventArgs e)
        {
            base.OnMouseWheel(e);
            if (e.Delta > 0)        // 120
                ptText.Y = ptText.Y + 15;
            else if (e.Delta < 0)       // - 120
                ptText.Y = ptText.Y - 15;

            InvalidateVisual();     // OnRender 이벤트 
            
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            base.OnMouseDown(e);
            MessageBox.Show("BBBBBB");
        }
    }
}
