import socket
import os
import hashlib
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # 生成socket连接对象
ip_port = ("192.168.1.115", 6969)  # 地址和端口号
client.connect(ip_port)  # 连接
print("服务器已连接")
while True:
    content = input("input filename(start with get):")
    if len(content) == 0:
        continue  # 如果传入空字符会阻塞
    if content.startswith("get"):
        client.send(content.encode("utf-8"))  # 传送和接收都是bytes类型
        # 1.先接收长度，建议8192
        server_response = client.recv(1024)
        file_size = int(server_response.decode("utf-8"))
        print("接收到的大小：", file_size)
           # 2.接收文件内容
        client.send("准备好接收".encode("utf-8"))  # 接收确认
        filename = "new" + content.split(" ")[1]
        f = open(filename, "wb")
        received_size = 0
        m = hashlib.md5()
        while received_size < file_size:
            size = 0  # 准确接收数据大小，解决粘包
            if file_size - received_size > 1024:  # 多次接收
                size = 1024
            else:  # 最后一次接收完毕
                size = file_size - received_size
            data = client.recv(size)  # 多次接收内容，接收大数据
            data_len = len(data)
            received_size += data_len
            print("已接收：", int(received_size / file_size * 100), "%")
            m.update(data)
            f.write(data)
        f.close()
        print("实际接收的大小:", received_size)  # 解码
client.close()
