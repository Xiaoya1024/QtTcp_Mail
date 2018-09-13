#ifndef PORT_H
#define PORT_H
//客户端请求协议头
/*
    cmdtype		含义				示例
    0x00		登录请求			0x00 wangyang 6 123456
    0x01		获取所有已收到邮件	0x01 wangyang 0
    0x02		发送邮件请求		0x02 wangyang sizeof(邮件协议包) 邮件协议包 数据
    0x03		删除邮件请求		0x03 wangyang 14 20180904122343
    0x04		存储草稿箱请求		0x04 wangyang sizeof(邮件协议包) 邮件协议包 数据
    0x05		恢复已删除邮件请求  0x05 wangyang 14 20180904122343
    0x06		获取所有已发送邮件	0x06 wangyang 0
    0x07		获取所有草稿邮件	0x07 wangyang 0
    0x08		获取所有已删除邮件	0x08 wangyang 0
    0x09		获取所有未读邮件	0x09 wangyang 0
    0x0A		下载附件请求		0x0A wangyang 14 20180904122343
新增   0x0B     注册请求           0x0B wangyang sizeof(注册协议包) 注册协议包 数据
*/
#pragma pack (1)
typedef struct client_header{
    //表示客户端的请求类型
    char cmdType;
    //表示客户端的用户名(邮箱地址)
    //表示该数据包之后跟随的数据长度
    int length;
}client_header;

//登陆包
typedef struct login_package{
    char username[20];
    char password[20];
}login_package;

//注册协议包
typedef struct register_package{
    char username[20];
    char password[20];
    char phone[12];
}register_package;

//找回密码协议包
typedef struct findPass_package{
    char username[20];
    char phone[12];
}findPass_package;
//新密码协议包
typedef struct newPass_package{
    char password[20];
}newPass_package;



//邮件协议包
typedef struct mail_package{
    //邮件标题
        char title[30];
        //发送者
        char sender[20];
        //接收者
        char recver[20];
        //发送时间
        char time[20];
        //邮件内容只能发送50字
        char context[100];
        //附件个数
        int appendixCount;
}mail_package;

//附件协议包
typedef struct appendix_package{
    //附件文件名
    char appendixName[30];
    //附件内容大小
    int appendixLen;
}appendix_package;
typedef struct infmForMail_package
{
    char sender[20];
    char time[20];
}infmForMail_package;



//服务器端协议包
//服务器响应协议头
/*
    statu	含义											示例
    0x00	表示响应失败,length表示失败原因说明字符串的大小
    0x01	表示登录成功,length值为0						0x01 0
    0x02	表示响应获取所有已收邮件请求，length表示下一级协议大小
    0x03	表示响应发送邮件请求成功,length = 0;
    0x04	表示删除邮件请求成功,length = 0;
    0x05	存储邮件到草稿箱成功,length = 0;
    0x06	回复已删除邮件成功,length=0;
    0x07	响应获取所有已发送邮件, length表示下一级协议包大小
    0x08	响应获取所有草稿箱邮件.length表示下一级协议包大小
    0x09	响应获取所有已删除邮件请求，length表示下一个邮件协议包大小
    0x0A	响应获取所有未读邮件请求，length表示下一级协议包大小
    0x0B	响应获下载邮件请求，length表示下一个附件协议包大小
*/
typedef struct response_header{
    //响应状态码
    char statu;
    //数据长度
    int length;
}response_header;

//响应协议包
typedef struct response_package{
        //表示邮件协议包: 0x01, 附件协议包: 0x02;
        char repType;
        //reptype = 0x01,count表示邮件数量;
        //reptype = 0x02,count表示附件数量(本应用中值为1);
        //个数
        char count;
}response_package;

typedef struct simpleMail_package
{
    char title[30];
    //发送者
    char sender[20];
    //发送时间
    char time[20];
    //附件个数
    int appendixCount;

    int recvFlag;
}simpleMail_package;

typedef struct pathList_package
{
    int len;

}pathList_package;

#endif // PORT_H
