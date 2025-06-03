from scapy.all import *

ans, _ = arping("172.20.10.15/24")
ans.summary()