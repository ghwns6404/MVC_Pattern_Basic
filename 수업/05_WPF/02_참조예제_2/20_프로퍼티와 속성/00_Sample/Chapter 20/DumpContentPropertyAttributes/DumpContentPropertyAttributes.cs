//--------------------------------------------------------------
// DumpContentPropertyAttributes.cs (c) 2006 by Charles Petzold
//--------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Windows;
using System.Windows.Markup;

public class DumpContentPropertyAttributes
{
    [STAThread]
    public static void Main()
    {
        // PresentationCore와 PresentationFramework가 로드되는지 확인
        UIElement dummy1 = new UIElement();
        FrameworkElement dummy2 = new FrameworkElement();

        // 컨텐트 프로퍼티와 클래스를 저장할 SortedList
        SortedList<string, string> listClass = new SortedList<string, string>();

        // 문자열을 포맷팅
        string strFormat = "{0,-35}{1}";

        // 로드한 어셈블리를 순회하는 루프
        foreach (AssemblyName asmblyname in 
                    Assembly.GetExecutingAssembly().GetReferencedAssemblies())
        {
            // 타입 루프
            foreach (Type type in Assembly.Load(asmblyname).GetTypes())
            {
                // 커스텀 속성 루프
                // (상속받지 않은 것에 한해 false로 인자를 설정)
                foreach (object obj in type.GetCustomAttributes(
                                        typeof(ContentPropertyAttribute), true))
                {
                    // ContentPropertyAttribute이면 리스트에 추가
                    if (type.IsPublic && obj as ContentPropertyAttribute != null)
                        listClass.Add(type.Name,
                                      (obj as ContentPropertyAttribute).Name);
                }
            }
        }
        // 결과 출력
        Console.WriteLine(strFormat, "Class", "Content Property");
        Console.WriteLine(strFormat, "-----", "----------------");

        foreach (string strClass in listClass.Keys)
            Console.WriteLine(strFormat, strClass, listClass[strClass]);
    }
}
