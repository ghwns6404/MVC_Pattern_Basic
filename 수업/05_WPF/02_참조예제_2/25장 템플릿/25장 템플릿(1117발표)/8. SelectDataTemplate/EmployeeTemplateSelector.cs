//---------------------------------------------------------
// EmployeeTemplateSelector.cs (c) 2006 by Charles Petzold
//---------------------------------------------------------
using Petzold.ContentTemplateDemo;
using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.SelectDataTemplate
{   
    //컨텐트 타입에 따라 컨텐트를 다양한 형태로 보여주는 방법
    //ContentControl에서 정의된 ContentTemplateSelector를 사용 방법
    //1. DataTemplateSelector를 상속    
    public class EmployeeTemplateSelector : DataTemplateSelector
    {
        //2. SelectTemplate을 오버라이딩
        
        public override DataTemplate SelectTemplate(object item, 
                                                    DependencyObject container)
        {
            //2-1. SelectTemplate메소드의 첫번째 인자는 ContentControl에 보여질 객체
            //2-2. 두번째 인자는 ContentPresenter타입
            Employee emp = item as Employee;
            FrameworkElement el = container as FrameworkElement;

            return (DataTemplate) el.FindResource(
                emp.LeftHanded ? "templateLeft" : "templateRight");
        }
    }
}
