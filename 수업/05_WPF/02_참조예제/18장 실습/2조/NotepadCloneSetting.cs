using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Xml.Serialization;

namespace Petzold.NotPadClone
{

    /*************************************************************************************
 클래스명 : NotepadCloneSetting
 필      드 :
 설      명 : 파일의 기본값을 설정,파일 설정에 대해 저장과 불어들임
**************************************************************************************/

    class NotepadCloneSetting
    {
        public WindowState WindowState = WindowState.Normal;  //최소화, 보통 등의 창 상태를 가져오거나 설정합니다.
        public Rect RestoreBounds = Rect.Empty;               //위치와 넓이가 없는 사각형을 나타내는 특수 값을 가져옵니다
        public TextWrapping TextWrapping = TextWrapping.NoWrap;        // 개체의 텍스트 줄 바꿈 방식을 나타내는 열거형 값 중 하나입니다.
        public string FontFamily = "";                            //이 요소의 콘텐츠에 대한 기본 설정 글꼴 패밀리를 가져오거나 설정합니다. 
        public string FontStyle = new FontStyleConverter().ConvertToString(FontStyles.Normal);
        public string FontWeight = new FontWeightConverter().ConvertToString(FontWeights.Normal);
        public string FontStretch = new FontStretchConverter().ConvertToString(FontStretches.Normal);
        public double FontSize = 11;

        //설정을 파일에 저장
        public virtual bool Save(string strAppData)
        {
            try
            {
                Directory.CreateDirectory(Path.GetDirectoryName(strAppData));  //지정된 path로 모든 디렉터리와 하위 디렉터리를 만듭니다. 
                StreamWriter write = new StreamWriter(strAppData);
                XmlSerializer xml = new XmlSerializer(GetType());
                xml.Serialize(write, this);   //개체를 XML 문서로 serialize하고 XML 문서에서 deserialize합니다. XmlSerializer를 사용하면 개체가 XML로 인코딩되는 방법을 제어할 수 있습니다.
                write.Close();
            }
            catch
            {
                return false;
            }
            return true;

        }

        //설정을 파일에서 불어들임
        public static object Load(Type type, string strAppData)
        {
            StreamReader reader;
            object settings;
            XmlSerializer xml = new XmlSerializer(type);

            try
            {
                reader = new StreamReader(strAppData);
                settings = xml.Deserialize(reader);   //지정된 StreamReader에 포함된 XML 문서를 deserialize합니다. .NET Compact Framework에서 지원됩니다. 
                reader.Close();
            }
            catch
            {
                //지정된 매개 변수가 있는 인스턴스에서 리플렉션된 생성자를 호출하여 일반적으로 사용되지 않는 매개 변수에 대한 기본값을 제공합니다. 
                settings = type.GetConstructor(System.Type.EmptyTypes).Invoke(null); 

            }
            return settings;
        }
    }
}
