//------------------------------------------------
// ControlMenuItem.cs (c) 2006 by Charles Petzold
//------------------------------------------------
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.DumpControlTemplate
{
    public class ControlMenuItem : MenuItem
    {
        public ControlMenuItem()
        {
            //Control 클래스가 정의돈 어셈블리를 얻음
            Assembly asbly = Assembly.GetAssembly(typeof(Control));

            // 그 클래스 내에 타입 모두를 배열로 저장
            Type[] atype = asbly.GetTypes();

            //Control의 파생클래스를 정렬된 리스트에 저장
            SortedList<string, MenuItem> sortlst = 
                                    new SortedList<string, MenuItem>();

            Header = "Control";
            Tag = typeof(Control);
            //             KEY   , Value
            sortlst.Add("Control", this);

            //배열 내의 모든 타입을 나열함
            //Control클래스와 그것의 파생 클래스를 위해 메뉴 항목을 생성해
            //SortedList 객체에 추가함
            //메뉴 항목의 Tag프로퍼티는 Type 객체와 관련돼 있음을 알아둘 것
            foreach (Type typ in atype)
            {
                if (typ.IsPublic && (typ.IsSubclassOf(typeof(Control))))
                {
                    MenuItem item = new MenuItem();
                    item.Header = typ.Name;
                    item.Tag = typ;
                    sortlst.Add(typ.Name, item);
                }
            }

            // 정렬된 리스를 순회하며 메뉴의 부모를 설정
            foreach (KeyValuePair<string, MenuItem> kvp in sortlst)
            {
                if (kvp.Key != "Control")
                {
                    string strParent = ((Type)kvp.Value.Tag).BaseType.Name;
                    MenuItem itemParent = sortlst[strParent];
                    itemParent.Items.Add(kvp.Value);
                }
            }

            //다시 순회
            //추상 클래스이고 선택할 것이 있으면 비활성
            //추상 클래스가 아니고 선택할 것이 없으면 항목 추가
            foreach (KeyValuePair<string, MenuItem> kvp in sortlst)
            {
                Type typ = (Type)kvp.Value.Tag;

                if (typ.IsAbstract && kvp.Value.Items.Count == 0)
                    kvp.Value.IsEnabled = false;

                if (!typ.IsAbstract && kvp.Value.Items.Count > 0)
                {
                    MenuItem item = new MenuItem();
                    item.Header = kvp.Value.Header as string;
                    item.Tag = typ;
                    kvp.Value.Items.Insert(0, item);
                }
            }
        }
    }
}
