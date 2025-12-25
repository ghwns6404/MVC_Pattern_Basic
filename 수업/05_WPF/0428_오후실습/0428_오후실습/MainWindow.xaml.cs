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

namespace _0428_오후실습
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        private People people = new People();
        private Person per;
        private Random rand = new Random();
        public MainWindow()
        {
            InitializeComponent();

            per = people[0];

            UpdateNameToUI();
            UpdatePhoneToUI();
            UpdateIdToUI();
            UpdatePwToUI();
        }
        #region Update 시리즈
        private void UpdateNameToUI()
        {
            if (per == null)
            {
                name.Text = "";
                nameLabel.Content = "";

            }
            else
            {
                name.Text = per.Name;
                nameLabel.Content = per.Name;
            }
        }
        private void UpdatePhoneToUI()
        {
            if (per == null)
            {
                phone.Text = "";
                phoneLabel.Content = "";

            }
            else
            {
                phone.Text = per.Phone;
                phoneLabel.Content = per.Phone;

            }
        }
        private void UpdateIdToUI()
        {
            if (per == null)
            {
                id.Text = "";
                idLabel.Content = "";

            }
            else
            {
                id.Text = per.Id;
                idLabel.Content = per.Id;

            }
        }
        private void UpdatePwToUI()
        {
            if (per == null)
            {
                pw.Text = "";
                pwLabel.Content = "";

            }
            else
            {
                pw.Text = per.Pw;
                pwLabel.Content = per.Pw;

            }
        }
        private void UpdateListBox()
        {
            listbox.Items.Clear();
            foreach (Person p in people)
                listbox.Items.Add(p.ToString());
        }
        #endregion


        private void listbox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (listbox.SelectedIndex >= 0)
            {
                per = people[listbox.SelectedIndex];
                UpdateNameToUI();
                UpdatePhoneToUI();
                UpdateIdToUI();
                UpdatePwToUI();
            }
        }

        private void addButton_Click(object sender, RoutedEventArgs e)
        {
            if (name.Text == "" || phone.Text == "" || id.Text == "" || pw.Text == "")
                return;

            people.Add(new Person() { Name = name.Text, Phone = phone.Text,
                                        Id = id.Text,Pw = pw.Text});
            // 리스트 박스의 아이템을 갱신한다. 
            UpdateListBox();
        }

        private void removeButton_Click(object sender, RoutedEventArgs e)
        {
            if (listbox.SelectedIndex >= 0)
            {
                people.RemoveAt(listbox.SelectedIndex);

                // 컬렉션에 원소가 없다면 리슽의 현재 아이템이 없도록(per =null) 한다. 
                if (people.Count == 0)
                    per = null;
                else
                    per = people[0];

                // 모든 UI 컨트롤을 갱신한다. 
                UpdateNameToUI();
                UpdatePhoneToUI();
                UpdateIdToUI();
                UpdatePwToUI();
                UpdateListBox();
            }
        }

        private void updateButton_Click(object sender, RoutedEventArgs e)
        {
            if (name.Text == "" || phone.Text == "")
                return;

            per.Name = name.Text;
            per.Phone = phone.Text;
            per.Id = id.Text;
            per.Pw = pw.Text;

            UpdateNameToUI();
            UpdatePhoneToUI();
            UpdateIdToUI();
            UpdatePwToUI();
            UpdateListBox();
        }
        private void selectButton_Click(object sender, RoutedEventArgs e)
        {
            if (name.Text == "" || phone.Text == "" || id.Text == "" || pw.Text == "")
                return;

            UpdateNameToUI();
            UpdatePhoneToUI();
            UpdateIdToUI();
            UpdatePwToUI();
            UpdateListBox();
        }

        private void randomButton_Click(object sender, RoutedEventArgs e)
        {
            string randomName = "";
            string randomPhone = "010";
            string randomId = "";
            string randomPw = "";
            for (int i = 0; i < 3; i++)
            {
                //이름
                int code = rand.Next(0xAC00, 0xD7A4);
                randomName += (char)code;
            }
            for (int i = 0; i < 2; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    //전화번호
                    randomPhone += rand.Next(0, 10);
                }
                randomPhone += rand.Next(0, 10); 
            }
            for (int i = 0; i < 8; i++)
            {
                //아이디
                char c = (char)rand.Next('A', 'Z' + 1);
                randomId += c;
            }
            for (int i = 0; i < 10; i++)
            {
                //비밀번호
                char c = (char)rand.Next('A', 'Z' + 1);
                randomPw += c;
            }
            name.Text = randomName;
            phone.Text = randomPhone;
            id.Text = randomId;
            pw.Text = randomPw;
        }
    }
    public class Person
    {
        public string Name { get; set; }
        public string Phone { get; set; }
        public string Id { get; set; }
        public string Pw { get; set; }
        public override string ToString()
        {
            return Name + " : " + Phone +  "\t" + "ID: " + Id + "\t" + " PW: " + Pw;
        }
    }

    public class People : List<Person>
    {
        public People()
        {
            Add(new Person() { Name = "홍길동", Phone = "010-1111-1234" , Id ="asd1", Pw ="asd1"});
            Add(new Person() { Name = "일지매", Phone = "010-2222-1234", Id = "asd2", Pw = "asd2" });
            Add(new Person() { Name = "임꺽정", Phone = "010-3333-1234" , Id ="asd3", Pw ="asd3"});
        }
    }


}
