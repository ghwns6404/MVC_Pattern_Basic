using System;
using System.Drawing;
using System.Windows.Forms;

public class ComboBoxExam : Form
{
    private ComboBox cbb;
    public ComboBoxExam()
    {
        cbb = new ComboBox();
        cbb.Parent = this;
        cbb.SelectedValueChanged += new EventHandler(cbb_SelectedValueChanged);
        cbb.SelectionChangeCommitted += new EventHandler(cbb_SelectionChangeCommitted);
        cbb.Location = new Point(100, 100);
        cbb.Text = "색을 선택하세요";
        cbb.Items.Add("빨강");
        cbb.Items.Add("노랑");
        cbb.Items.Add("파랑");
        cbb.Items.Add("검정");
        cbb.Items.Add("햐양");
    }
    private void cbb_SelectedValueChanged(object sender, EventArgs e)
    {
        MessageBox.Show("SelectedValueChanged 이벤트 발생");
        Color color = Color.White;
        ComboBox obj = (ComboBox)sender;
        switch (obj.SelectedIndex)
        {
            case 0:
                color = Color.Red;
                break;
            case 1:
                color = Color.Yellow;
                break;
            case 2:
                color = Color.Blue;
                break;
            case 3:
                color = Color.Black;
                break;
            case 4:
                color = Color.White;
                break;
        }
        this.BackColor = color;
    }
    private void cbb_SelectionChangeCommitted(object sender, EventArgs e)
    {
        MessageBox.Show("SelectionChangeCommitted 이벤트 발생");
        this.Text = ((ComboBox)sender).SelectedText;
    }

    public static void Main()
    {
        Application.Run(new ComboBoxExam());
    }
}
