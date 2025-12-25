//-----------------------------------------------------
// HybridClockPage.xaml.cs (c) 2006 by Charles Petzold
//-----------------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Petzold.HybridClock
{
    public partial class HybridClockPage : Page
    {
        // XAML 엑세스를 위해 컬러를 초기화.
        public static readonly Color clrBackground = Colors.Aqua;
        public static readonly Brush brushBackground = Brushes.Aqua;

        // 시계 눈금을 위한 변환을 저장.
        TranslateTransform[] xform = new TranslateTransform[60];

        public HybridClockPage()
        {
            InitializeComponent();

            // Storyboard의 BeginTime 프로퍼티를 설정.
            storyboard.BeginTime = -DateTime.Now.TimeOfDay;

            // context menu Opened 이벤트에 대한 핸들러 설정.
            ContextMenu menu = new ContextMenu();
            menu.Opened += ContextMenuOnOpened;
            ContextMenu = menu;
            
            // Loaded 이벤트에 대한 핸들러를 설정.
            Loaded += WindowOnLoaded;
        }

        void WindowOnLoaded(object sender, EventArgs args)
        {
            // 시계 주위에 눈금 생성.
            for (int i = 0; i < 60; i++)
            {
                Ellipse elips = new Ellipse();
                elips.HorizontalAlignment = HorizontalAlignment.Center;
                elips.VerticalAlignment = VerticalAlignment.Center;
                elips.Fill = Brushes.Blue;
                elips.Width = 
                elips.Height = i % 5 == 0 ? 6 : 2;

                TransformGroup group = new TransformGroup();
                group.Children.Add(xform[i] = 
                    new TranslateTransform(datetime.ActualWidth, 0));
                group.Children.Add(
                    new TranslateTransform(grd.Margin.Left / 2, 0));
                group.Children.Add(
                    new TranslateTransform(-elips.Width / 2, -elips.Height /2));
                group.Children.Add(
                    new RotateTransform(i * 6));
                group.Children.Add(
                    new TranslateTransform(elips.Width / 2, elips.Height /2));

                elips.RenderTransform = group;
                grd.Children.Add(elips);
            }
            // 투명 마스크 생성.
            MakeMask();

            // 날짜/시간 문자열의 크기가 바뀌는 이벤트에 대한 핸들러를 설정.
            datetime.SizeChanged += DateTimeOnSizeChanged;
        }
        // 날짜/시간 문자열이 바뀔 경우 : 변환과 마스크를 새로 계산.
        void DateTimeOnSizeChanged(object sender, SizeChangedEventArgs args)
        {
            if (args.WidthChanged)
            {
                for (int i = 0; i < 60; i++)
                    xform[i].X = datetime.ActualWidth;

               MakeMask();
            }
        }
        // 투명 마스크 생성.
        void MakeMask()
        {
            DrawingGroup group = new DrawingGroup();
            Point ptCenter = new Point(datetime.ActualWidth + grd.Margin.Left, 
                                       datetime.ActualWidth + grd.Margin.Left);

            // 원 주위에 있는 256개의 쇄기 모양을 계산.
            for (int i = 0; i < 256; i++)
            {
                Point ptInner1 = 
                    new Point(ptCenter.X + datetime.ActualWidth * 
                                    Math.Cos(i * 2 * Math.PI / 256),
                              ptCenter.Y + datetime.ActualWidth * 
                                    Math.Sin(i * 2 * Math.PI / 256));

                Point ptInner2 = 
                    new Point(ptCenter.X + datetime.ActualWidth * 
                                    Math.Cos((i + 2) * 2 * Math.PI / 256),
                              ptCenter.Y + datetime.ActualWidth * 
                                    Math.Sin((i + 2) * 2 * Math.PI / 256));

                Point ptOuter1 = 
                    new Point(ptCenter.X + 
                                (datetime.ActualWidth + grd.Margin.Left) * 
                                    Math.Cos(i * 2 * Math.PI / 256),
                              ptCenter.Y + 
                                (datetime.ActualWidth + grd.Margin.Left) * 
                                    Math.Sin(i * 2 * Math.PI / 256));

                Point ptOuter2 = 
                    new Point(ptCenter.X + 
                                (datetime.ActualWidth + grd.Margin.Left) * 
                                    Math.Cos((i + 2) * 2 * Math.PI / 256),
                              ptCenter.Y + 
                                (datetime.ActualWidth + grd.Margin.Left) * 
                                    Math.Sin((i + 2) * 2 * Math.PI / 256));

                PathSegmentCollection segcoll = new PathSegmentCollection();
                segcoll.Add(new LineSegment(ptInner2, false));
                segcoll.Add(new LineSegment(ptOuter2, false));
                segcoll.Add(new LineSegment(ptOuter1, false));
                segcoll.Add(new LineSegment(ptInner1, false));

                PathFigure fig = new PathFigure(ptInner1, segcoll, true);

                PathFigureCollection figcoll = new PathFigureCollection();
                figcoll.Add(fig);

                PathGeometry path = new PathGeometry(figcoll);
                byte byOpacity = (byte)Math.Min(255, 512 - 2 * i);

                SolidColorBrush br = new SolidColorBrush(
                    Color.FromArgb(byOpacity, clrBackground.R, 
                                   clrBackground.G, clrBackground.B));

                GeometryDrawing draw = 
                    new GeometryDrawing(br, new Pen(br, 2), path);
                group.Children.Add(draw);
            }

            DrawingBrush brush = new DrawingBrush(group);
            mask.Fill = brush;
        }

        // 날짜/시간 포맷을 위한 컴텍스트 메뉴를 초기화.
        void ContextMenuOnOpened(object sender, RoutedEventArgs args)
        {
            ContextMenu menu = sender as ContextMenu;
            menu.Items.Clear();

            string[] strFormats = { "d", "D", "f", "F", "g", "G", "M", 
                                    "R", "s", "t", "T", "u", "U", "Y" };

            foreach (string strFormat in strFormats)
            {
                MenuItem item = new MenuItem();
                item.Header = DateTime.Now.ToString(strFormat);
                item.Tag = strFormat;
                item.IsChecked = strFormat == 
                    (Resources["clock"] as ClockTicker).Format;
                item.Click += MenuItemOnClick;
                menu.Items.Add(item);
            }
        }
        // 컨텍스트 메뉴에서 선택된 항목을 처리.
        void MenuItemOnClick(object sender, RoutedEventArgs args)
        {
            MenuItem item = (sender as MenuItem);
            (Resources["clock"] as ClockTicker).Format = item.Tag as string;
        }
    }
}