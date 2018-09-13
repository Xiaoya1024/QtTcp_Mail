#ifndef PORT_H
#define PORT_H
#pragma pack (1)
//客户端请求协议结构


//请求协议头
struct header{
    //表示客户端的请求类型
    char cmdtype;
    //表示客户端的用户名
    char username[20];
    //表示该数据包之后更随的数据长度
    int length;
};
//cmdtype		含义				示例
//	0x00		登录请求			0x00 wangyang 6 123456
//	0x01		获取所有已收到邮件	0x01 wangyang 0
//	0x02		发送邮件请求		0x02 wangyang sizeof(邮件协议包) 邮件协议包 数据
//	0x03		删除邮件请求		0x03 wangyang 14 20180904122343
//	0x04		存储草稿箱请求		0x04 wangyang sizeof(邮件协议包) 邮件协议包 数据
//	0x05		恢复已删除邮件请求  0x05 wangyang 14 20180904122343
//	0x06		获取所有已发送邮件	0x06 wangyang 0
//	0x07		获取所有草稿邮件	0x07 wangyang 0
//	0x08		获取所有已删除邮件	0x08 wangyang 0
//	0x09		获取所有未读邮件	0x09 wangyang 0
//	0x0A		下载附件请求		0x0A wangyang 14 20180904122343
//二级协议包
struct mail_package{
    char sender[20];//发送者
    char recipient[20];//接收者
    char title[30];//邮件标题
    char time[14];//发送时间
    //附件名称
    short contentLen;//邮件内容长度
    char appendixCount;//附件个数
};
struct appendix_package{
    //附件文件名
    char appendixName[30];
    //附件内容大小
    short len;

};
//服务器响应协议
//响应协议头
struct response{
    //响应状态码
    char statu;
    //数据长度
    int length;
};
//  0x00	表示响应失败,length表示失败原因说明字符串的大小
//	0x01	表示登录成功,length值为0						0x01 0
//	0x02	表示响应获取所有已收邮件请求，length表示下一级协议大小
//	0x03	表示响应发送邮件请求成功,length = 0;
//	0x04	表示删除邮件请求成功,length = 0;
//	0x05	存储邮件到草稿箱成功,length = 0;
//	0x06	回复已删除邮件成功,length=0;
//	0x07	响应获取所有已发送邮件, length表示下一级协议包大小
//	0x08	响应获取所有草稿箱邮件.length表示下一级协议包大小
//	0x09	响应获取所有已删除邮件请求，length表示下一个邮件协议包大小
//	0x0A	响应获取所有未读邮件请求，length表示下一级协议包大小
//	0x0B	响应获下载邮件请求，length表示下一个附件协议包大小

//下一级协议包

struct response_package{
    //表示邮件协议包：0x01，附件协议包：0x02；
    char repType;
    //reptype = 0x01,count表示邮件数量;
    //reptype = 0x02,count表示附件数量(本应用中值为1);
    //个数
    int count;
};
#endif // PORT_H
