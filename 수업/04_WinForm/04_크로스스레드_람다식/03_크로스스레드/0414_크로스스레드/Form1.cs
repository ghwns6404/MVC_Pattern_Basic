using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _0414_크로스스레드
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        #region 첫번째
        private void button1_Click(object sender, EventArgs e)
        {
            Test1("동일 스레드에서 접근");
            
            Thread thread = new Thread(ThreadFun);            thread.IsBackground = true;
            thread.Start();
        }

        public void ThreadFun()
        {
            Thread.Sleep(2000);
            //Test1("다른 스레드에서 접근");
            SetTextBox("다른 스레드에서 접근");
        }

        #region 크로스 스레드 해결1
        delegate void fnSetTextBoxCallback(string contents);
        private void SetTextBox(string contents)
        {
            if (textBox1.InvokeRequired)
            {
                Invoke(new fnSetTextBoxCallback(Test1),
                    new object[] { contents });
            }
            else
            {
                Test1(contents);
            }

        }
        #endregion 
        
        public void Test1(string str)
        {
            textBox1.Text = str;
        }
        #endregion

        #region 크로스 스레드 2
        private void button2_Click(object sender, EventArgs e)
        {
            Thread thread = new Thread(ThreadFun1); 
            thread.IsBackground = true;
            thread.Start();
        }

        public void ThreadFun1()
        {
            SetTextBox(textBox1, "다른스레드에서호출");
        }

        public static void SetTextBox(TextBox tb, string contents)
        {
            if (tb.InvokeRequired)
            {
                tb.Invoke(new MethodInvoker(delegate ()
                {
                    tb.Text = contents;
                }));
            }
            else
            {
                tb.Text = contents;
            }
        }
        #endregion 
    }
}
