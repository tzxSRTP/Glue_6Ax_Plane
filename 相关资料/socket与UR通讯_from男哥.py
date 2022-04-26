import socket
import math
import numpy as np
import struct
import math
import parameters as pa


def PC2robot_TCP(x_r, y_r, z_r, rx_r, ry_r, rz_r):
    """
    function:让机械臂运动到要到达的 r系坐标
    :param x_r: 要到达的x
    :param y_r: 要到达的y
    :param z_r: 要到达的z
    :param rx_r: 要到达的rx
    :param ry_r: 要到达的ry
    :param rz_r: 要到达的rz
    :return: NONE
    """
    #socket连接机械臂
    sk = socket.socket()
    sk.connect(pa.target_ip)

    #发送指令处理
    send_data1 = 'def svt():\n  movej(p['
    send_data_x = str(x_r) + ','
    send_data_y = str(y_r) + ','
    send_data_z = str(z_r) + ','
    send_data_rx = str(rx_r) + ','
    send_data_ry = str(ry_r) + ','
    send_data_rz = str(rz_r)
    send_data2 = '],a=1.5,v=2,t=0,r=0)\nend\n'
    send_data = send_data1 + send_data_x + send_data_y + send_data_z + send_data_rx + send_data_ry + send_data_rz + send_data2

    sk.send(send_data.encode('utf8'))  # socket发送指令

    sk.close()  # 关闭socket通信


def get_R():
    """
    获取当前工具的：
    旋转矩阵
    旋转矢量
    :return:
    """
    # socket and connect
    sk = socket.socket()
    sk.connect(pa.target_ip)

    # get robot initial position
    data = bytes(sk.recv(1060))
    x, y, z = struct.unpack('!ddd', data[444:468])
    rx, ry, rz = struct.unpack('!ddd', data[468:492])

    print("原始工具位姿：{0} {1} {2} {3} {4} {5}".format(x, y, z, rx, ry, rz))
    # 计算旋转矩阵
    theta = math.sqrt(rx*rx + ry*ry + rz*rz)
    r = np.array([rx, ry, rz]).reshape((3, 1))
    n = r / theta
    n_ = np.array([[0, float(-n[2]), float(n[1])], [float(n[2]), 0, float(-n[0])], [float(-n[1]), float(n[0]), 0]])

    R = np.cos(theta) * np.eye(3) + (1-np.cos(theta)) * np.dot(n, n.T) + np.sin(theta)*n_

    tool = dict()
    tool['R'] = R
    tool['xyz'] = [x, y, z]
    tool['r'] = [rx, ry, rz]

    return tool


def R2r(R):
    """
    旋转矩阵转换为旋转矢量
    :param R:
    :param rx:
    :param ry:
    :param rz:
    :return:
    """
    theta = math.acos((np.trace(R)-1)/2)
    # print(theta)
    n_s = ((R-R.T)/2)/math.sin(theta)
    n = np.empty([3, 1])
    n[0] = n_s[2, 1]
    n[1] = n_s[0, 2]
    n[2] = n_s[1, 0]
    r = n*theta
    print("抓取旋转矢量：", r)

    return r
    # print(r[0]/rx, r[1]/ry, r[2]/rz)


def R2RPY(R):
    """
    旋转矩阵转换为欧拉角
    :param R:
    :return:
    """
    # 旋转矩阵转换为欧拉角
    sy = math.sqrt(R[2, 1] * R[2, 1] + R[2, 2] * R[2, 2])

    if sy > 1e-6:
        thetax = math.atan2(R[2, 1], R[2, 2]) * 180 / math.pi
        thetay = math.atan2(- R[2, 0], sy) * 180 / math.pi
        thetaz = math.atan2(R[1, 0], R[0,  0]) * 180 / math.pi
    else:
        thetax = math.atan2(-R[1, 2], R[1, 1]) * 180 / math.pi
        thetay = math.atan2(-R[2, 0], sy) * 180 / math.pi
        thetaz = 0
    print(thetax, thetay, thetaz)


if __name__ == "__main__":
    # 机械臂移动到原点位姿
    O = pa.Otool
    PC2robot_TCP(O[0], O[1], O[2], O[3], O[4], O[5])
