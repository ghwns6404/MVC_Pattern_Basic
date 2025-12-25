using System.IO;

namespace 관리프로그램Vol_5
    //ini파일을 만드는 곳인데 제발 호출이 되는지
    //폼을 닫을때 ini파일이 잘 만들어 졌는지 이백번 확인!!!!
{
    internal class MyInfoFile
    {
        const string FILE_NAME = "chatclient.ini";
        //const string MEMBER_SERVER_IP = "220.90.180.108";
        const string MEMBER_SERVER_IP = "127.0.0.1";
        const int MEMBER_SERVER_PORT = 9000;

        const string CHAT_SERVER_IP = "220.90.180.108";
        const int CHAT_SERVER_PORT = 8000;

        public static void WriteMesssage(string id, string pw)
        {
            using (StreamWriter writer = new StreamWriter(FILE_NAME))
            {
                writer.WriteLine("MEMBER_SERVER_IP:" + MEMBER_SERVER_IP);
                writer.WriteLine("MEMBER_SERVER_PORT:" + MEMBER_SERVER_PORT);
                writer.WriteLine("CHAT_SERVER_IP:" + CHAT_SERVER_IP);
                writer.WriteLine("CHAT_SERVER_PORT:" + CHAT_SERVER_PORT);
                writer.WriteLine("아이디:" + id);
                writer.WriteLine("패스워드:" + pw);
            }
        }

        public static void ReadMesssage(out string mip, out int mport, out string cip, out int cport, out string id, out string pw)
        {
            using (StreamReader reader = new StreamReader(FILE_NAME))
            {
                //"아이피:1111.1111.1111.1111"
                mip = reader.ReadLine().Split(':')[1];
                mport = int.Parse(reader.ReadLine().Split(':')[1]);
                cip = reader.ReadLine().Split(':')[1];
                cport = int.Parse(reader.ReadLine().Split(':')[1]);
                id = reader.ReadLine().Split(':')[1];
                pw = reader.ReadLine().Split(':')[1];
            }
        }
    }
}
