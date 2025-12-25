using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace FakeStar
{
    public class MyClient
    {
        #region 싱글톤
        private static MyClient instance;
        public static MyClient Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new MyClient();
                    instance.client.Timeout = TimeSpan.FromSeconds(5);
                }
                return instance;
            }
        }
        private MyClient()
        {
            client = new HttpClient();
        }
        #endregion

        #region 멤버필드 (속성)
        public class Member
        {
            public string Id { get; set; }
            public string Email { get; set; }
            public string Pw { get; set; }
            public string Name { get; set; }
            public string CreatedAt { get; set; }
        }

        public class PublicMember
        {
            public string Id { get; set; }
            public string Name { get; set; }
            public string CreatedAt { get; set; }
        }

        public class Post
        {
            public int Id { get; set; }

            public string MemId { get; set; }

            public string Title { get; set; }

            public string Content { get; set; }

            public string CreatedAt { get; set; }

            public string UpdatedAt { get; set; }

            public List<int> FileIds { get; set; }
        }

        public class CreateSessionresponse
        {
            public string token { get; set; }
        }

        public class Session
        {
            public int Id { get; set; }
            public int MemId { get; set; }
            public string CreatedAt { get; set; }
            public string UpdatedAt { get; set; }
        }

        public class Sessionstore
        {
            public string Token { get; set; }

            public int Session_id { get; set; }

            public int Mem_id { get; set; }

            public string Created_at { get; set; }

            public string Expired_at { get; set; }
        }
        #endregion

        private readonly string baseUrl = "http://localhost:9000";
        private readonly HttpClient client;
        private string sessionToken;

        #region Member(회원 관리)
        // 1. 모든 멤버 조회 (GET /members) ok
        public async Task<List<PublicMember>> GetMembersAsync()
        {
            var response = await client.GetAsync($"{baseUrl}/members");
            response.EnsureSuccessStatusCode();
            string json = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<List<PublicMember>>(json);
        }

        // 2. 특정 멤버 조회 (GET /members/{id})  ok
        public async Task<PublicMember> GetMemberAsync(string id)
        {
            var response = await client.GetAsync($"{baseUrl}/members/{id}");
            response.EnsureSuccessStatusCode();
            string json = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<PublicMember>(json);
        }

        // 3. 내 정보 조회 (GET /members/me)   ok
        public async Task<Member> GetMemberMeAsync()
        {
            try
            {
                var response = await client.GetAsync($"{baseUrl}/members/me");
                if (response.StatusCode == System.Net.HttpStatusCode.Unauthorized)
                {
                    return null;
                }
                response.EnsureSuccessStatusCode();
                string json = await response.Content.ReadAsStringAsync();
                return JsonConvert.DeserializeObject<Member>(json);
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine("서버 연결 오류: " + ex.Message);
                return null;
            }
            catch (Exception ex)
            {
                Console.WriteLine("알 수 없는 오류: " + ex.Message);
                return null;
            }
        }

        // 4. 멤버 추가 (POST /members, x-www-form-urlencoded)  ok
        public async Task<bool> InsertMemberAsync(string email, string pw, string name)
        {
            var body = new
            {
                email = email,
                pw = pw,
                name = name
            };
            var content = new StringContent(JsonConvert.SerializeObject(body), Encoding.UTF8, "application/json");
            var response = await client.PostAsync($"{baseUrl}/members", content);
            return response.IsSuccessStatusCode;
        }

        // 5. 비밀번호 변경 (PUT /members/pw, x-www-form-urlencoded)  ok
        public async Task<bool> UpdateMemberPwAsync(string pw, string newPw)
        {
            var body = new { pw = pw, newPw = newPw };
            var content = new StringContent(JsonConvert.SerializeObject(body), Encoding.UTF8, "application/json");
            var response = await client.PutAsync($"{baseUrl}/members/me/pw", content);
            return response.IsSuccessStatusCode;
        }

        // 6. 내 계정 삭제 (DELETE /members/me)  ok
        public async Task<bool> DeleteMemberMeAsync()
        {
            var request = new HttpRequestMessage(HttpMethod.Delete, $"{baseUrl}/members/me");
            var response = await client.SendAsync(request);
            return response.IsSuccessStatusCode;
        }

        // 7. 로그인 (POST /sessions/me, x-www-form-urlencoded)   ok
        public async Task<bool> LoginMemberAsync(string email, string pw)
        {
            var body = new { email = email, pw = pw };
            var content = new StringContent(JsonConvert.SerializeObject(body), Encoding.UTF8, "application/json");
            var response = await client.PostAsync($"{baseUrl}/sessions/me", content);

            if (!response.IsSuccessStatusCode)
                return false;

            string json = await response.Content.ReadAsStringAsync();
            CreateSessionresponse data = JsonConvert.DeserializeObject<CreateSessionresponse>(json);
            if (string.IsNullOrEmpty(data?.token))
                return false;


            sessionToken = data.token;

            //-------------------------

            // Bearer 없이 토큰만 추가
            client.DefaultRequestHeaders.Remove("Authorization");
            client.DefaultRequestHeaders.Add("Authorization", sessionToken);
            return true;
        }
        #endregion

        #region Post(게시물 관리)
        // 9. 모든 게시글 조회 (GET /posts)   
        public async Task<List<Post>> GetPostsAsync()
        {
            var response = await client.GetAsync($"{baseUrl}/posts");

            response.EnsureSuccessStatusCode();
            string json = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<List<Post>>(json);
        }

        public async Task<Image> GetPostFileAsync(int id)
        {
            var response = await client.GetAsync($"{baseUrl}/post-files/{id}");
            response.EnsureSuccessStatusCode();

            byte[] imageBytes = await response.Content.ReadAsByteArrayAsync();
            using (var ms = new MemoryStream(imageBytes))
            {
                return Image.FromStream(ms);  // 이미지 변환
            }
        }

        // 10. 특정 게시글 조회 (GET /posts/{id})
        public async Task<Post> GetPostAsync(string id)
        {
            var response = await client.GetAsync($"{baseUrl}/posts/{id}");
            response.EnsureSuccessStatusCode();
            string json = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<Post>(json);
        }

        // 11. 게시글 추가 (POST /posts, multipart/form-data)
        public async Task<bool> InsertPostAsync(string title, string content, string filePath = null)
        {
            try
            {
                using (var form = new MultipartFormDataContent())
                {
                    form.Add(new StringContent(title), "title");
                    form.Add(new StringContent(content), "content");
                    //foreach (string filePath in filePaths)
                    //{

                    if (!string.IsNullOrEmpty(filePath) && System.IO.File.Exists(filePath))
                    {
                        var fileContent = new ByteArrayContent(System.IO.File.ReadAllBytes(filePath));
                        form.Add(fileContent, System.IO.Path.GetFileName(filePath), System.IO.Path.GetFileName(filePath));
                    }
                    //}

                    var response = await client.PostAsync($"{baseUrl}/posts", form);

                    if (response.StatusCode == System.Net.HttpStatusCode.Created)       //여기서 true로 안가고 예외처리 | 
                    {
                        return true;
                    }
                    else if (response.StatusCode == System.Net.HttpStatusCode.BadRequest)
                    {
                        string msg = await response.Content.ReadAsStringAsync();
                        MessageBox.Show(msg, "요청 오류", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return false;
                    }
                    else
                    {
                        string msg = await response.Content.ReadAsStringAsync();
                        MessageBox.Show(msg, "서버 오류", MessageBoxButtons.OK, MessageBoxIcon.Error);  //여기서 오류 발생
                        return false;
                    }
                }
            }
            catch (HttpRequestException ex)
            {
                MessageBox.Show("서버 연결 오류: " + ex.Message, "네트워크 오류", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            catch (Exception ex)
            {
                MessageBox.Show("알 수 없는 오류: " + ex.Message, "오류", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
        }


        // 12. 게시글 삭제 (DELETE /posts/{id})
        public async Task<bool> DeletePostAsync(string id)
        {
            var request = new HttpRequestMessage(HttpMethod.Delete, $"{baseUrl}/posts/{id}");
            var response = await client.SendAsync(request);

            if (!response.IsSuccessStatusCode)
            {
                string msg = await response.Content.ReadAsStringAsync();
                MessageBox.Show($"삭제 실패: {response.StatusCode}\n{msg}", "오류", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            return true;
        }
        #endregion

        #region Session(세션 관리)
        // 8. 내 세션 정보 (GET /sessions/whoami)   ok
        public async Task<Session> GetSessionWhoamiAsync()
        {
            var response = await client.GetAsync($"{baseUrl}/sessions/whoami");
            response.EnsureSuccessStatusCode();
            string json = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<Session>(json);
        }

        // 내 세션 목록 조회 (GET /sessions/me)
        public async Task<List<Session>> GetSessionMeAsync()
        {
            var response = await client.GetAsync($"{baseUrl}/sessions/me");
            response.EnsureSuccessStatusCode();
            string json = await response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<List<Session>>(json);
        }

        // 세션 삭제 (DELETE /sessions/{id})
        public async Task<bool> DeleteSessionAsync(string id)
        {
            var request = new HttpRequestMessage(HttpMethod.Delete, $"{baseUrl}/sessions/{id}");
            var response = await client.SendAsync(request);
            return response.IsSuccessStatusCode;
        }
        #endregion
    }
}
