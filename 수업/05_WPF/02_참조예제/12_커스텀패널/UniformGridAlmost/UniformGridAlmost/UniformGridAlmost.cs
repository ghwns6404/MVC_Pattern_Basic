using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows.Media;

namespace UniformGridAlmost
{
    class UniformGridAlmost:Panel // 패널의 가장 간단한 타입.
    {
        //===========================================================
        //====public 읽기 전용 정적 의존 프로퍼티  설정================
       // public static readonly DependencyProperty ColumnsProperty;
        private int column;
        
        //정적 생성자에서 의존 프로퍼티를 등록
        static UniformGridAlmost()
        {
         //   ColumnsProperty = DependencyProperty.Register(
         //       "Colums",typeof(int),typeof(UniformGridAlmost),
         //       new FrameworkPropertyMetadata(1,FrameworkPropertyMetadataOptions.AffectsMeasure));
        }

        //Columns 프로퍼티
        public int Columns
        {
      //      set { SetValue(ColumnsProperty,value); }
        //    get { return (int)GetValue(ColumnsProperty); }
              set { column = value; }
              get { return column; }
        }

        //============================
        //=========================================================
        //읽기 전용 Rows 프로퍼티
        public int Rows
        {
            get { return (InternalChildren.Count + Columns - 1) / Columns; }
        }

        //===========================================
        //===Panel클래스에서 반드시 구현해야한다.
        //1)MeasureOverride   
        //2)ArrangeOverride   
        //===========================================



        //각 자식에 대해 사용 가능한 크기를 먼저 계산한다. (공간을 할당)
        protected override Size MeasureOverride(Size sizeAvailable)
        {
            //행과 열을 고려해서 자식의 크기를 계산
            //1)SizeAvailable은 어느 한쪽이나 양쪽 모두가 무한대가 될수 있기에..)
            Size sizeChild = new Size(sizeAvailable.Width/Columns,sizeAvailable.Height/Rows);

            //최대 폭과 높이를 누적시키기 위한 변수
            double maxwidth = 0;
            double maxheight = 0;

            foreach (UIElement child in InternalChildren)
            {
                //자식마다 Measure를 호출
                //2)행과 열을 동일하게 나눠줌
                //주석을 하게되면 ? ? ? 
                child.Measure(sizeChild);
              
                //DesiredSize란??===================================================================
                //DesiredSize 값은 특히 정렬 처리 단계에서 레이아웃 재정의 동작을 구현하는 경우에 유용합니다. 
                //시나리오에 따라 레이아웃 논리에서 DesiredSize를 그대로 적용하는 경우도 있고,
                //DesiredSize에 대해 제약 조건이 적용될 수도 있습니다. 
                //이러한 제약 조건으로 인해 레이아웃의 부모 또는 자식의 다른 특성이 변경될 수도 있습니다. 
                //=================================================================================

                //자식의 DesiredSize 프로퍼티를 이용 
                //DesiredSize 값은 Measure 호출의 일부로 레이아웃 시스템에서 설정됩니다.
                maxheight = Math.Max(maxheight,child.DesiredSize.Height);
                maxwidth = Math.Max(maxwidth,child.DesiredSize.Width);
            }
        
           //그리드 자체의 요구 크기를 계산
            return new Size(Columns * maxwidth, Rows * maxheight); 
        }


        //ArrangeOverride메소드를 토해 패널은 모든 자식을 그리드에 배치한다
        protected override Size ArrangeOverride(Size sizeFinal)
        {
           //행과 열을 고려해 자식의 크기 계산
            Size sizeChild = new Size(sizeFinal.Width/Columns,sizeFinal.Height /Rows );
            
            for (int index = 0; index < InternalChildren.Count; index++)
            {
                int row = index / Columns;
                int col = index % Columns;

                //SizeFinal 내에서 각 자식의 사각형을 계산
                Rect rectChild = new Rect(new Point (col*sizeChild.Width,row*sizeChild.Height),sizeChild);

                //자식에 대해 Arrange를 호출
                // 주석을 하게되면 ? ? ?
                InternalChildren[index].Arrange(rectChild );
                
            }
            return sizeFinal;
        }
    }
}
