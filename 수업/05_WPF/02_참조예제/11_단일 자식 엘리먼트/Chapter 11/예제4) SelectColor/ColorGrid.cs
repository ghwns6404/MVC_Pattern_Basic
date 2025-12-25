//------------------------------------------
// ColorGrid.cs (c) 2006 by Charles Petzold
//------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Petzold.SelectColor
{
    class ColorGrid : Control
    {
        // 행과 열의 수
        const int yNum = 5;
        const int xNum = 8;

        // 출력할 색상들
        string[,] strColors = new string[yNum, xNum]
        {
            { "Black", "Brown", "DarkGreen", "MidnightBlue", 
                "Navy", "DarkBlue", "Indigo", "DimGray" },
            { "DarkRed", "OrangeRed", "Olive", "Green", 
                "Teal", "Blue", "SlateGray", "Gray" },
            { "Red", "Orange", "YellowGreen", "SeaGreen", 
                "Aqua", "LightBlue", "Violet", "DarkGray" },
            { "Pink", "Gold", "Yellow", "Lime", 
                "Turquoise", "SkyBlue", "Plum", "LightGray" },
            { "LightPink", "Tan", "LightYellow", "LightGreen", 
                "LightCyan", "LightSkyBlue", "Lavender", "White" }
        };

        // ColorCell 객체 생성
        ColorCell[,] cells = new ColorCell[yNum, xNum];
        ColorCell cellSelected;             // 선택된 ColorCell
        ColorCell cellHighlighted;          // ColorCell 강조

        // 이 컨트롤을 구성할 엘리먼트
        Border bord;                    // 외곽선
        UniformGrid unigrid;            // 모든 표의 셀이 같은 그리드
        
        // 현재 선택된 색상
        Color clrSelected = Colors.Black;

        // Public "Changed" 이벤트
        public event EventHandler SelectedColorChanged;

        // Public 생성자
        public ColorGrid()
        {
            // 컨트롤의 Border 생성
            bord = new Border();
            bord.BorderBrush = SystemColors.ControlDarkDarkBrush;
            bord.BorderThickness = new Thickness(1);
            AddVisualChild(bord);           // 이벤트 라우팅에 필요
            AddLogicalChild(bord);

            // UniformGrid을 생성해 Border의 자식으로 지정
            unigrid = new UniformGrid();
            unigrid.Background = SystemColors.WindowBrush;
            unigrid.Columns = xNum;
            bord.Child = unigrid;

            // UniformGrid에 ColorCell 객체들을 채움
            for (int y = 0; y < yNum; y++)
            for (int x = 0; x < xNum; x++)
            {
                Color clr = (Color) typeof(Colors).
                    GetProperty(strColors[y, x]).GetValue(null, null);

                cells[y, x] = new ColorCell(clr);
                unigrid.Children.Add(cells[y, x]);

                // 첫번째 cellSelected은 검정색
                if (clr == SelectedColor)
                {
                    cellSelected = cells[y, x];
                    cells[y, x].IsSelected = true;
                }

                // 셀의 툴팁 설정
                ToolTip tip = new ToolTip();
                tip.Content = strColors[y, x];
                cells[y, x].ToolTip = tip;             
            }
        }

        // Public SelectedColor 프로퍼티. get만 지원( 유저가 임의로 값을 바꾸면 안되기 때문 )

        // 현재 선택된 Color 리턴, 
        public Color SelectedColor
        {
            get { return clrSelected; }
        }

        // VisualChildrenCount 오버라이딩
        protected override int VisualChildrenCount
        {
            get { return 1; }
        }

        // GetVisualChild 오버라이딩
        // Border 타입의 객체를 반환 -> Border 객체의 Child 프로퍼티에는 UniformGrid패널을 할당
        protected override Visual GetVisualChild(int index)
        {
            if (index > 0)
                throw new ArgumentOutOfRangeException("index");

            return bord;
        }
        

        // MeasureOverride 오버라이딩
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            bord.Measure(sizeAvailable);
            return bord.DesiredSize;
        }

        // ArrangeOverride 오버라이딩
        protected override Size ArrangeOverride(Size sizeFinal)
        {
            bord.Arrange(new Rect(new Point(0, 0), sizeFinal));
            return sizeFinal;
        }

        // 마우스 이벤트 처리
        protected override void OnMouseEnter(MouseEventArgs args)
        {
            base.OnMouseEnter(args);

            if (cellHighlighted != null)
            {
                cellHighlighted.IsHighlighted = false;
                cellHighlighted = null;
            }
        }
        protected override void OnMouseMove(MouseEventArgs args)
        {
            base.OnMouseMove(args);
            ColorCell cell = args.Source as ColorCell;

            if (cell != null)
            {
                if (cellHighlighted != null)
                    cellHighlighted.IsHighlighted = false;

                cellHighlighted = cell;
                cellHighlighted.IsHighlighted = true;
            }
        }
        protected override void OnMouseLeave(MouseEventArgs args)
        {
            base.OnMouseLeave(args);

            if (cellHighlighted != null)
            {
                cellHighlighted.IsHighlighted = false;
                cellHighlighted = null;
            }
        }
        protected override void OnMouseDown(MouseButtonEventArgs args)
        {
            base.OnMouseDown(args);

            ColorCell cell = args.Source as ColorCell;

            if (cell != null)
            {
                if (cellHighlighted != null)
                    cellHighlighted.IsSelected = false;

                cellHighlighted = cell;
                cellHighlighted.IsSelected = true;
            }
            Focus();
        }
        protected override void OnMouseUp(MouseButtonEventArgs args)
        {
            base.OnMouseUp(args);
            ColorCell cell = args.Source as ColorCell;

            if (cell != null)
            {
                if (cellSelected != null)
                    cellSelected.IsSelected = false;

                cellSelected = cell;
                cellSelected.IsSelected = true;

                clrSelected = (cellSelected.Brush as SolidColorBrush).Color;
                OnSelectedColorChanged(EventArgs.Empty);
            }
        }
        // 키보드 이벤트 처리
        protected override void OnGotKeyboardFocus(
                                    KeyboardFocusChangedEventArgs args)
        {
            base.OnGotKeyboardFocus(args);

            if (cellHighlighted == null)
            {
                if (cellSelected != null)
                    cellHighlighted = cellSelected;
                else
                    cellHighlighted = cells[0, 0];

                cellHighlighted.IsHighlighted = true;
            }
        }
        protected override void OnLostKeyboardFocus(
                                    KeyboardFocusChangedEventArgs args)
        {
            base.OnGotKeyboardFocus(args);

            if (cellHighlighted != null)
            {
                cellHighlighted.IsHighlighted = false;
                cellHighlighted = null;
            }
        }
        protected override void OnKeyDown(KeyEventArgs args)
        {
            base.OnKeyDown(args);

            int index = unigrid.Children.IndexOf(cellHighlighted);
            int y = index / xNum;
            int x = index % xNum;

            switch (args.Key)
            {
                case Key.Home:
                    y = 0;
                    x = 0; 
                    break;

                case Key.End: 
                    y = yNum - 1; 
                    x = xNum + 1; 
                    break;

                case Key.Down:
                    if ((y = (y + 1) % yNum) == 0)
                        x++;
                    break;

                case Key.Up:
                    if ((y = (y + yNum - 1) % yNum) == yNum - 1)
                        x--;
                    break;

                case Key.Right:
                    if ((x = (x + 1) % xNum) == 0)
                        y++;
                    break;

                case Key.Left:
                    if ((x = (x + xNum - 1) % xNum) == xNum - 1)
                        y--;
                    break;

                case Key.Enter:
                case Key.Space:
                    if (cellSelected != null)
                        cellSelected.IsSelected = false;

                    cellSelected = cellHighlighted;
                    cellSelected.IsSelected = true;
                    clrSelected = (cellSelected.Brush as SolidColorBrush).Color;
                    OnSelectedColorChanged(EventArgs.Empty);
                    break;

                default:
                    return;
            }
            if (x >= xNum || y >= yNum)
                MoveFocus(new TraversalRequest(
                                FocusNavigationDirection.Next));

            else if (x < 0 || y < 0)
                MoveFocus(new TraversalRequest(
                                FocusNavigationDirection.Previous));
            else
            {
                cellHighlighted.IsHighlighted = false;
                cellHighlighted = cells[y, x];
                cellHighlighted.IsHighlighted = true;
            }
            args.Handled = true;
        }
        // SelectedColorChanged 이벤트를 발생 시 Protected 메소드
        protected virtual void OnSelectedColorChanged(EventArgs args)
        {
            if (SelectedColorChanged != null)
                SelectedColorChanged(this, args);
        }
    }
}
