using System;
using System.Drawing;
using System.Windows.Forms;

public class ListBoxExam : Form
{
    private CheckedListBox checkedListBox1;
    private ListBox listBox1;
    private TextBox txt_info;
    string[] str1 = { "딸기", "바나나", "포도", "귤", "호두", "사과", "수박", "체리" };
    string[] str2 = { "서울", "경기", "충청", "전라", "경상", "제주" };

    public ListBoxExam()
    {

        listBox1 = new ListBox();
        checkedListBox1 = new CheckedListBox();
        txt_info = new TextBox();

        listBox1.Parent = this;
        listBox1.SetBounds(10, 10, 50, 100);
        listBox1.SelectedIndexChanged += new EventHandler(SelectedIndexChanged);
        checkedListBox1.Parent = this;
        checkedListBox1.SetBounds(70, 10, 50, 100);
        checkedListBox1.SelectedIndexChanged += new EventHandler(SelectedIndexChanged);

        txt_info.Parent = this;
        txt_info.SetBounds(10, 120, 300, 120);
        txt_info.Multiline = true;

        for (int i = 0; i < str1.Length; i++)
            listBox1.Items.Add(str1[i]);

        for (int j = 0; j < str2.Length; j++)
            checkedListBox1.Items.Add(str2[j]);
    }

    private void SelectedIndexChanged(object sender, EventArgs e)
    {
        ListControl obj = (ListControl)sender;
        if (obj == listBox1)
        {
            txt_info.Text += "\r\n [listBox 선택] : " + listBox1.SelectedItem;
        }
        else if (obj == checkedListBox1)
        {
            string str = "";
            for (int i = 0; i < checkedListBox1.CheckedItems.Count; i++)
            {
                str += "\t" + (i + 1).ToString() + " : " + checkedListBox1.CheckedItems[i].ToString();
            }
            txt_info.Text += "\r\n [CheckListBox 선택] : " + str;
        }
    }


    public static void Main()
    {
        Application.Run(new ListBoxExam());
    }
}
