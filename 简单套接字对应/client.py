import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host='192.168.43.15'
port = 12345
print("Linked")
info=''
while info!='byebye':
    print("SERVER:"+info)
    msg=input()
    s.sendto(msg.encode(),(host,port))
    if msg=='byebye':
        break
    info = s.recv(1024).decode()
s.close()     
