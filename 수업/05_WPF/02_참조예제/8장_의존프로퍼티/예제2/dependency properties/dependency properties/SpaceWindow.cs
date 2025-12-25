//--------------------------------------------
// SpaceWindow.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.SetSpaceProperty
{
    
    public class SpaceWindow : Window
    {
        //윈도우 속성 
        // 의존프로퍼티와 public 프로퍼티
        public static readonly DependencyProperty SpaceProperty;

        public int Space
        {
            set
            {
                SetValue(SpaceProperty, value);
            }
            get
            {
                return (int)GetValue(SpaceProperty);
            }
        }

        // static(정적) 생성자
        static SpaceWindow()
        {
            // 메타데이터정의
            FrameworkPropertyMetadata metadata = new FrameworkPropertyMetadata();
            metadata.Inherits = true;           //엘리먼트 트리를 통해 계승가능

            // SpaceProperty에 소유자를 추가하고, 메타데이터를 오버라이딩(재정의)
            //이전에 등록된 의존 프로퍼티에 새로운 소유자를 추가할때 원래의 metadata가 적용되지 않으므로 반드시 그 자신의
            //metadata를 생성 해야 한다. 
            SpaceProperty =
                SpaceButton.SpaceProperty.AddOwner(typeof(SpaceWindow));
            SpaceProperty.OverrideMetadata(typeof(SpaceWindow), metadata);
        }
    }
}
