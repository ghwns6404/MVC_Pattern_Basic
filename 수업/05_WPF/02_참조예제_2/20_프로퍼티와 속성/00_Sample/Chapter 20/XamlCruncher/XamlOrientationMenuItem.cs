//--------------------------------------------------------
// XamlOrientationMenuItem.cs (c) 2006 by Charles Petzold
//--------------------------------------------------------
using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Petzold.XamlCruncher
{
    class XamlOrientationMenuItem : MenuItem
    {
        MenuItem itemChecked;
        Grid grid;
        TextBox txtbox;
        Frame frame;

        // Dock 타입의 Orientation public 프로퍼티
        public Dock Orientation
        {
            set
            {
                foreach (MenuItem item in Items)
                    if (item.IsChecked = (value == (Dock)item.Tag))
                        itemChecked = item;
            }
            get
            {
                return (Dock)itemChecked.Tag;
            }
        }

        // 생성자는 3개의 인자를 요구함
        public XamlOrientationMenuItem(Grid grid, TextBox txtbox, Frame frame)
        {
            this.grid = grid;
            this.txtbox = txtbox;
            this.frame = frame;

            Header = "_Orientation";

            for (int i = 0; i < 4; i++)
                Items.Add(CreateItem((Dock)i));

            (itemChecked = (MenuItem) Items[0]).IsChecked = true;
        }
        // Dock 설정상의 각 메뉴를 생성
        MenuItem CreateItem(Dock dock)
        {
            MenuItem item = new MenuItem();
            item.Tag = dock;
            item.Click += ItemOnClick;
            item.Checked += ItemOnCheck;

            // 메뉴에 보일 2개의 문자열
            FormattedText formtxt1 = CreateFormattedText("Edit");
            FormattedText formtxt2 = CreateFormattedText("Display");
            double widthMax = Math.Max(formtxt1.Width, formtxt2.Width);

            // DrawingVisual과 DrawingContext 생성
            DrawingVisual vis = new DrawingVisual();
            DrawingContext dc = vis.RenderOpen();

            // 박스로 둘러싸인 텍스트를 그림
            switch (dock)
            {
                case Dock.Left:         // 왼쪽에서 편집하고 오른쪽에서 보여줌
                    BoxText(dc, formtxt1, formtxt1.Width, new Point(0, 0));
                    BoxText(dc, formtxt2, formtxt2.Width, 
                            new Point(formtxt1.Width + 4, 0));
                    break;

                case Dock.Top:          // 위쪽에서 편집하고 아래쪽에서 보여줌
                    BoxText(dc, formtxt1, widthMax, new Point(0, 0));
                    BoxText(dc, formtxt2, widthMax, 
                            new Point(0, formtxt1.Height + 4));
                    break;

                case Dock.Right:        // 오른쪽에서 편집하고 왼쪽에서 보여줌
                    BoxText(dc, formtxt2, formtxt2.Width, new Point(0, 0));
                    BoxText(dc, formtxt1, formtxt1.Width, 
                            new Point(formtxt2.Width + 4, 0));
                    break;

                case Dock.Bottom:       // 아래쪽에서 편집하고 위쪽에서 보여줌
                    BoxText(dc, formtxt2, widthMax, new Point(0, 0));
                    BoxText(dc, formtxt1, widthMax, 
                            new Point(0, formtxt2.Height + 4));
                    break;
            }

            dc.Close();

            // visual의 Drawing을 바탕으로 해 Image 객체 생성
            DrawingImage drawimg = new DrawingImage(vis.Drawing);
            Image img = new Image();
            img.Source = drawimg;

            // 메뉴의 헤더를 Image 객체에 설정
            item.Header = img;

            return item;
        }
        // 귀찮은 FormattedText 인자 다루기
        FormattedText CreateFormattedText(string str)
        {
            return new FormattedText(str, CultureInfo.CurrentCulture, 
                FlowDirection.LeftToRight, 
                new Typeface(SystemFonts.MenuFontFamily, SystemFonts.MenuFontStyle, 
                             SystemFonts.MenuFontWeight, FontStretches.Normal),
                SystemFonts.MenuFontSize, SystemColors.MenuTextBrush);
        }
        // 사각으로 둘러싸인 텍스트를 그림
        void BoxText(DrawingContext dc, FormattedText formtxt, 
                                        double width, Point pt)
        {
            Pen pen = new Pen(SystemColors.MenuTextBrush, 1);

            dc.DrawRectangle(null, pen, 
                new Rect(pt.X, pt.Y, width + 4, formtxt.Height + 4));
            double X = pt.X + (width - formtxt.Width) / 2;
            dc.DrawText(formtxt, new Point(X + 2, pt.Y + 2));
        }
        // 클릭했을 떄 항목을 체크하거나 체크 해제함
        void ItemOnClick(object sender, RoutedEventArgs args)
        {
            itemChecked.IsChecked = false;
            itemChecked = args.Source as MenuItem;
            itemChecked.IsChecked = true;
        }
        // 체크된 항목에 따라 위치를 변경
        void ItemOnCheck(object sender, RoutedEventArgs args)
        {
            MenuItem itemChecked = args.Source as MenuItem;

            // 둘째와 셋째의 열과 행을 0으로 초기화
            for (int i = 1; i < 3; i++)
            {
                grid.RowDefinitions[i].Height = new GridLength(0);
                grid.ColumnDefinitions[i].Width = new GridLength(0);
            }

            // TextBox와 Frame 셀을 0으로 초기화
            Grid.SetRow(txtbox, 0);
            Grid.SetColumn(txtbox, 0);
            Grid.SetRow(frame, 0);
            Grid.SetColumn(frame, 0);

            // 열과 행을 위치 설정에 따라 설정
            switch ((Dock)itemChecked.Tag)
            {
                case Dock.Left:             // 왼쪽에서 편집하고 오른쪽에서 보여줌
                    grid.ColumnDefinitions[1].Width = GridLength.Auto;
                    grid.ColumnDefinitions[2].Width = 
                                new GridLength(100, GridUnitType.Star);
                    Grid.SetColumn(frame, 2);
                    break;

                case Dock.Top:              // 위쪽에서 편집하고 아래쪽에서 보여줌
                    grid.RowDefinitions[1].Height = GridLength.Auto;
                    grid.RowDefinitions[2].Height = 
                                new GridLength(100, GridUnitType.Star);
                    Grid.SetRow(frame, 2);
                    break;

                case Dock.Right:            // 오른쪽에서 편집하고 왼쪽에서 보여줌
                    grid.ColumnDefinitions[1].Width = GridLength.Auto;
                    grid.ColumnDefinitions[2].Width = 
                                new GridLength(100, GridUnitType.Star);
                    Grid.SetColumn(txtbox, 2);
                    break;

                case Dock.Bottom:           // 아래쪽에서 편집하고 위쪽에서 보여줌
                    grid.RowDefinitions[1].Height = GridLength.Auto;
                    grid.RowDefinitions[2].Height = 
                                new GridLength(100, GridUnitType.Star);
                    Grid.SetRow(txtbox, 2);
                    break;
            }
        }
    }
}
