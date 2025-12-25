//------------------------------------------------------
// SingleRecordDataEntry.cs (c) 2006 by Charles Petzold
//------------------------------------------------------
using Microsoft.Win32;
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Xml.Serialization;

namespace Petzold.SingleRecordDataEntry
{
    public partial class SingleRecordDataEntry : Window
    {
        // 파일열기, 저장 다이얼로그 띄울때 파일형식부분에 쓸꺼
        const string strFilter = "Person XML files (*.PersonXml)|" +
                                 "*.PersonXml|All files (*.*)|*.*";

        // Person객체를 xml 포맷으로 저장하기 위한 객체 생성
        XmlSerializer xml = new XmlSerializer(typeof(Person));

        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new SingleRecordDataEntry());
        }
        public SingleRecordDataEntry()
        {
            InitializeComponent();

            // File New 명령을 시뮬레이션
            ApplicationCommands.New.Execute(null, this);

            // 패널의 첫번째 텍스트박스에 포커스를 준다.
            // pnlPerson이라는 이름은 xaml코드에서 설정해 줬음
            pnlPerson.Children[1].Focus();
        }
        // 새로하기 이벤트 핸들러
        void NewOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            // Person 타입의 새로운 객체 생성 하고 PersonPanel의 DataContext프로퍼티로 설정
            // 이렇게 하면 Person객체의 기본값이 PersonPanel에 표시됨
            pnlPerson.DataContext = new Person();
        }

        // 데이터열기 이벤트 핸들러 (파일 IO부분) 
        // 데이터 저장과의 차이는 Deserialize와 Serialize 부분
        void OpenOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            OpenFileDialog dlg = new OpenFileDialog();    // 파일열기 다이얼로그 객체 생성
            dlg.Filter = strFilter;                                   // 표시되는 파일형식 대입 (파일형식부분)
            Person pers;                                            // Person클래스 객체 생성

            if ((bool)dlg.ShowDialog(this))                      // 파일열기 다이얼로그가 열려있으면?
            {
                try
                {
                    // 파일이름 문자열을 읽는 스트림 리더 객체 생성
                    StreamReader reader = new StreamReader(dlg.FileName);
                    // 스트림에서 객체를 재구성 해서 Person 객체에 넣음
                    pers = (Person) xml.Deserialize(reader);
                    reader.Close();
                }
                catch (Exception exc)
                {
                    MessageBox.Show("파일을 못열겠어요 ㅡ  _ㅡ : " + exc.Message, 
                                    Title, MessageBoxButton.OK,  MessageBoxImage.Exclamation);
                    return;
                }
                // DataContext는 요소가 바인딩에 사용되는 데이터 소스에 대한 정보 및 경로 등
                // 바인딩의 기타 특성을 부모 요소로부터 상속 할 수 있도록 하는 개념.
                pnlPerson.DataContext = pers;
            }
        }

        // 데이터 저장 메뉴 이벤트 핸들러 (파일 IO부분)
        void SaveOnExecuted(object sender, ExecutedRoutedEventArgs args)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = strFilter;

            if ((bool)dlg.ShowDialog(this))
            {
                try
                {
                    StreamWriter writer = new StreamWriter(dlg.FileName);
                    // 읽어서 저장
                    xml.Serialize(writer, pnlPerson.DataContext);
                    writer.Close();
                }
                catch (Exception exc)
                {
                    MessageBox.Show("나 이거 저장 못하겠슴 ' ㅁ'  : " + exc.Message,
                                    Title, MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    return;
                }
            }
        }
    }
}
