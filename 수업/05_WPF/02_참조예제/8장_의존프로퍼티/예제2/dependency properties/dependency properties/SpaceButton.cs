//--------------------------------------------
// SpaceButton.cs (c) 2006 by Charles Petzold
//--------------------------------------------
using System;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Petzold.SetSpaceProperty
{
    public class SpaceButton : Button
    {
        // 전통적 방식의 .Net private and public 프로퍼티
        string txt;
        public string Text
        {
            set
            {
                txt = value;
                // 컨텐츠에 SpaceOutText메서드 등록
                Content = SpaceOutText(txt);

            }
            get
            {
                // 텍스트 값 리턴
                return txt;
            }
        }

        // 의존프로퍼티와 public 프로퍼티
        public static readonly DependencyProperty SpaceProperty;

        public int Space
        {
            set
            {
                // SpaceProperty에 기반하여 Value 값을 평가해서 Set.
                SetValue(SpaceProperty, value);
            }
            get
            {
                // SpaceProperty에서 얻어온 값 리턴
                return (int)GetValue(SpaceProperty);
            }
        }

        // static(정적) 생성자
        static SpaceButton()
        {
            // 메타데이터 정의
            FrameworkPropertyMetadata metadata = new FrameworkPropertyMetadata();
            metadata.DefaultValue = 1;               //메타테이터의 기본값이 1
            metadata.AffectsMeasure = true;          //엘리먼트 크기에 영향을 주므로 true
            metadata.Inherits = true;                //엘리먼트 트리를 통해 계승가능           
            metadata.PropertyChangedCallback += OnSpacePropertyChanged;

            // 의존프로퍼티에 레지스트리 등록            
            SpaceProperty = DependencyProperty.Register("Space", typeof(int),
                                     typeof(SpaceButton), metadata, ValidateSpaceValue);
        }

        // 값 검증을 위한 콜백 메서드(이 조건에 맞아야지 Set할 수 있음)
        static bool ValidateSpaceValue(object obj)
        {
            int i = (int)obj;
            return i >= 0;
        }

        // 프로퍼티가 변경 됐을 때의 콜백 메서드
        static void OnSpacePropertyChanged(DependencyObject obj,
                                    DependencyPropertyChangedEventArgs args)
        {
            //obj가 SpaceButton이면 객체반환
            SpaceButton btn = obj as SpaceButton;
            //컨텐츠이벤트 등록
            btn.Content = btn.SpaceOutText(btn.txt);
        }

        // 텍스트에 빈칸을 넣어주는 메서드
        string SpaceOutText(string str)
        {
            if (str == null)
                return null;

            //StringBuilder 클래스는 내부적으로 문자열 버퍼를 가지고 문자열을 관리
            //Append, AppendFormat 메소드가 호출될 때 마다 내부 버퍼에 연결 하고자 하는 문자열을 복사
            //내부 버퍼는 System.String 타입
            StringBuilder build = new StringBuilder();

            foreach (char ch in str)
            {
                build.Append(ch + new string(' ', Space));
            }
            // 메서드 생성시 만들어진 문자열 리턴
            return build.ToString();
        }
    }
}
