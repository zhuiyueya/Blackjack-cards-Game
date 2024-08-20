#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

// Base64编码函数
std::string base64_encode(const std::string& str) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, str.c_str(), str.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    return std::string(bufferPtr->data, bufferPtr->length);
}

int main(int argc,char**argv) {
    int sock;
    struct sockaddr_in server_addr;
    std::string from = "1827369463@qq.com"; // 发件人QQ邮箱
    std::string password = "wsektjwtrzqtbcgd"; // 发件人授权码
    std::string to_qq_number = argv[1]; // 收件人QQ邮箱
    std::string subject = "Test Subject"; // 邮件主题
    std::string message = argv[2]; // 邮件正文

    // 创建套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    // 设置服务器地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(587); // 使用SSL端口
    if (inet_pton(AF_INET, "120.232.69.34", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported" << std::endl;
        return 1;
    }
	
    // 连接到SMTP服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }
	    // 接收服务器响应
    char response[256];
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;
    // 发送HELO命令
    send(sock, "HELO smtp.qq.com\r\n", 18, 0);
	    // 接收服务器响应
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

    // 进行身份验证
    std::string auth_cmd = "AUTH LOGIN\r\n";
    send(sock, auth_cmd.c_str(), auth_cmd.size(), 0);
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;
    // 发送用户名，这里需要等待服务器响应后再发送密码
        // 发送Base64编码的用户名
    std::string base64_username = base64_encode(from);
    write(sock, base64_username.c_str(), base64_username.size());
    write(sock, "\r\n", 2);
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

        // 发送Base64编码的密码
    std::string base64_password = base64_encode(password);
    write(sock, base64_password.c_str(), base64_password.size());
    write(sock, "\r\n", 2);

    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

    // 发送MAIL FROM命令
    std::string mail_from_cmd = "MAIL FROM:<" + from + ">\r\n";
    send(sock, mail_from_cmd.c_str(), mail_from_cmd.size(), 0);
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

    // 发送RCPT TO命令
    std::string rcpt_to_cmd = "RCPT TO:<" + to_qq_number + ">\r\n";
    send(sock, rcpt_to_cmd.c_str(), rcpt_to_cmd.size(), 0);
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

    // 发送DATA命令
    send(sock, "DATA\r\n", 6, 0);
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

    // 构建并发送邮件内容
    std::string headers = "From: " + from + "\r\n";
    headers += "To: " + to_qq_number + "\r\n";
    headers += "Subject: " + subject + "\r\n\r\n";
    std::string email_content = headers + message+"\r\n.\r\n";
    send(sock, email_content.c_str(), email_content.size(), 0);
     /*   // 构建并发送邮件内容
    std::string headers = "From: " + from + "\r\n";
    headers += "To: " + to_qq_number + "\r\n";
    headers += "Subject: " + subject + "\r\n\r\n";
    std::string email_content = headers + message + "\r\n.\r\n";
    SSL_write(ssl, email_content.c_str(), email_content.size());
    */


    // 结束邮件内容
    //send(sock, "\r\n.\r\n", 3, 0);
    read(sock, response, 256);
    std::cout << "Server response: " << response << std::endl;

    // 发送QUIT命令
    send(sock, "QUIT\r\n", 6, 0);

    // 关闭套接字
    close(sock);

    return 0;
}
