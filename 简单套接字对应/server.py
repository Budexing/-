#-*-conding:utf-8-*-
'''
Created on 2018年11月19日

@author: Demons
'''
if __name__ == '__main__':
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    port = 12345
    s.settimeout(60)
    s.bind((" ", port))
    info, addr = s.recvfrom(8192)
    info = info.decode()
    print("start listening")
    while info != "byebye":
        print('CLIENT:' + info)
        msg = input()
        s.sendto(msg.encode(), addr)
        if msg == "byebye":
            break
        info = s.recv(1024).decode()
    s.close()
