//-------------------------------------------------
// LoadEmbeddedXaml.cs (c) 2006 by Charles Petzold
//-------------------------------------------------
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Xml;

namespace Petzold.LoadEmbeddedXaml
{
    public class LoadEmbeddedXaml : Window
    {
        [STAThread]
        public static void Main()
        {
            Application app = new Application();
            app.Run(new LoadEmbeddedXaml());
        }
        public LoadEmbeddedXaml()
        {
            Title = "Load Embedded Xaml";

            string strXaml =
                "<Page xmlns='http://schemas.microsoft.com/winfx/2006/xaml/presentation' " +
                    "WindowTitle='Xaml Page'>"+
                    "<StackPanel>"+
                        "<Button HorizontalAlignment='Center' Margin='24'>"+
                            "Hello, XAML!"+
                        "</Button>"+
                        "<Ellipse Width='200' Height='100' Margin='24' Stroke='Red' StrokeThickness='10' />"+
                        "<ListBox Width='100' Height='100' Margin='24'>"+
                            "<ListBoxItem>Sunday</ListBoxItem>"+
                            "<ListBoxItem>Monday</ListBoxItem>"+
                            "<ListBoxItem>Tuesday</ListBoxItem>"+
                            "<ListBoxItem>Wednesday</ListBoxItem>"+
                           " <ListBoxItem>Thursday</ListBoxItem>"+
                          "  <ListBoxItem>Friday</ListBoxItem>"+
                         "   <ListBoxItem>Saturday</ListBoxItem>"+
                        "</ListBox>"+
                    "</StackPanel>"+
                "</Page>";
                //"<Button xmlns='http://schemas.microsoft.com/" +
                //                      "winfx/2006/xaml/presentation'" +
                //" Foreground='LightSeaGreen' FontSize='24pt'>" +
                //"Click me!" +
                //"</Button>";
            

            StringReader strreader = new StringReader(strXaml);
            XmlTextReader xmlreader = new XmlTextReader(strreader);

            object obj = XamlReader.Load(xmlreader);
            Content = obj;
        }
    }
}
