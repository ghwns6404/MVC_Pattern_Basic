//---------------------------------------
// People.cs (c) 2006 by Charles Petzold
//---------------------------------------
using Microsoft.Win32;
using Petzold.SingleRecordDataEntry;
using System;
using System.IO;
using System.Collections.ObjectModel;
using System.Windows;
using System.Xml.Serialization;

namespace Petzold.MultiRecordDataEntry
{
    // ObservableCollection : 
    // 항목이 추가 또는 제거되거나 전체목록이 새로 고쳐질 때 
    // 알림을 제공하는 동적 데이터 컬렉션
    // 제너릭 클래스 이기 때문에 Person 타입의 새로운 객체를 컬렉션에 추가할 때 캐스팅 안해도 됨
    public class People : ObservableCollection<Person>
    {
        // 파일열기, 저장 다이얼로그 띄울때 파일형식부분에 쓸꺼
        const string strFilter = "People XML files (*.PeopleXml)|" +
                                 "*.PeopleXml|All files (*.*)|*.*";

        // People타입 객체 반환하는 정적 메서드, Window타입 객체를 인자로 받음
        public static People Load(Window win)               
        {
            OpenFileDialog dlg = new OpenFileDialog();      // 파일열기 다이얼로그 객체 생성
            dlg.Filter = strFilter;                                     // 표시되는 파일형식 대입 (파일형식부분)
            People people = null;                                  // People 객체 초기화

            if ((bool)dlg.ShowDialog(win))
            {
                try
                {
                    // 파일이름 문자열을 읽는 스트림 리더 객체 생성
                    StreamReader reader = new StreamReader(dlg.FileName);

                    // Person객체를 xml 포맷으로 저장하기 위한 객체 생성
                    XmlSerializer xml = new XmlSerializer(typeof(People));

                    // 스트림에서 객체를 재구성 해서 Person 객체에 넣음
                    people = (People)xml.Deserialize(reader);
                    reader.Close();
                }
                catch (Exception exc)
                {
                    MessageBox.Show("파일을 못열겠어요 ㅡ  _ㅡ : " + exc.Message,
                        win.Title, MessageBoxButton.OK,MessageBoxImage.Exclamation);
                    people = null;
                }
            }
            return people;
        }
        public bool Save(Window win)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = strFilter;

            if ((bool)dlg.ShowDialog(win))
            {
                try
                {
                    StreamWriter writer = new StreamWriter(dlg.FileName);
                    XmlSerializer xml = new XmlSerializer(GetType());

                    // 읽어서 저장
                    xml.Serialize(writer, this);
                    writer.Close();
                }
                catch (Exception exc)
                {
                    MessageBox.Show("나 이거 저장 못하겠슴 ' ㅁ'  : " + exc.Message,
                                    win.Title, MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    return false;
                }
            }
            return true;
        }
    }
}
