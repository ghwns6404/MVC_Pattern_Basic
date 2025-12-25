//----------------------------------------------
// NavigationBar.cs (c) 2006 by Charles Petzold
//----------------------------------------------
using System;
using System.Collections;                   // IList 관련
using System.Collections.Specialized;    // NotifyCollectionChangedEventArgs 관련
using System.ComponentModel;         // ICollectionView 관련
using System.Reflection;                    // ConstructorInfo 관련
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace Petzold.DataEntry
{
    public partial class NavigationBar : ToolBar
    {
        IList coll;
        ICollectionView collview;
        Type typeItem;

        // Public 생성자
        public NavigationBar()
        {
            InitializeComponent();
        }

        // Public 프로퍼티
        public IList Collection
        {
            set
            {
                coll = value;

                // CollectionView를 생성하고 이벤트 핸들러를 부여
                // 지정된 기본 뷰 객체 생성
                collview = CollectionViewSource.GetDefaultView(coll);
                // 뷰의 내용이 바뀔때마다 발생하는 이벤트 핸들러 등록
                collview.CurrentChanged += CollectionViewOnCurrentChanged;
                // 
                collview.CollectionChanged += CollectionViewOnCollectionChanged;

                // 텍스트박스와 버튼을 초기화 시키기 위해 이벤트 핸들러 호출 (NavigationBar.cs)
                CollectionViewOnCurrentChanged(null, null);

                // 텍스트블럭(라벨 비스무리 한거) 초기화
                txtblkTotal.Text = coll.Count.ToString();
            }
            get
            {
                return coll;
            }
        }

        // 다음은 컬렉션이 관리하는 항목의 타입이다.
        // Add, New 명령 처리를 위해 사용된다.
        public Type ItemType
        {
            set { typeItem = value; }
            get { return typeItem; }
        }
      
        // CollectionView를 위한 이벤트 핸들러
        void CollectionViewOnCollectionChanged(object sender, 
                                        NotifyCollectionChangedEventArgs args)
        {
            txtblkTotal.Text = coll.Count.ToString();
        }

        void CollectionViewOnCurrentChanged(object sender, EventArgs args)
        {
            txtboxCurrent.Text = (1 + collview.CurrentPosition).ToString();
            btnPrev.IsEnabled = collview.CurrentPosition > 0;
            btnNext.IsEnabled = collview.CurrentPosition < coll.Count - 1;
            btnDel.IsEnabled = coll.Count > 1;
        }

        // 버튼 클릭 이벤트 모음 ---------------------------------------------------------------
        void FirstOnClick(object sender, RoutedEventArgs args)
        {
            // ICollectionVeiw에서 자동으로 앞뒤 컬렉션으로 이동할 수 있는 메서드 제공
            collview.MoveCurrentToFirst();
        }
        void PreviousOnClick(object sender, RoutedEventArgs args)
        {
            collview.MoveCurrentToPrevious();
        }
        void NextOnClick(object sender, RoutedEventArgs args)
        {
            collview.MoveCurrentToNext();
        }
        void LastOnClick(object sender, RoutedEventArgs args)
        {
            collview.MoveCurrentToLast();
        }

        // 새 데이터 만들기
        void AddOnClick(object sender, RoutedEventArgs args)
        {
            // 이거 먼말인지 모르게써여, 아는 분 제보좀 해주세요 ' ㅅ'
            ConstructorInfo info =  typeItem.GetConstructor(System.Type.EmptyTypes);

            // 리스트에 항목추가
            coll.Add(info.Invoke(null));

            // 뷰의 마지막 항목을 CurrentItem으로 설정
            collview.MoveCurrentToLast();
        }

        void DeleteOnClick(object sender, RoutedEventArgs args)
        {
            // 현재위치의 인덱스 삭제
            coll.RemoveAt(collview.CurrentPosition);
        }
        //----------------------------------------------------------------------------------------

        // 현재 레코드 번호 출력하는 텍스트 박스를 위한 이벤트 핸들러
        string strOriginal;

        // 텍스트 박스에 포커스가 존재할때 이벤트
        void TextBoxOnGotFocus(object sender, KeyboardFocusChangedEventArgs args)
        {
            // 텍스트박스에 있는 값 변수저장
            // 여기서는 텍스트 박스에 직접 입력한 값으로 레코드 이동이 가능
            strOriginal = txtboxCurrent.Text;
        }

        // 텍스트 박스에 포커스가 사라졌을때 이벤트
        void TextBoxOnLostFocus(object sender, KeyboardFocusChangedEventArgs args)
        {
            int current;

            // if문안에 조건을 잘 모르게써여, 아는 분 제보 바람 ' ㅁ'
            if (Int32.TryParse(txtboxCurrent.Text, out current))
                if (current > 0 && current <= coll.Count)
                    collview.MoveCurrentToPosition(current - 1);
            else
                txtboxCurrent.Text = strOriginal;
        }
        void TextBoxOnKeyDown(object sender, KeyEventArgs args)
        {
            if (args.Key == Key.Escape)
            {
                // esc 키를 입력하면 텍스트박스에 있는 값이 현재 레코드 값으로 다시 변경
                txtboxCurrent.Text = strOriginal;
                args.Handled = true;
            }
            else if (args.Key == Key.Enter)
            {
                args.Handled = true;
            }
            else
                return;

            // 포커스를 맨 앞으로 이동.. 그니깐 오른쪽? 에 있는 애한테 이동
            MoveFocus(new TraversalRequest(FocusNavigationDirection.Right));
        }
    }
}
