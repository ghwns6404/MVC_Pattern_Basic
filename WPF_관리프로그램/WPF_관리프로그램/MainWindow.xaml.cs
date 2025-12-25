using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPF_관리프로그램
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        //private Member member = null;

        private MemberControl membercontrol = MemberControl.Singleton;
        public MainWindow()
        {
            InitializeComponent();
            membercontrol.mainwindow = this;
            membercontrol.members = (MemberList)FindResource("MemberList");
            membercontrol.member = (Member)FindResource("Member");
        }

        private void InsertButton_Click(object sender, RoutedEventArgs e)
        {
            int _id = int.Parse(Id.Text);
            string _name = Name.Text;
            int _age = int.Parse(Age.Text);
            string _phone = Phone.Text;

            membercontrol.Con_Insert(_id, _name, _age, _phone);
        }
        private void SelectButton_Click(object sender, RoutedEventArgs e)
        {
            int SelectId = int.Parse(SelectData.Text);

            membercontrol.Con_Select(SelectId);
            
        }
        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            Member update_member = (Member)listbox.SelectedItem;
            
            membercontrol.Con_Update(update_member);

        }
        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
           int idx = listbox.SelectedIndex;
           membercontrol.Con_Delete(idx);
        }

        
    }
}
