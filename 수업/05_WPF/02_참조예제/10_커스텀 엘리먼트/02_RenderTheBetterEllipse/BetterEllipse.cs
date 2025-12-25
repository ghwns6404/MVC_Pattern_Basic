//----------------------------------------------
// BetterEllipse.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Globalization;

namespace Petzold.RenderTheBetterEllipse
{
    public class BetterEllipse : FrameworkElement
    {
        // 의존 프로퍼티
        public static readonly DependencyProperty FillProperty;
        public static readonly DependencyProperty StrokeProperty;

        // 의존 프로퍼티에 대한 Public 인터페이스
        public Brush Fill
        {
            set { SetValue(FillProperty, value); }
            get { return (Brush)GetValue(FillProperty); }
        }
        public Pen Stroke
        {
            set { SetValue(StrokeProperty, value); }
            get { return (Pen)GetValue(StrokeProperty); }
        }

        // 정적 생성자
        static BetterEllipse()
        {
            //의존 프로퍼티 등록?
            FillProperty =
                DependencyProperty.Register("Fill", //프로퍼티 이름
                        typeof(Brush),              //프로퍼티의 타입
                        typeof(BetterEllipse),      //프로퍼티의 부모타입(클래스 이름)
                                                    //프로퍼티의 메타데이터
                        new FrameworkPropertyMetadata(null, 
                            FrameworkPropertyMetadataOptions.AffectsRender)); //flag값
            StrokeProperty =
                DependencyProperty.Register("Stroke", typeof(Pen),
                        typeof(BetterEllipse), new FrameworkPropertyMetadata(null,
                                FrameworkPropertyMetadataOptions.AffectsMeasure));
        }

        // MeasureOverride의 오버라이딩
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            Size sizeDesired = base.MeasureOverride(sizeAvailable);

            if (Stroke != null)
                sizeDesired = new Size(Stroke.Thickness, Stroke.Thickness);

            return sizeDesired;
        }

        // OnRender의 오버라이딩
        protected override void OnRender(DrawingContext dc)
        {
            Size size = RenderSize;

            // 펜의 두께로 RenderSize 조정
            if (Stroke != null)
            {
                //Thickness프로퍼티 만큼 타원의 직경을 줄인다.
                size.Width = Math.Max(0, size.Width - Stroke.Thickness);
                size.Height = Math.Max(0, size.Height - Stroke.Thickness);
            }

            // 타원 그리기
            dc.DrawEllipse(Fill, Stroke,
                new Point(RenderSize.Width / 2, RenderSize.Height / 2),
                size.Width / 2, size.Height / 2);

            #region 추가적인 코드
            string testString = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor";

            // FormattedText 생성
            FormattedText formtxt = new FormattedText(
                testString, //문자열
                CultureInfo.GetCultureInfo("en-us"),//Text 특정 문화권    
                FlowDirection.LeftToRight,          //Text 읽는 방향
                new Typeface("Verdana"),            //Text 스타일
                32,                                 //글꼴 크기
                Brushes.Black);                     //브러쉬 색깔

            // Set a maximum width and height. If the text overflows these values, an ellipsis "..." appears.
            formtxt.MaxTextWidth = 300;             //Text 줄의 최대폭
            formtxt.MaxTextHeight = 240;            //Text 열의 최대높이

            // Use a larger font size beginning at the first (zero-based) character and continuing for 5 characters.
            // The font size is calculated in terms of points -- not as device-independent pixels.
            formtxt.SetFontSize(36 * (96.0 / 72.0), 0, 5);

            // 범위의 텍스트 색 변경 (색깔, 위치, 범위)
            formtxt.SetFontWeight(FontWeights.Bold, 6, 11);

            // 범위의 텍스트 그라데이션 주기
            formtxt.SetForegroundBrush(
                                    new LinearGradientBrush(
                                    Colors.Orange,
                                    Colors.Teal,
                                    90.0),                      //브러쉬(그라데이션)
                                    6, 11);                     //위치, 범위

            // 범위의 텍스트의 폰트스타일
            formtxt.SetFontStyle(FontStyles.Italic, 28, 28);

            //출력
            dc.DrawText(formtxt, new Point((RenderSize.Width - formtxt.Width) / 2,
                                            (RenderSize.Height - formtxt.Height) / 2));
            #endregion

            dc.PushClip(new RectangleGeometry(new Rect(new Point(0, 0), RenderSize)));
        }
    }
}