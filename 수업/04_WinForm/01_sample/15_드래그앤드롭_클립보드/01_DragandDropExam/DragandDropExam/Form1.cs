using System;
using System.Drawing;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace DragandDropExam
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // 드래그 앤 드롭 기능 사용 설정(폼 드롭 기능 활성화)
            this.AllowDrop = true;          

            // 드래그 앤 드롭이 가능한 텍스트 박스 컨트롤 구현
            this.txt_box.AllowDrop = true;  // TextBox 드롭 기능 활성화
            // DragOber :마우스가 컨트롤이나 폼 안에 들어와서 움직일때마다 호출
            this.txt_box.DragOver += new DragEventHandler(txt_box_DragOver); 
            // 마우스가 컨트롤이나 폼 안에 존재할 때, 아우스를 놓으면 발생
            this.txt_box.DragDrop += new DragEventHandler(txt_box_DragDrop);

            // 드래그 앤 드롭이 가능한 리치에디트박스 컨트롤 구현
            this.rich_txt_box.AllowDrop = true; // RichTextBox 드롭 기능 활성화  
            // DragEnter : 드래깅 하는 마우스 포인터가 처음으로 컨트롤이나 폼에 들어왔을 경우
            this.rich_txt_box.DragEnter += new DragEventHandler(rich_txt_box_DragEnter);
            this.rich_txt_box.DragDrop  += new DragEventHandler(rich_txt_box_DragDrop);

            this.DragOver += new DragEventHandler(pic_box_DragOver); //DragOver 이벤트
            this.DragDrop += new DragEventHandler(pic_box_DragDrop); //DragDrop 이벤트
        }

        // TextBox 위에 마우스가 위치하면
        void txt_box_DragOver(object sender, DragEventArgs dea)
        {
            // GetDataPresent : 확인할 형식에 해당되는 데이터가 Data 속성에 
            // 포함되어 있는지를 알려줌 
            // 형식) 21가지의 형식 : Bitmap, Dib, FileDrop(파일 놓기)...

            // 드래그한 데이터가 파일이거나 문자열 타입이라면
            if (dea.Data.GetDataPresent(DataFormats.FileDrop) ||
               dea.Data.GetDataPresent(DataFormats.StringFormat))
            {
                // 드래그한 데이터가 Move 기능을 허용한다면 
                if ((dea.AllowedEffect & DragDropEffects.Move) != 0)
                    // 끌기 소스의 데이터가 놓기 대상으로 이동
                    dea.Effect = DragDropEffects.Move;// 코드 존재 유무 

                // 드래그한 데이터가 Ctrl 키가 눌린 상태에서 Copy를 허용한다면
                // keyState 1 : 마우스 왼쪽 버튼
                //          2 : 마우스 오른쪽 버튼
                //          4 : Shift 키
                //          8 : Ctrl 키
                //         16 : 마우스 중간 버튼
                //         32 : Alt 키 
                if(((dea.AllowedEffect & DragDropEffects.Copy) != 0) &&
                   ((dea.KeyState & 8) != 0))
                    dea.Effect = DragDropEffects.Copy;// 데이터가 놓기 대상에 복사됨
                    
            }
        }


        // 마우스버튼을 놓을 때 발생
        void txt_box_DragDrop(object sender, DragEventArgs dea)
        {
            if (dea.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] fname = (string[])dea.Data.GetData(DataFormats.FileDrop);
                TextReader tr = new StreamReader(fname[0]);// 파일 읽기 
                txt_box.Text = tr.ReadToEnd();// 텍스트 박스에 출력
                tr.Close();// 파일 닫기 
            }
        }

        void rich_txt_box_DragEnter(object sender, DragEventArgs dea)
        {
            // 끌기 소스가 파일이나 문자열일 경우 
            if (dea.Data.GetDataPresent(DataFormats.FileDrop) ||
                dea.Data.GetDataPresent(typeof(string)))
            {
                // 드래그한 개체의 Effect 속성을 Move 속성으로 변경
                if ((dea.AllowedEffect & DragDropEffects.Move) != 0)
                    dea.Effect = DragDropEffects.Move;

                // Copy 속성 및 Ctrl 키 
                // - 1 : 마우스 왼쪽 버튼
                // - 2 : 마우스 오른쪽 버튼
                // - 4 : Shift 키
                // - 8 : Ctrl 키
                // - 16 : 마우스 중간 버튼
                // - 32 : Alt 키 
                if (((dea.AllowedEffect & DragDropEffects.Copy) != 0) &&
                    ((dea.KeyState & 8) != 0))      // Ctrl 키가 눌린경우 
                    dea.Effect = DragDropEffects.Copy;
            }
        }

        void rich_txt_box_DragDrop(object sender, DragEventArgs dea)
        {
            if (dea.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] fname = (string[])dea.Data.GetData(DataFormats.FileDrop);                
                this.rich_txt_box.LoadFile(fname[0]);
            }
        }

        void pic_box_DragOver(object sender, DragEventArgs dea)
        {
            if (dea.Data.GetDataPresent(DataFormats.FileDrop) ||
               dea.Data.GetDataPresent(typeof(Bitmap)))
            {
                if ((dea.AllowedEffect & DragDropEffects.Move) != 0)
                    dea.Effect = DragDropEffects.Move;

                if (((dea.AllowedEffect & DragDropEffects.Copy) != 0) &&
                    ((dea.KeyState & 8) != 0))
                    dea.Effect = DragDropEffects.Copy;
            }
        }

        void pic_box_DragDrop(object sender, DragEventArgs dea)
        {
            if (dea.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string [] fname = (string [])dea.Data.GetData(DataFormats.FileDrop);
                try
                {
                    Image img = Image.FromFile(fname[0]);
                    this.pic_box.Image = img;                    
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return;
                }
            }

        }
    }
}