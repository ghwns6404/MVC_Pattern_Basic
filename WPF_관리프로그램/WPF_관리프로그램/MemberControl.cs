using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Xml.Linq;

namespace WPF_관리프로그램
{
    internal class MemberControl
    {
        public MemberList members = null;
        public Member member = null;
        public MainWindow mainwindow = null;

        #region 생성자
        public static MemberControl Singleton { get; private set; }
        static MemberControl()
        {
            Singleton = new MemberControl();
        }
        private MemberControl() { }
       

        #endregion

        public void Con_Insert(int id, string name, int age, string phone)
        {
            members.Add(new Member(id, name, age, phone));
        }
        public void Con_Select(int selectId)
        {
            member = members.FirstOrDefault(x => x.Id == selectId);
            if (member == null)
            {
                MessageBox.Show("아이디 없음");
                return;
            }
            mainwindow.id.Text = member.Id.ToString();
            mainwindow.name.Text = member.Name;
            mainwindow.age.Text = member.Age.ToString();
            mainwindow.phone.Text = member.Phone;
        }
        public void Con_Update(Member update_member)
        {
            update_member.Id = int.Parse(mainwindow.Id.Text);
            update_member.Name = mainwindow.Name.Text;
            update_member.Age = int.Parse(mainwindow.Age.Text);
            update_member.Phone = mainwindow.Phone.Text;
        }
        public void Con_Delete(int idx)
        {
            members.RemoveAt(idx);
        }
        public void Init(StackPanel panel)
        {
            panel.DataContext = member;
        }


    }
}
